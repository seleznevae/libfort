#!/usr/bin/env python3
"""Check that libfort version is consistent across the repository.

Version must be the same in:
  - src/fort.h and lib/fort.h (LIBFORT_*_VERSION, LIBFORT_REVISION, LIBFORT_VERSION_STR)
  - CMakeLists.txt (project(libfort VERSION ...))
  - tests/package_tests/CMakeLists.txt (expected version of installed package)
  - ChangeLog.md (first "## vX.Y.Z" header)
  - git tag, if passed via --tag (e.g. --tag v0.5.3)
"""

import argparse
import os
import re
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))


def read(rel_path):
    with open(os.path.join(ROOT, rel_path), encoding="utf-8") as f:
        return f.read()


def search(rel_path, pattern):
    match = re.search(pattern, read(rel_path), re.MULTILINE)
    if match is None:
        raise RuntimeError("{}: pattern '{}' not found".format(rel_path, pattern))
    return match


def header_versions(rel_path):
    def define(name):
        return search(rel_path, r"^#define\s+{}\s+(\S+)".format(name)).group(1)

    numeric = "{}.{}.{}".format(define("LIBFORT_MAJOR_VERSION"),
                                define("LIBFORT_MINOR_VERSION"),
                                define("LIBFORT_REVISION"))
    version_str = define("LIBFORT_VERSION_STR").strip('"')
    return [
        (rel_path + " (LIBFORT_*_VERSION, LIBFORT_REVISION)", numeric),
        (rel_path + " (LIBFORT_VERSION_STR)", version_str),
    ]


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--tag", help="git tag of the release, e.g. v0.5.3")
    args = parser.parse_args()

    try:
        versions = header_versions("src/fort.h") + header_versions("lib/fort.h")
        versions.append(("CMakeLists.txt (project VERSION)",
                         search("CMakeLists.txt",
                                r"project\(\s*libfort\s+VERSION\s+([0-9.]+)").group(1)))
        versions.append(("tests/package_tests/CMakeLists.txt",
                         search("tests/package_tests/CMakeLists.txt",
                                r'libfort_VERSION\}\s+EQUAL\s+"([0-9.]+)"').group(1)))
        versions.append(("ChangeLog.md (first ## vX.Y.Z header)",
                         search("ChangeLog.md", r"^##\s+v(\S+)").group(1)))
    except RuntimeError as e:
        print("error: {}".format(e))
        return 1

    if args.tag:
        versions.append(("git tag", args.tag[1:] if args.tag.startswith("v") else args.tag))

    expected = versions[0][1]
    width = max(len(name) for name, _ in versions)
    for name, version in versions:
        mark = "ok" if version == expected else "MISMATCH"
        print("{:<{}}  {:<10} {}".format(name, width, version, mark))

    if any(version != expected for _, version in versions):
        print("error: libfort version is inconsistent (expected {} from {})".format(
            expected, versions[0][0]))
        return 1

    print("libfort version {} is consistent".format(expected))
    return 0


if __name__ == "__main__":
    sys.exit(main())
