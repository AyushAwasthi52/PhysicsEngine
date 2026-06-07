#!/bin/bash

# Check if commit message is provided
if [ $# -eq 0 ]; then
    echo "Usage: ./push.sh \"commit message\""
    exit 1
fi

COMMIT_MSG="$1"

git add .
git commit -m "$COMMIT_MSG"

# Stop if commit failed
if [ $? -ne 0 ]; then
    echo "Commit failed."
    exit 1
fi

git push

echo "Successfully pushed to GitHub!"