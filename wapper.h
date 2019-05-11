//
// Created by LoranceChen on 2019-05-08.
//

#ifndef LIBGIT_TEST_WAPPER_H
#define LIBGIT_TEST_WAPPER_H
#include <git2.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
void error_check(int error) {
    if (error < 0) {
        const git_error *e = giterr_last();
        printf("Error %d/%d: %s\n", error, e->klass, e->message);
        exit(error);
    }
}

git_repository* open_bare_repo(const char path[]) {
    //init repo
    git_repository *repo = NULL;
    //open bare
    int error = git_repository_open_bare(&repo, path);

    error_check(error);

    return repo;

}

git_repository* open_repo(const char path[]) {
    //init repo
    git_repository *repo = NULL;
    //open bare
    int error = git_repository_open(&repo, path);

    error_check(error);

    return repo;

}

void oid_from_str(git_oid *oid, const char sha[]) {
    //oid
    /* Convert a SHA to an OID */
    int error = git_oid_fromstr(oid, sha);
    error_check(error);
    /* Make a shortened printable string from an OID */
//    char shortsha[10] = {0};
//    git_oid_tostr(shortsha, 9, oid);

    return;
}

void head_oid(git_oid *head_oid, git_repository * repo) {
    int error = git_reference_name_to_id(head_oid, repo, "HEAD");
    error_check(error);
    return;
}

void lookup(git_repository* repo, git_oid *oid,
            git_commit** commit,
            git_tree** tree,
            git_blob** blob,
            git_tag** tag,
            int type//1: commit, 2: tree, 3: blob, 4: tag
) {
    int error;
    if(type == 1) {
        error = git_commit_lookup(commit, repo, oid);
        error_check(error);
    } else if(type == 2) {
        error = git_tree_lookup(tree, repo, oid);
        error_check(error);
    } else if(type == 3) {
        error = git_blob_lookup(blob, repo, oid);
        error_check(error);
    } else if(type == 4) {
        error = git_tag_lookup(tag, repo, oid);
        error_check(error);
    }

    return;
}

void create_blob(git_repository *repo, git_oid *blob_oid, const char str[]) {
//    int error = git_blob_create_fromworkdir(&oid, repo, "README.md");
//    error = git_blob_create_fromdisk(&oid, repo, "/etc/hosts");

//    const char str[] = "# Hello there!";
    int error = git_blob_create_frombuffer(blob_oid, repo, str, strlen(str));
    error_check(error);

    return;
}

void create_tree_with_blob(git_repository *repo, git_oid *out_tree_id, const git_oid *blob_oid, const git_tree *source_tree, const char file[]) {
    git_treebuilder *bld = NULL;
    int error;
    if(source_tree != NULL) {
        error = git_treebuilder_new(&bld, repo, source_tree);
    } else {
        error = git_treebuilder_new(&bld, repo, NULL);
    }
    error_check(error);

/* Add some entries */
//    git_object *obj = NULL;
//    error = git_revparse_single(&obj, repo, "HEAD:README.md");
    error = git_treebuilder_insert(NULL, bld,
                                   file,        /* filename */
                                   blob_oid, /* OID */
                                   GIT_FILEMODE_BLOB); /* mode */
    error_check(error);
//    git_object_free(obj);
//    error = git_revparse_single(&obj, repo, "v0.1.0:foo/bar/baz.c");
//    error = git_treebuilder_insert(NULL, bld,
//                                   "d.c",
//                                   git_object_id(obj),
//                                   GIT_FILEMODE_BLOB);
//    git_object_free(obj);

    error = git_treebuilder_write(out_tree_id, bld);
    error_check(error);

    git_treebuilder_free(bld);
}


void create_tree_with_custom_index(git_repository *repo, git_index *index, git_oid *out_tree_id) {
    git_index_entry entry = {{0}};
    entry.mode = GIT_FILEMODE_BLOB;
    entry.path = "a/b/c/d/file.txt";
    char *content = "123 content2222.";
    int error = git_index_add_frombuffer(
            index,
            &entry,
            content,
            strlen(content)
    );
    error_check(error);

    git_index_write(index);
    error_check(error);

    git_index_write_tree(out_tree_id, index);
    error_check(error);
}

void lookup_commit_tree(git_commit* commit, git_tree *tree) {
    int error = git_commit_tree(&tree, commit);
    error_check(error);
    return;
}

void create_init_commit(git_repository *repo,
                        git_oid *out_new_commit_id,
                        const git_tree *tree,
                        const char message[]) {

    // commit repo
    git_signature *me = NULL;
    int error = git_signature_now(&me, "Me", "me@example.com");
    error_check(error);

    error = git_commit_create_v(
            out_new_commit_id,
            repo,
            "HEAD",                      /* name of ref to update */
            me,                          /* author */
            me,                          /* committer */
            "UTF-8",                     /* message encoding */
            message,  /* message */
            tree,                        /* root tree */
            0,                           /* parent count */
            NULL);                    /* parents */
    error_check(error);

    return;
}
void create_commit(git_repository *repo,
                   git_oid *out_new_commit_id,
                   const git_commit *parent,
                   const git_tree *tree,
                   const char message[]) {
    // commit repo
    git_signature *me = NULL;
    int error = git_signature_now(&me, "Me", "me@example.com");
    error_check(error);

    error = git_commit_create_v(
            out_new_commit_id,
            repo,
            "HEAD",                      /* name of ref to update */
            me,                          /* author */
            me,                          /* committer */
            "UTF-8",                     /* message encoding */
            message,  /* message */
            tree,                        /* root tree */
            1,                           /* parent count */
            parent);                    /* parents */
    error_check(error);

    return;
}

void create_merge_commit(git_repository *repo,
                         git_oid *out_new_commit_id,
                         const git_commit *parent1,
                         const git_commit *parent2,
                         const git_tree *tree,
                         const char message[]) {
    // commit repo
    git_signature *me = NULL;
    int error = git_signature_now(&me, "Me", "me@example.com");
    error_check(error);

    error = git_commit_create_v(
            out_new_commit_id,
            repo,
            "HEAD",                      /* name of ref to update */
            me,                          /* author */
            me,                          /* committer */
            "UTF-8",                     /* message encoding */
            message,  /* message */
            tree,                        /* root tree */
            2,                           /* parent count */
            parent1, parent2);                    /* parents */
    error_check(error);

    return;
}

void create_commit_with_amend() {

}

#endif //LIBGIT_TEST_WAPPER_H
