#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2026 Jim Plamondon
# SPDX-License-Identifier: GPL-3.0-only
"""Unit tests for enriched_to_jims_mscx.py — MusicXML V3 change events.

Run: python3 tools/jims/test_enriched_to_jims_mscx.py

The converter is pure transcription: it reads every jims:staff-state of the
enriched file (V1, V2, or V3 namespace), transcribes the first into the
StaffType and every later one into a StaffTypeChange at its measure, and
never inspects the Kernel-written jims:change summary. These tests pin that
contract with a minimal stock .mscx skeleton (no MuseScore binary needed).
"""
import json
import os
import re
import subprocess
import sys
import tempfile
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
CONVERTER = os.path.join(HERE, "enriched_to_jims_mscx.py")

STATE_XML = """<jims:staff-state>
  <jims:scale><jims:step>M2</jims:step><jims:step>m2</jims:step><jims:step>M2</jims:step><jims:step>M2</jims:step><jims:step>M2</jims:step><jims:step>m2</jims:step><jims:step>M2</jims:step></jims:scale>
  <jims:collection-rotation>{coll}</jims:collection-rotation>
  <jims:mode-rotation>{mode}</jims:mode-rotation>
  <jims:generator-cents>700</jims:generator-cents>
  <jims:period-cents>1200</jims:period-cents>
  <jims:embedding large-steps="5" small-steps="2"/>
  <jims:extent lower-do-register="4" period-count="1"/>{tonic}{reference}
</jims:staff-state>"""


def state_xml(mode=0, coll=0, reference="", tonic=""):
    return STATE_XML.format(mode=mode, coll=coll,
                            reference=("\n  <jims:reference>" + reference + "</jims:reference>") if reference else "",
                            tonic=("\n  <jims:tonic-ambit>" + tonic + "</jims:tonic-ambit>") if tonic else "")


def enriched(ns, measures):
    """measures: list of (state_xml_or_None, change_xml_or_None, note_count)."""
    body = []
    for i, (state, change, notes) in enumerate(measures, start=1):
        attrs = ""
        if state or change:
            attrs = "<attributes><divisions>1</divisions>" + (state or "") + (change or "") + "</attributes>"
        ns_notes = "".join('<note><pitch><step>C</step><octave>4</octave></pitch><jims:pitch n-per="1" n-gen="-2"/><duration>1</duration></note>' for _ in range(notes))
        body.append(f'<measure number="{i}">{attrs}{ns_notes}</measure>')
    return (f'<?xml version="1.0" encoding="UTF-8"?><score-partwise version="4.0" xmlns:jims="{ns}">'
            '<part-list><score-part id="P1"><part-name>J</part-name></score-part></part-list>'
            '<part id="P1">' + "".join(body) + '</part></score-partwise>')


def stock_mscx(measures, notes_per_measure):
    ms = []
    for _ in range(measures):
        notes = "".join("<voice><Chord><Note>\n              <pitch>60</pitch>\n            </Note></Chord></voice>" for _ in range(notes_per_measure))
        ms.append("<Measure>\n        " + notes + "\n      </Measure>")
    return ('<museScore version="4.60"><Score><Style></Style><Part><Staff id="1"><StaffType group="pitched"><name>stdNormal</name></StaffType></Staff></Part><Staff id="1">'
            + "".join(ms) + '</Staff></Score></museScore>')


def run(enriched_text, mscx_text, env=None, lines=13):
    with tempfile.TemporaryDirectory() as d:
        e = os.path.join(d, "e.musicxml"); m = os.path.join(d, "in.mscx"); o = os.path.join(d, "out.mscx")
        with open(e, "w") as f:
            f.write(enriched_text)
        with open(m, "w") as f:
            f.write(mscx_text)
        full_env = {k: v for k, v in os.environ.items() if not k.startswith("JIMS_")}
        full_env.update(env or {})
        p = subprocess.run([sys.executable, CONVERTER, e, m, o, str(lines)], capture_output=True, text=True, env=full_env)
        out = ""
        if p.returncode == 0:
            with open(o) as f:
                out = f.read()
        return p.returncode, p.stdout + p.stderr, out


def states_in(mscx):
    return [json.loads(s) for s in re.findall(r"<jimsStateJson>([^<]*)</jimsStateJson>", mscx)]


V3 = "urn:jims:musicxml:3"
V2 = "urn:jims:musicxml:2"
V1 = "urn:jims:musicxml:1"
CHANGE = ('<jims:change><jims:kind>key</jims:kind><jims:kind>mode</jims:kind>'
          '<jims:key-change n-per="-1" n-gen="3" direction="up"/>'
          '<jims:mode-change old-tonic-n-gen="-2" old-tonic-label="Do" new-tonic-n-gen="1" new-tonic-label="La"/></jims:change>')


class V3Timeline(unittest.TestCase):
    def test_every_reference_form_transcribes_and_none_is_explicit_or_absent(self):
        for xml, expected in [
            ("<jims:none/>", "none"),
            ('<jims:reference-pitch key-number="62"/>', {"reference-pitch": {"key_number": 62}}),
            ("<jims:pitch-class>2</jims:pitch-class>", {"pitch-class": {"pitch_class": 2}}),
            ("<jims:frequency-hz>293.665</jims:frequency-hz>", {"frequency-hz": {"hertz": 293.665}}),
        ]:
            rc, log, out = run(enriched(V3, [(state_xml(reference=xml), None, 2)]), stock_mscx(1, 2))
            self.assertEqual(rc, 0, log)
            self.assertEqual(states_in(out)[0]["reference"], expected)
        # V2 (absent reference) and V1 read as none.
        for ns in (V2, V1):
            rc, log, out = run(enriched(ns, [(state_xml(), None, 2)]), stock_mscx(1, 2))
            self.assertEqual(rc, 0, log)
            self.assertEqual(states_in(out)[0]["reference"], "none")

    def test_later_states_become_staff_type_changes_at_their_measures_without_flags(self):
        doc = enriched(V3, [
            (state_xml(reference='<jims:reference-pitch key-number="62"/>', tonic="tonic-bounded"), None, 2),
            (None, None, 2),
            (state_xml(mode=5, reference='<jims:reference-pitch key-number="53"/>', tonic="tonic-bounded"), CHANGE, 1),
            (state_xml(mode=5, coll=-3, reference='<jims:reference-pitch key-number="53"/>', tonic="tonic-bounded"),
             '<jims:change><jims:kind>scale</jims:kind><jims:scale-change old-collection-rotation="0" new-collection-rotation="-3"/></jims:change>', 1),
        ])
        # Stock skeleton with the same note count per measure as the enriched file (2,2,1,1).
        skeleton = stock_mscx(4, 1).replace("</Note></Chord></voice>\n      </Measure>", "</Note></Chord></voice><voice><Chord><Note>\n              <pitch>60</pitch>\n            </Note></Chord></voice>\n      </Measure>", 2)
        rc, log, out = run(doc, skeleton)
        self.assertEqual(rc, 0, log)
        states = states_in(out)
        self.assertEqual(len(states), 3)
        self.assertEqual(states[0]["reference"], {"reference-pitch": {"key_number": 62}})
        self.assertEqual(states[1]["mode_rotation"], 5)
        self.assertEqual(states[1]["reference"], {"reference-pitch": {"key_number": 53}})
        self.assertEqual(states[2]["collection_rotation"], -3)
        for s in states:
            self.assertEqual(s["tonic_ambit"], "tonic-bounded")
            self.assertEqual(list(s)[-1], "tonic_ambit")
        # StaffTypeChange carriers sit at measures 3 and 4 (document order).
        measures = re.findall(r"<Measure>(.*?)</Measure>", out, re.S)
        self.assertEqual([("<StaffTypeChange>" in m) for m in measures], [False, False, True, True])
        self.assertEqual(out.count("<jimsNPer>"), 6)

    def test_change_summary_is_never_inspected(self):
        base = [(state_xml(reference="<jims:none/>"), None, 1), (state_xml(mode=5, reference="<jims:none/>"), CHANGE, 1)]
        corrupt = [(state_xml(reference="<jims:none/>"), None, 1),
                   (state_xml(mode=5, reference="<jims:none/>"), CHANGE.replace('direction="up"', 'direction="down"').replace("La", "Ti"), 1)]
        rc1, _, out1 = run(enriched(V3, base), stock_mscx(2, 1))
        rc2, _, out2 = run(enriched(V3, corrupt), stock_mscx(2, 1))
        self.assertEqual((rc1, rc2), (0, 0))
        self.assertEqual(out1, out2)

    def test_override_flags_are_rejected_beside_an_xml_timeline(self):
        doc = enriched(V3, [(state_xml(reference="<jims:none/>"), None, 1), (state_xml(mode=5, reference="<jims:none/>"), CHANGE, 1)])
        for env in ({"JIMS_MODE_CHANGE": "2:5"}, {"JIMS_REFERENCE": "62"}, {"JIMS_SCALE_CHANGE": "2:-3"},
                    {"JIMS_KEY_CHANGE": "2:0:1", "JIMS_REFERENCE": "62"}):
            rc, log, _ = run(doc, stock_mscx(2, 1), env=env)
            self.assertNotEqual(rc, 0, env)
            self.assertIn("timeline", log)

    def test_hand_authored_single_state_fixtures_keep_the_override_path(self):
        doc = enriched(V1, [(state_xml(), None, 2), (None, None, 2)])
        rc, log, out = run(doc, stock_mscx(2, 2), env={"JIMS_REFERENCE": "62", "JIMS_KEY_CHANGE": "2:-1:3", "JIMS_MODE_CHANGE": "2:5", "JIMS_TONIC_TOKEN": "tonic-bounded"})
        self.assertEqual(rc, 0, log)
        states = states_in(out)
        self.assertEqual(len(states), 2)
        self.assertEqual(states[0]["reference"], {"reference-pitch": {"key_number": 62}})
        self.assertEqual(states[1]["reference"], {"reference-pitch": {"key_number": 53}})
        self.assertEqual(states[1]["mode_rotation"], 5)
        self.assertEqual(states[1]["tonic_ambit"], "tonic-bounded")

    def test_out_of_contract_documents_are_rejected(self):
        rc, log, _ = run(enriched(V3, [(None, None, 1), (state_xml(reference="<jims:none/>"), None, 1)]), stock_mscx(2, 1))
        self.assertNotEqual(rc, 0); self.assertIn("measure 1", log)
        rc, log, _ = run(enriched(V3, [(state_xml(reference="<jims:none/><jims:pitch-class>2</jims:pitch-class>"), None, 1)]), stock_mscx(1, 1))
        self.assertNotEqual(rc, 0); self.assertIn("exactly one form", log)
        rc, log, _ = run(enriched("urn:jims:musicxml:9", [(state_xml(), None, 1)]), stock_mscx(1, 1))
        self.assertNotEqual(rc, 0); self.assertIn("unsupported", log)


if __name__ == "__main__":
    unittest.main()
