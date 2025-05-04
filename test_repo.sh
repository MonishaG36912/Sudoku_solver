#!/bin/bash

# Loop through all test scripts and execute them
for test_script in ./test_script/*.sh; do
    echo "Running $test_script"
    bash "$test_script"
    echo "====================================="
done
