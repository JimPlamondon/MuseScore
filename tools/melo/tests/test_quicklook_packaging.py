"""Keep the default local package's real preview extension, not an opt-in stub."""
import contextlib
import importlib.util
import io
from pathlib import Path
import plistlib
import shutil
import sys
import tempfile
import unittest
from unittest.mock import patch

SPEC = importlib.util.spec_from_file_location('score_package', Path(__file__).resolve().parents[1] / 'package_local_macos.py')
PACKAGE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PACKAGE)
EXTENSION = Path('Contents/PlugIns/MuseScoreQuickLookPreviewExtension.appex')


class QuickLookPackagingTests(unittest.TestCase):
    def package_fixture(self, root, include_extension):
        root = root.resolve()
        build, prefix, output = root / 'build', root / 'stage', root / 'Delivered.app'
        info = build / 'src/app/mscore.app/Contents/Info.plist'
        info.parent.mkdir(parents=True)
        info.write_bytes(plistlib.dumps({'CFBundleIdentifier': 'com.melopresto.score'}))
        (build / 'CMakeCache.txt').write_text(f'CMAKE_INSTALL_PREFIX:PATH={prefix}\n')
        staged = prefix / 'mscore.app'
        (staged / 'Contents').mkdir(parents=True)
        if include_extension:
            (staged / EXTENSION / 'Contents/MacOS').mkdir(parents=True)
        calls = []

        def run(*args):
            calls.append(tuple(str(arg) for arg in args))
            if args[0] == 'ditto':
                shutil.copytree(args[1], args[2])

        argv = ['package_local_macos.py', '--build', str(build), '--output', str(output), '--macdeployqt', '/test/macdeployqt']
        with patch.object(PACKAGE, 'ROOT', root), patch.object(PACKAGE, 'run', run), \
                patch.object(PACKAGE, 'seal') as seal, patch.object(sys, 'argv', argv), \
                patch.object(sys, 'platform', 'darwin'), contextlib.redirect_stdout(io.StringIO()):
            PACKAGE.main()
            seal.assert_called_once_with(output)
        return output, calls

    def test_default_package_builds_keeps_and_deploys_preview(self):
        with tempfile.TemporaryDirectory() as folder:
            output, calls = self.package_fixture(Path(folder), True)
            self.assertTrue((output / EXTENSION).is_dir(), 'Default packaging removed Quick Look')
            self.assertTrue(any('MuseScoreQuickLookPreviewExtension' in call for call in calls if call[0] == 'cmake'))
            self.assertTrue(any(any(arg.startswith('-executable=') and 'MuseScoreQuickLookPreviewExtension' in arg for arg in call)
                                for call in calls if call[0] == '/test/macdeployqt'))

    def test_missing_preview_is_rejected_instead_of_silently_shipping(self):
        with tempfile.TemporaryDirectory() as folder, contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                self.package_fixture(Path(folder), False)


if __name__ == '__main__':
    unittest.main()
