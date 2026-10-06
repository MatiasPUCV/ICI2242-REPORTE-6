"""Runs the program on an input and compares its output with the expected one.

Usage: run_cli.py <executable> <input.in> <expected.out>

Both ways of using the program are tested: standard input and file path.
"""
import subprocess
import sys
from pathlib import Path


def run_program(executable, input_path, use_file_argument):
    if use_file_argument:
        return subprocess.run([executable, input_path], capture_output=True, text=True)
    with open(input_path) as stdin:
        return subprocess.run([executable], stdin=stdin, capture_output=True, text=True)


def main():
    executable, input_path, expected_path = sys.argv[1:4]
    expected = Path(expected_path).read_text().strip()

    for use_file_argument in (False, True):
        mode = "file" if use_file_argument else "stdin"
        result = run_program(executable, input_path, use_file_argument)
        actual = result.stdout.strip()
        if result.returncode != 0 or actual != expected:
            print(f"FAILED ({mode}): expected {expected!r}, got {actual!r}")
            print(result.stderr)
            return 1
        print(f"OK ({mode}): {Path(input_path).name} -> {actual}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
