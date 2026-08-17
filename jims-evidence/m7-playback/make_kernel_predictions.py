#!/usr/bin/env python3
"""JiMStaff Milestone 7 — Kernel-generated playback predictions.

For each fixture score, reads the JiMS staff state in force per bar (base
staff type; a StaffTypeChange carrier installs its state from its bar) and
every note's lattice identity, and asks the MERGED Kernel bridge
(`cargo run --release -p jims-musescore-bridge --example request` in the
Kernel checkout named by JIMS_ROOT) for the note's sounding pitch. Nothing
here computes a frequency; timings assume 4/4 at the score's tempo (whole
notes = 4 beats) starting at t = 0.

usage: make_kernel_predictions.py <out.json> [JIMS_ROOT]
"""
import hashlib
import html
import json
import os
import re
import subprocess
import sys
import zipfile

HERE = os.path.dirname(os.path.abspath(__file__))
SCORES = os.path.join(HERE, 'scores')
JIMS_ROOT = sys.argv[2] if len(sys.argv) > 2 else os.environ.get('JIMS_ROOT', '/Users/jim/Developer/JiMS/GitHub/jims')
WORKSPACE = os.path.join(JIMS_ROOT, 'Libraries', 'jims')

DUR_BEATS = {'whole': 4.0, 'half': 2.0, 'quarter': 1.0, 'eighth': 0.5, '16th': 0.25}


def bridge(envelope):
    out = subprocess.run(
        ['cargo', 'run', '-q', '--release', '-p', 'jims-musescore-bridge', '--example', 'request', '--', json.dumps(envelope)],
        cwd=WORKSPACE, capture_output=True, text=True, check=True)
    resp = json.loads(out.stdout.strip().splitlines()[-1])
    if not resp.get('ok'):
        raise SystemExit(f'bridge refused: {resp}')
    return resp['result']


def load_mscx(path):
    if path.endswith('.mscz'):
        with zipfile.ZipFile(path) as z:
            name = [n for n in z.namelist() if n.endswith('.mscx')][0]
            return z.read(name).decode('utf-8')
    return open(path, encoding='utf-8').read()


def analyse(path):
    x = load_mscx(path)
    tempo_m = re.search(r'<Tempo>.*?<tempo>([0-9.]+)</tempo>', x, re.S)
    bps = float(tempo_m.group(1)) if tempo_m else 2.0  # MuseScore default 120 bpm = 2 beats/s
    # Base staff state: the Part's <Staff> StaffType.
    part_staff = re.search(r'<Part[ >].*?</Part>', x, re.S).group(0)
    base_state = html.unescape(re.search(r'<jimsStateJson>(.*?)</jimsStateJson>', part_staff, re.S).group(1))
    # Score staff measures.
    score_staff = x[x.index('<Staff id="1">', x.index('</Part>')):]
    score_staff = score_staff[:score_staff.index('</Staff>')]
    measures = re.findall(r'<Measure>(.*?)</Measure>', score_staff, re.S)
    notes = []
    t = 0.0
    state = base_state
    for mi, m in enumerate(measures):
        carrier = re.search(r'<StaffTypeChange>.*?<jimsStateJson>(.*?)</jimsStateJson>.*?</StaffTypeChange>', m, re.S)
        if carrier:
            state = html.unescape(carrier.group(1))
        # Only voice 1 chords/rests in document order.
        for item in re.finditer(r'<(Chord|Rest)>(.*?)</\1>', m, re.S):
            kind, body = item.group(1), item.group(2)
            dur = re.search(r'<durationType>([^<]+)</durationType>', body).group(1)
            beats = 4.0 if dur == 'measure' else DUR_BEATS[dur]
            seconds = beats / bps
            if kind == 'Chord':
                for n in re.finditer(r'<Note>(.*?)</Note>', body, re.S):
                    nb = n.group(1)
                    nper = int(re.search(r'<jimsNPer>(-?\d+)', nb).group(1))
                    ngen = int(re.search(r'<jimsNGen>(-?\d+)', nb).group(1))
                    notes.append(dict(bar=mi + 1, start_s=round(t, 4), end_s=round(t + seconds, 4), nPer=nper, nGen=ngen, state=state))
            t += seconds
    return notes


def main():
    out_path = sys.argv[1]
    manifest = {'schema': 'jims.m7-playback-predictions.v1', 'source': 'Kernel bridge op note_sounding_pitch via jims-musescore-bridge example `request`',
                'kernel_root': JIMS_ROOT, 'renders': []}
    for fname in sorted(os.listdir(SCORES)):
        path = os.path.join(SCORES, fname)
        sha = hashlib.sha256(open(path, 'rb').read()).hexdigest()
        rows = []
        for n in analyse(path):
            state = json.loads(n['state'])
            r = bridge({'abi': 2, 'op': 'note_sounding_pitch', 'state': state, 'nPer': n['nPer'], 'nGen': n['nGen']})
            rows.append(dict(
                label=f"bar{n['bar']}({n['nPer']},{n['nGen']})", bar=n['bar'], nPer=n['nPer'], nGen=n['nGen'],
                start_s=n['start_s'], end_s=n['end_s'],
                reference=r['reference'], reference_key_number=r['reference_key_number'], anchor=r['anchor'],
                predicted_hz=r['frequency_hz'], midi_key=r['midi_key'], cents_offset=r['cents_offset'],
                generator_cents=state['generator_cents'], period_cents=state['period_cents']))
        manifest['renders'].append({'score': fname, 'score_sha256': sha, 'wav': os.path.splitext(fname)[0] + '.wav', 'notes': rows})
        print(fname, len(rows), 'notes')
    json.dump(manifest, open(out_path, 'w'), indent=1)


if __name__ == '__main__':
    main()
