#!/bin/bash

# Clean up previous output files
cleanup() {
    rm -f result.diff
}

# Check if a file exists
file_exists() {
    if [ ! -f "$1" ]; then
        echo "File $1 not found!"
        exit 1
    fi
}
