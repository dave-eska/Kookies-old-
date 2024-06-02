#!/bin/bash

# Check if there are any arguments passed
if [ $# -eq 0 ]; then
    echo "Please provide a commit message"
    exit 1
fi

# Run git commands
git add src/ include/ thirdparty/ build/res/ README.md build/config.json
git status
git commit -m "$*"
