#!/usr/bin/env python3
"""
Expander: inlines all #include directives for our library headers.
Handles both #include <library/...> and #include "library/..." styles.
Used by oj-verify to compile single-file test programs.
"""

import sys
import os
import re

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
LIBRARY_DIR = os.path.dirname(SCRIPT_DIR)

visited = set()


def resolve_include(header, including_file):
    # Try relative to library root
    full_path = os.path.join(LIBRARY_DIR, header)
    if os.path.isfile(full_path):
        return os.path.abspath(full_path)

    # Try relative to the including file's directory
    full_path = os.path.join(os.path.dirname(including_file), header)
    if os.path.isfile(full_path):
        return os.path.abspath(full_path)

    return None


def expand_file(path):
    abs_path = os.path.abspath(path)
    if abs_path in visited:
        return
    visited.add(abs_path)

    include_pattern = re.compile(
        r'^\s*#\s*include\s+[<"](library/.+?)[>"]'
    )

    with open(path) as f:
        for line in f:
            m = include_pattern.match(line)
            if m:
                header = m.group(1)
                resolved = resolve_include(header, abs_path)
                if resolved:
                    expand_file(resolved)
                else:
                    print(line, end='')
            else:
                print(line, end='')


def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <source_file>", file=sys.stderr)
        sys.exit(1)

    source = sys.argv[1]
    if not os.path.isfile(source):
        print(f"File not found: {source}", file=sys.stderr)
        sys.exit(1)

    expand_file(source)


if __name__ == '__main__':
    main()
