//
// Created by LoranceChen on 2019-05-06.
//

#ifndef LIBGIT_TEST_CORE_H
#define LIBGIT_TEST_CORE_H

#include <git2/types.h>

typedef struct core_commit{
    char *parent_commit;
    char *path;
    char *content;

} core_commit;

extern void core_create_commit(git_repository *repo,  core_commit *commit);


#endif //LIBGIT_TEST_CORE_H
