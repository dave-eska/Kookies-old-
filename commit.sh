#!/bin/bash

# Check if there are any arguments passed
if [ $# -eq 0 ]; then
    echo "Please provide a commit message"
    exit 1
fi

# Run git commands
git add src/ include/ thirdparty/ build/res
git status
git commit -m "$*"
