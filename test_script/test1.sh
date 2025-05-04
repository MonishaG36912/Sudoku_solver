#!/bin/bash

# Load utility functions if needed
source ./test_script/utils.sh

# Rebuild the solver if needed
make > /dev/null

# Temporary input to feed "2" and filename
TEMP_INPUT=$(mktemp)
echo "2" > "$TEMP_INPUT"
echo "./test_files/input1.txt" >> "$TEMP_INPUT"

# Run solver and capture full output
./sudoku_solver < "$TEMP_INPUT" > actual_output.txt

# Extract just the numbers from the solved puzzle
awk '
/Solved Sudoku Puzzle:/ {found=1; next}
found && NF && $0 !~ /^\+\+/ {
    gsub("[|+]", "")
    gsub(/  +/, " ")
    print $0
}
' actual_output.txt | sed -E 's/^ +| +$//g' > cleaned_output.txt

# Normalize expected answer by collapsing extra spaces and stripping leading/trailing spaces
sed -E 's/  +/ /g; s/^ +| +$//g' ./test_files/answer1.txt > cleaned_answer.txt

# Compare results
diff cleaned_output.txt cleaned_answer.txt > result.diff

if [ $? -eq 0 ]; then
    echo "✅ Test passed!"
else
    echo "❌ Test failed! Differences:"
    cat result.diff
fi

# Clean up
rm -f "$TEMP_INPUT" actual_output.txt cleaned_output.txt cleaned_answer.txt result.diff
