#!/usr/bin/env python3
"""JiMStaff Milestone 7 — synthesizer calibration (stock control).

The MS Basic soundfont's samples are not perfectly tuned (a stock, non-JiMS
piano F3 measures about -7 cents from 174.61 Hz through the same CLI
render). To judge JiMS playback on its own merits, this control measures
every MIDI key's stock offset once: it writes a plain MusicXML score (no
JiMS anywhere) with one whole note per key, renders it with the same
`mscore -o <wav>` path, measures each note with the same estimator, and
records offset_cents(key) = measured vs 12-TET nominal (A4 = 440 Hz).
verify_frequencies.py then reports each JiMS note's raw error AND its
error relative to prediction + this per-key offset (the calibrated error
the 5-cent oracle applies to). Both numbers are kept in the results.

usage: calibrate_synth.py <mscore-binary> <work-dir> <out synth-calibration.json> [lo_key hi_key]
"""
import json
import os
import subprocess
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from verify_frequencies import estimate_frequency, read_wav  # noqa: E402

STEPS = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']


def musicxml_for_keys(keys):
    notes = []
    for k in keys:
        step = STEPS[k % 12]
        alter = '<alter>1</alter>' if '#' in step else ''
        notes.append(
            f'<note><pitch><step>{step[0]}</step>{alter}<octave>{k // 12 - 1}</octave></pitch>'
            f'<duration>4</duration><type>whole</type></note>')
    measures = ''.join(
        f'<measure number="{i + 1}">'
        + ('<attributes><divisions>1</divisions><key><fifths>0</fifths></key><time><beats>4</beats><beat-type>4</beat-type></time>'
           '<clef><sign>G</sign><line>2</line></clef></attributes>'
           '<direction placement="above"><direction-type><metronome><beat-unit>quarter</beat-unit><per-minute>120</per-minute>'
           '</metronome></direction-type><sound tempo="120"/></direction>' if i == 0 else '')
        + n + '</measure>'
        for i, n in enumerate(notes))
    return ('<?xml version="1.0" encoding="UTF-8"?>\n<score-partwise version="4.0"><part-list><score-part id="P1">'
            '<part-name>Piano</part-name></score-part></part-list><part id="P1">' + measures + '</part></score-partwise>\n')


def main():
    mscore, work, out = sys.argv[1], sys.argv[2], sys.argv[3]
    lo = int(sys.argv[4]) if len(sys.argv) > 4 else 48
    hi = int(sys.argv[5]) if len(sys.argv) > 5 else 84
    keys = list(range(lo, hi + 1))
    os.makedirs(work, exist_ok=True)
    xml = os.path.join(work, 'synth-calibration.musicxml')
    wav = os.path.join(work, 'synth-calibration.wav')
    open(xml, 'w').write(musicxml_for_keys(keys))
    subprocess.run([mscore, '-o', wav, xml], check=True, capture_output=True)
    rate, samples = read_wav(wav)
    result = {'schema': 'jims.m7-synth-calibration.v1', 'instrument': 'MS Basic (default piano), MuseScore CLI -o wav',
              'note_seconds': 2.0, 'offsets_cents': {}}
    for i, k in enumerate(keys):
        t0 = i * 2.0
        seg = samples[int((t0 + 0.25) * rate):int((t0 + 2.0 - 0.15) * rate)]
        f, why = estimate_frequency(rate, seg)
        nominal = 440.0 * 2 ** ((k - 69) / 12.0)
        if f is None:
            result['offsets_cents'][str(k)] = None
            print(f'key {k}: {why}')
        else:
            off = 1200.0 * np.log2(f / nominal)
            result['offsets_cents'][str(k)] = round(float(off), 3)
            print(f'key {k}: nominal {nominal:.3f} measured {f:.3f} offset {off:+.3f} cents')
    json.dump(result, open(out, 'w'), indent=1)


if __name__ == '__main__':
    main()
