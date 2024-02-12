# bash

echo BUILD_DIR $BUILD_DIR >buildflags.log
echo PROJECT_DIR $PROJECT_DIR >>buildflags.log
echo PWD $(pwd)  >>buildflags.log
echo COMMIT_HASH $(git log -1 --format=%h) >>buildflags.log
echo BRANCH $(git branch --show-current) >>buildflags.log
echo >>buildflags.log
printenv >>buildflags.log

echo \'-D BRANCH=\"$(git branch --show-current)\"\'
echo \'-D COMMIT_HASH=\"$(git log -1 --format=%h)\"\'
echo \'-D PROJECT_URL=\"$(git config remote.origin.url)\"\'
echo \'-D PROJECT_DIR=\"$(pwd)\"\'
TIMESTAMP=$(date +"%Y-%m-%d %H:%M")
echo \'-D TIMESTAMP=\"$TIMESTAMP\"\'