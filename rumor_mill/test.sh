#!/bin/bash
# Can run a single test (pass the test.in file as command line arg)
# or all tests in the specified test directory.

# Provide an interpreter to use (e.g. "java ", "python ", "./", ...)
# ^ Notice how the space is included in "java " or "python "?
INTERPRETER=./

# Provide the path to your program.
PROGRAM=a.out

# Provide the path to your test directory.
TESTS=samples

# Light error checking.
if [ $# -gt 1 ]; then
    echo "Usage: $0 [path-to-test.in]"
    exit 1
fi

# Runs a single test.
run_test () {
    OUT=$($1$2 < $3)
    EXP=$(cat "${3%.*}.ans")
    if [ "$OUT" == "$EXP" ]; then
        echo -e "Test $3 \x1b[032mPASSED\x1b[0m."
    else
        echo -e "Test $3 \x1b[031mFAILED\x1b[0m.\nGiven:\n$OUT\nExpected:\n$EXP"
    fi
}

# Single test mode.
if [ $# -eq 1 ]; then
    run_test $INTERPRETER $PROGRAM $1
    exit 0
fi

# Test directory mode.
shopt -s nullglob
for TEST in $TESTS/*.in; do
    run_test $INTERPRETER $PROGRAM $TEST
done
