#!/bin/bash

echo "Running test4..."

TEMP_INPUT=$(mktemp)
echo "2" > "$TEMP_INPUT"
echo "./test_files/input4.txt" >> "$TEMP_INPUT"

./sudoku_solver < "$TEMP_INPUT" > test4_output.txt

EXPECTED="Error: Invalid placement of value 1"
grep -q "$EXPECTED" test4_output.txt

if [ $? -eq 0 ]; then
    echo "✅ Test 4 passed! Detected invalid value correctly."
else
    echo "❌ Test 4 failed! Expected error not found."
    cat test4_output.txt
fi

rm -f "$TEMP_INPUT" test4_output.txt
