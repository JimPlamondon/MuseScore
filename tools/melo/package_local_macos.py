#!/usr/bin/env python3
"""Stage a self-contained local development app; never use Apple credentials."""
import argparse
import os
from pathlib import Path
import plistlib
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]

def run(*args):
    subprocess.run([str(x) for x in args], check=True)

def seal(output):
    preview = output / 'Contents/PlugIns/MuseScoreQuickLookPreviewExtension.appex'
    # Re-sign nested local code after deployment, then seal the containing bundle.
    # '-' is an ad-hoc signature, not an Apple distribution identity.
    for folder, _, names in os.walk(output, followlinks=False):
        for name in names:
            path = Path(folder) / name
            if path.is_symlink() or path in (output / 'Contents/MacOS/mscore', preview / 'Contents/MacOS/MuseScoreQuickLookPreviewExtension'):
                continue
            with path.open('rb') as f:
                magic = f.read(4)
            if magic in (b'\xcf\xfa\xed\xfe', b'\xce\xfa\xed\xfe', b'\xca\xfe\xba\xbe', b'\xca\xfe\xba\xbf'):
                run('codesign', '--force', '--sign', '-', '--timestamp=none', path)
    for framework in output.rglob('*.framework'):
        run('codesign', '--force', '--sign', '-', '--timestamp=none', framework)
    if preview.exists():
        run('codesign', '--force', '--sign', '-', '--timestamp=none', '--entitlements', ROOT / 'src/macos_integration/entitlements.plist', preview)
    run('codesign', '--force', '--sign', '-', '--timestamp=none', output)
    run('codesign', '--verify', '--deep', '--strict', output)
    check = [sys.executable, ROOT / 'tools/melo/check_score_identity.py', '--app', output]
    if preview.exists():
        check.append('--require-preview')
    run(*check)

def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--build', type=Path, required=True)
    p.add_argument('--output', type=Path, required=True, help='New .app path; existing destinations are refused')
    p.add_argument('--macdeployqt', type=Path, required=True)
    p.add_argument('--include-preview', action='store_true', help='Experimental: include the preview helper only after host validation')
    a = p.parse_args()
    if sys.platform != 'darwin':
        p.error('macOS is required')
    build, output = a.build.resolve(), a.output.resolve()
    if output.exists() or output.suffix != '.app':
        p.error('output must be a new .app path')
    cache = {}
    for line in (build / 'CMakeCache.txt').read_text().splitlines():
        if '=' in line and ':' in line.split('=', 1)[0]:
            key, value = line.split('=', 1)
            cache[key.split(':', 1)[0]] = value
    prefix = Path(cache['CMAKE_INSTALL_PREFIX'])
    if not prefix.is_relative_to(ROOT) or prefix == ROOT:
        p.error('configure CMAKE_INSTALL_PREFIX to a staging directory inside this source tree first')
    source_app = build / 'src/app/mscore.app'
    info = plistlib.loads((source_app / 'Contents/Info.plist').read_bytes())
    if info.get('CFBundleIdentifier') != 'com.melopresto.score':
        p.error('the built app does not have the MeloPresto Score identity')
    if cache.get('MUE_RUN_LRELEASE') == 'ON':
        run('cmake', '--build', build, '--target', 'translations', '-j', '8')
    run('cmake', '--install', build)
    output.parent.mkdir(parents=True, exist_ok=True)
    run('ditto', prefix / 'mscore.app', output)
    preview = output / 'Contents/PlugIns/MuseScoreQuickLookPreviewExtension.appex'
    if preview.exists() and not a.include_preview:
        shutil.rmtree(preview)
    deploy = [a.macdeployqt, output, '-always-overwrite', '-verbose=1', '-qmldir=' + str(ROOT)]
    if preview.exists():
        deploy.append('-executable=' + str(preview / 'Contents/MacOS/MuseScoreQuickLookPreviewExtension'))
    run(*deploy)
    # Preserve upstream QML isolation from Qt-based VST plugins.
    qml = output / 'Contents/Resources/qml'
    if qml.exists():
        qml.rename(qml.with_name('qml_mu'))
        config = output / 'Contents/Resources/qt.conf'
        config.write_text(config.read_text().replace('Resources/qml', 'Resources/qml_mu'))
    for dsym in output.rglob('*.dSYM'):
        if dsym.is_dir():
            shutil.rmtree(dsym)
    seal(output)
    print('Local development app:', output)

if __name__ == '__main__':
    main()
