# libgit2 travel
test libgit2 library use case mainly for bare repository.

## Features
- create first commit
- create a commit to exist HEAD commit
- merge (non-fast forward) two commits in non-conflict situation
- commit amend only commit message & amend commit message and tree
- ...

## Todo
- create tag
- create branch
- create a commit in a branch, such as branch-tmp
    - create a new commit
    - verify branch-tmp ref to the new commit
- merge / fast forward
- get list history content of a file
- get differ of two version

## resource
- [merge with temporary index](https://stackoverflow.com/questions/28246887/create-a-tree-object-from-a-temporary-index-using-libgit2-git2go)
- [refs and branch](https://ben.straub.cc/2013/06/03/refs-tags-and-branching/)
