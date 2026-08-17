#!/usr/bin/env python3
"""JiMStaff Milestone 7 — automated sensory evidence.

Measures the fundamental frequency of every predicted note window in a
MuseScore CLI WAV render and compares it with the Kernel's prediction
(kernel-predictions.json, generated through the merged bridge — never
hand-derived). Pass = every analyzed note within TOLERANCE_CENTS (5 cents
absolute, the FINAL's oracle).

Analyzer: reads RIFF/WAVE (PCM 16-bit or IEEE float 32-bit), mixes to mono,
takes each note's window (skipping attack/release margins), applies a Hann
window, finds the fundamental with a 5-partial Harmonic Product Spectrum
(robust to piano tones whose 2nd/3rd partial outweighs the fundamental),
refines by parabolic interpolation on the fundamental's own spectral peak,
and fails on silent, clipped, or ambiguous windows.

Synth calibration: with a fourth argument (synth-calibration.json from
calibrate_synth.py — the stock, non-JiMS per-MIDI-key offset of the same
soundfont through the same render path), each note's error is also
reported relative to prediction + that key's stock offset; the 5-cent
oracle then applies to this calibrated error, and the raw error stays in
the results beside it. Without a calibration file the raw error is judged.

usage: verify_frequencies.py <kernel-predictions.json> <wav-dir> [results.json] [synth-calibration.json]
"""
import json
import struct
import sys

import numpy as np

TOLERANCE_CENTS = 5.0
ATTACK_S = 0.25
RELEASE_S = 0.15


def read_wav(path):
    with open(path, 'rb') as f:
        data = f.read()
    if data[:4] != b'RIFF' or data[8:12] != b'WAVE':
        raise ValueError(f'{path}: not a RIFF/WAVE file')
    pos = 12
    fmt = None
    pcm = None
    while pos + 8 <= len(data):
        cid = data[pos:pos + 4]
        size = struct.unpack('<I', data[pos + 4:pos + 8])[0]
        body = data[pos + 8:pos + 8 + size]
        if cid == b'fmt ':
            tag, channels, rate, _, _, bits = struct.unpack('<HHIIHH', body[:16])
            if tag == 0xFFFE and len(body) >= 26:  # WAVE_FORMAT_EXTENSIBLE: sub-format GUID's first two bytes
                tag = struct.unpack('<H', body[24:26])[0]
            fmt = (tag, channels, rate, bits)
        elif cid == b'data':
            pcm = body
        pos += 8 + size + (size & 1)
    if fmt is None or pcm is None:
        raise ValueError(f'{path}: missing fmt/data chunk')
    tag, channels, rate, bits = fmt
    if tag == 3 and bits == 32:
        samples = np.frombuffer(pcm, dtype='<f4').astype(np.float64)
    elif tag == 1 and bits == 16:
        samples = np.frombuffer(pcm, dtype='<i2').astype(np.float64) / 32768.0
    elif tag == 1 and bits == 24:
        raw = np.frombuffer(pcm, dtype=np.uint8).reshape(-1, 3)
        ints = (raw[:, 0].astype(np.int32) | (raw[:, 1].astype(np.int32) << 8) | (raw[:, 2].astype(np.int32) << 16))
        ints = np.where(ints >= 1 << 23, ints - (1 << 24), ints)
        samples = ints.astype(np.float64) / float(1 << 23)
    else:
        raise ValueError(f'{path}: unsupported WAV format tag={tag} bits={bits}')
    if channels > 1:
        samples = samples[:len(samples) - len(samples) % channels].reshape(-1, channels).mean(axis=1)
    return rate, samples


def estimate_frequency(rate, seg):
    """Fundamental of a sustained note window.

    Harmonic Product Spectrum (HPS, 5 partials) finds the fundamental even
    when a piano tone's 2nd/3rd partial outweighs its fundamental (no octave
    errors); the estimate is then refined by parabolic interpolation on the
    plain spectrum around the fundamental's own bin. Silent, clipped, and
    ambiguous windows (an HPS runner-up within 60 % of the winner outside
    the winner's neighbourhood and octave relations) fail explicitly.
    """
    n = len(seg)
    if n < 2048:
        return None, 'window too short'
    peak_abs = float(np.max(np.abs(seg)))
    if peak_abs < 1e-4:
        return None, 'silent window'
    if peak_abs >= 0.999:
        return None, 'clipped window'
    nfft = 1 << int(np.ceil(np.log2(n * 4)))
    win = seg * np.hanning(n)
    spec = np.abs(np.fft.rfft(win, n=nfft))
    freqs = np.fft.rfftfreq(nfft, 1.0 / rate)
    df = freqs[1] - freqs[0]
    lo = int(np.searchsorted(freqs, 40.0))
    hi = int(np.searchsorted(freqs, 2000.0))
    # HPS over 5 partials.
    hps = np.log(spec[:hi] + 1e-12).copy()
    for h in range(2, 6):
        dec = np.log(spec[::h][:hi] + 1e-12)
        hps[:len(dec)] += dec
        hps[len(dec):] += np.log(1e-12)
    band = hps[lo:hi]
    k = int(np.argmax(band)) + lo
    # Ambiguity: a runner-up (outside +/-3 % of the winner and not at 2x/0.5x) too close.
    others = band.copy()
    f0 = freqs[k]
    def zero_near(f, frac=0.03):
        j0 = int(np.searchsorted(freqs, f * (1 - frac))) - lo
        j1 = int(np.searchsorted(freqs, f * (1 + frac))) - lo
        others[max(0, j0):max(0, j1) + 1] = -np.inf
    for f in (f0, f0 * 2, f0 / 2, f0 * 3, f0 / 3):
        zero_near(f)
    winner = band[k - lo]
    runner = float(np.max(others)) if np.isfinite(np.max(others)) else -np.inf
    # HPS values are log-sums; compare linear ratio of the 5-partial products.
    if np.isfinite(runner) and np.exp(runner - winner) > 0.6:
        return None, 'ambiguous window (HPS runner-up too close)'
    # Refine on the plain spectrum around the fundamental bin.
    w = max(1, int(round(f0 * 0.02 / df)))
    j = int(np.argmax(spec[max(lo, k - w):k + w + 1])) + max(lo, k - w)
    if 1 <= j < len(spec) - 1:
        a, b, c = np.log(spec[j - 1] + 1e-12), np.log(spec[j] + 1e-12), np.log(spec[j + 1] + 1e-12)
        denom = (a - 2 * b + c)
        delta = 0.5 * (a - c) / denom if abs(denom) > 1e-12 else 0.0
        f0 = freqs[j] + delta * df
    else:
        f0 = freqs[j]
    return float(f0), None


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        sys.exit(2)
    manifest = json.load(open(sys.argv[1]))
    wav_dir = sys.argv[2]
    out_path = sys.argv[3] if len(sys.argv) > 3 else None
    calibration = json.load(open(sys.argv[4]))['offsets_cents'] if len(sys.argv) > 4 else None
    results = {'tolerance_cents': TOLERANCE_CENTS, 'calibrated': calibration is not None, 'renders': []}
    all_pass = True
    for render in manifest['renders']:
        rate, samples = read_wav(f"{wav_dir}/{render['wav']}")
        rows = []
        for note in render['notes']:
            # Margins skip the attack transient and the release; short notes
            # keep a proportional share so the window stays analyzable.
            dur = note['end_s'] - note['start_s']
            start = note['start_s'] + min(ATTACK_S, 0.3 * dur)
            end = note['end_s'] - min(RELEASE_S, 0.2 * dur)
            seg = samples[int(start * rate):int(end * rate)]
            measured, why = estimate_frequency(rate, seg)
            predicted = note['predicted_hz']
            row = dict(note)
            if measured is None:
                row.update(measured_hz=None, error_cents=None, ok=False, reason=why)
                all_pass = False
            else:
                cents = 1200.0 * np.log2(measured / predicted)
                synth_offset = None
                if calibration is not None:
                    synth_offset = calibration.get(str(note['midi_key']))
                judged = cents - synth_offset if synth_offset is not None else cents
                ok = abs(judged) <= TOLERANCE_CENTS
                row.update(measured_hz=round(measured, 4), error_cents=round(float(cents), 3),
                           synth_offset_cents=synth_offset,
                           calibrated_error_cents=round(float(judged), 3) if synth_offset is not None else None,
                           ok=bool(ok))
                all_pass = all_pass and ok
            rows.append(row)
            status = 'OK ' if row['ok'] else 'FAIL'
            cal = f" calibrated {row['calibrated_error_cents']:+.3f}" if row.get('calibrated_error_cents') is not None else ''
            print(f"{status} {render['score']:28s} {note['label']:>10s} predicted {predicted:9.3f} Hz "
                  f"measured {row['measured_hz'] if row['measured_hz'] is not None else '   ---   '} "
                  f"(raw {row['error_cents'] if row['error_cents'] is not None else row.get('reason')} cents{cal})")
        results['renders'].append({'score': render['score'], 'wav': render['wav'], 'notes': rows})
    results['all_pass'] = bool(all_pass)
    if out_path:
        json.dump(results, open(out_path, 'w'), indent=1)
    print('ALL PASS' if all_pass else 'FAILURES PRESENT')
    sys.exit(0 if all_pass else 1)


if __name__ == '__main__':
    main()
