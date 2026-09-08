#!/usr/bin/env python3
"""Compile the actual policy against CMake-generated downstream/upstream settings."""
import os
from pathlib import Path
import subprocess
import tempfile


def main():
    root = Path(__file__).resolve().parents[2]
    with tempfile.TemporaryDirectory(prefix='melo-product-policy-') as directory:
        temp = Path(directory)
        (temp / 'CMakeLists.txt').write_text('''cmake_minimum_required(VERSION 3.16)
project(ProductPolicyProbe LANGUAGES NONE)
if(INCLUDE_IDENTITY)
    include("${SCORE_SOURCE}/buildscripts/melopresto_identity.cmake")
endif()
configure_file("${SCORE_SOURCE}/src/framework/cmake/muse_framework_config.h.in"
               "${CMAKE_BINARY_DIR}/muse_framework_config.h")
''')
        for name, identity, extra, expected in (
            ('melopresto-default', True, [], False),
            ('explicit-upstream-restoration', True, ['-DMELO_SCORE_SUPPRESS_UPSTREAM_PROMOTIONS=OFF'], True),
            ('upstream-without-product-identity', False, [], True),
        ):
            build = temp / name
            subprocess.run(['cmake', '-S', str(temp), '-B', str(build),
                            '-DSCORE_SOURCE=' + str(root), '-DINCLUDE_IDENTITY=' + ('ON' if identity else 'OFF'),
                            *extra], check=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            source = build / 'probe.cpp'
            source.write_text('#include "productpolicy.h"\n'
                              'static_assert(muse::productPromotionsEnabled() == '
                              + str(expected).lower() + ');\nint main() {}\n')
            subprocess.run([os.environ.get('CXX', 'c++'), '-std=c++17', '-I' + str(build),
                            '-I' + str(root / 'src/framework/global'), str(source), '-o', str(build / 'probe')], check=True)
            subprocess.run([str(build / 'probe')], check=True)
            print('PASS:', name)


if __name__ == '__main__':
    main()
