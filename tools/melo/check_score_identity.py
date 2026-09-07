#!/usr/bin/env python3
"""Validate the produced local macOS app's identity and document ownership."""
import argparse
import plistlib
from pathlib import Path

p = argparse.ArgumentParser()
p.add_argument('--app', type=Path, required=True)
p.add_argument('--require-preview', action='store_true')
a = p.parse_args()
info = plistlib.loads((a.app / 'Contents/Info.plist').read_bytes())
errors = []
def check(ok, message):
    if not ok:
        errors.append(message)
check(info.get('CFBundleIdentifier') == 'com.melopresto.score', 'application bundle identity')
check(info.get('CFBundleName') == 'MeloPresto Score', 'application display name')
check(info.get('CFBundlePackageType') == 'APPL', 'application package type')
check((a.app / 'Contents/MacOS' / info.get('CFBundleExecutable', '')).is_file(), 'executable exists')
check((a.app / 'Contents/Resources' / info.get('CFBundleIconFile', '')).is_file(), 'application icon exists')
exports = {x['UTTypeIdentifier']: x for x in info.get('UTExportedTypeDeclarations', [])}
check('com.melopresto.score.document' in exports, 'native score type is exported')
check('com.melopresto.score.backup' in exports, 'native backup type is exported')
check(not any(x.startswith('org.musescore.') for x in exports), 'inherited types must not be exported as ours')
if 'com.melopresto.score.document' in exports:
    t = exports['com.melopresto.score.document']
    check('meloscore' in t['UTTypeTagSpecification']['public.filename-extension'], 'native extension')
    check('public.data' in t['UTTypeConformsTo'], 'Finder data conformance')
    check((a.app / 'Contents/Resources' / t['UTTypeIconFile']).is_file(), 'document icon exists')
for t in info.get('CFBundleDocumentTypes', []):
    types = t.get('LSItemContentTypes', [])
    if 'com.melopresto.score.document' in types:
        check(t.get('LSHandlerRank') == 'Owner' and t.get('CFBundleTypeRole') == 'Editor', 'own native document editor role')
        check(not t.get('LSTypeIsPackage'), 'native archive remains a regular file')
    icon = t.get('CFBundleTypeIconFile')
    if icon:
        check((a.app / 'Contents/Resources' / icon).is_file(), 'declared document icon exists: ' + icon)
    if any(x.startswith('org.musescore.') for x in types):
        check(t.get('LSHandlerRank') == 'Alternate', 'MuseScore document role remains alternate')
check(not info.get('CFBundleURLTypes'), 'no inherited or unimplemented external URL registration')
children = list((a.app / 'Contents/PlugIns').glob('*.appex'))
check(not a.require_preview or bool(children), 'preview extension is bundled')
for child in children:
    metadata = plistlib.loads((child / 'Contents/Info.plist').read_bytes())
    check(metadata['CFBundleIdentifier'].startswith('com.melopresto.score.'), 'preview helper identity')
    check(metadata.get('NSExtension', {}).get('NSExtensionAttributes', {}).get('QLSupportedContentTypes') == ['com.melopresto.score.document'], 'preview helper supports only the owned native score type')
if errors:
    raise SystemExit('FAIL: ' + '; '.join(errors))
print('PASS: built app identity, icons, own native type and alternate inherited formats')
