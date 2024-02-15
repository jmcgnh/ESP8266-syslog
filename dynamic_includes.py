# Bing Co-pilot produced a python script by translating the following bash script
# it looks like it works, but I bet it could be better by using python-git
# direct calls rather than redirection through shell commands
#

## bash
## echo '#define BRANCH "'$(git branch --show-current)\"  >include/gitstatus.log
## echo '#define COMMIT_HASH "'$(git log -1 --format=%h)\" >>include/gitstatus.log
## echo '#define LAST_COMMIT_COMMENT "'$(git show --format=%s -s)\" >>include/gitstatus.log
## echo '#define PROJECT_URL "'$(git config remote.origin.url)\" >>include/gitstatus.log
## if [  ! -e include/gitstatus.h ] || [ ! $(diff -q include/gitstatus.h include/gitstatus.log >/dev/null) ]; then
##     mv include/gitstatus.log include/gitstatus.h
## fi
## 
## echo '#define PROJECT_DIR "'$(pwd)\" > include/buildstatus.h
## TIMESTAMP=$(date +"%Y-%m-%d %H:%M")
## echo '#define TIMESTAMP "'$TIMESTAMP\" >>include/buildstatus.h


# Bing Copilot made some improvements after I asked it to use GitPython
# I did the needed 'pip install GitPython' step already


import os
import datetime
from git import Repo
from pathlib import Path

# use current dir (.) as repo
repo = Repo(".")

# Get the current branch
branch = repo.active_branch.name

# Get the latest commit hash
commit_hash = repo.head.commit.hexsha

# Get the last commit comment
last_commit_comment = repo.head.commit.message.strip()

# Get the project URL
project_url = repo.remotes.origin.url

# Write to gitstatus.log
with open("include/gitstatus.log", "w") as gitstatus_file:
    gitstatus_file.write(f'#define BRANCH "{branch}"\n')
    gitstatus_file.write(f'#define COMMIT_HASH "{commit_hash}"\n')
    gitstatus_file.write(f'#define LAST_COMMIT_COMMENT "{last_commit_comment}"\n')
    gitstatus_file.write(f'#define PROJECT_URL "{project_url}"\n')

# Check if gitstatus.h exists or if contents differ
if not os.path.exists("include/gitstatus.h"):
    os.rename("include/gitstatus.log", "include/gitstatus.h")
else:
    with open("include/gitstatus.log", "r") as log_file:
        log_contents = log_file.read()
    with open("include/gitstatus.h", "r") as header_file:
        header_contents = header_file.read()
    if log_contents != header_contents:
        os.replace("include/gitstatus.log", "include/gitstatus.h")

# Write to buildstatus.h
with open("include/buildstatus.h", "w") as buildstatus_file:
    path = Path(os.getcwd())
    buildstatus_file.write(f'#define PROJECT_DIR "{path.as_posix()}"\n')
    timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M")
    buildstatus_file.write(f'#define TIMESTAMP "{timestamp}"\n')
