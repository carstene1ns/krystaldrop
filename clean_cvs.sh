#!/bin/sh

# remove CVSROOT and .cvsignore everywhere

git filter-branch --force --tree-filter '
rm -rf CVSROOT
find . -name ".cvsignore" -delete
' --prune-empty -- --all