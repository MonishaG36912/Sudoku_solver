#!/bin/bash

echo "Running test6..."

TEMP_INPUT=$(mktemp)
echo "2" > "$TEMP_INPUT"
echo "./test_files/input6.txt" >> "$TEMP_INPUT"

./sudoku_solver < "$TEMP_INPUT" > test6_output.txt

EXPECTED="Error: Invalid placement of value 6"
grep -q "$EXPECTED" test6_output.txt

if [ $? -eq 0 ]; then
    echo "✅ Test 6 passed! Detected invalid placement of 6."
else
    echo "❌ Test 6 failed! Expected error not found."
    cat test6_output.txt
fi

rm -f "$TEMP_INPUT" test6_output.txt
