# libgit2 travel
test libgit2 library use case mainly for bare repository.

## Features
- create first commit
- create a commit to exist HEAD commit
- merge (non-fast forward) two commits in non-conflict situation
- commit amend only commit message
- amend commit message and tree
- create a file in a directory path
- ...

## Todo
- create tag
- create branch
- create a commit in a branch, such as branch-tmp
    - create a new commit
    - verify branch-tmp ref to the new commit
- merge with fast forward
- get list history content of a file
    - get commit id of the file (git shell)
        - `git --no-pager log --pretty=format:"%h" ./file3.txt`
    - get file content of a commit id
        - `git --no-pager show 4a2d991:file3.txt --pretty=format:"%h" --no-patch`
- get differ of file's two version
- show and solve conflict
- concurrency safe: insure index( such as HEAD) NO concurrent problem.
    - git http push to master(HEAD) and web save to master(HEAD)
    - multiple web client save HEAD
    - see `transaction.h`

## resource
- [merge with temporary index](https://stackoverflow.com/questions/28246887/create-a-tree-object-from-a-temporary-index-using-libgit2-git2go)
- [refs and branch](https://ben.straub.cc/2013/06/03/refs-tags-and-branching/)
