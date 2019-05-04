#include <iostream>
#include <git2.h>

void error_check(int error) {
    if (error < 0) {
        const git_error *e = giterr_last();
        printf("Error %d/%d: %s\n", error, e->klass, e->message);
        exit(error);
    }
}

git_repository* open_bare_repo() {
    //init repo
    git_repository *repo = NULL;
    //open bare
    int error = git_repository_open_bare(&repo, "/Users/lorancechen/tmp/gitgud_repo/test_repo");

    error_check(error);

    return repo;

}

git_repository* open_repo() {
    //init repo
    git_repository *repo = NULL;
    //open bare
//    int error = git_repository_open(&repo, "/Users/lorancechen/tmp/gitgud_repo/test_repo");
    int error = git_repository_open(&repo, "/Users/lorancechen/tmp/gitgud_repo/test_repo");

    if (error < 0) {
        const git_error *e = giterr_last();
        printf("Error %d/%d: %s\n", error, e->klass, e->message);
        exit(error);
    }

    return repo;

}

void oid_test(git_oid *oid) {
    //oid
    /* Convert a SHA to an OID */
    const char *sha = "5fb923908cbb468d094c1ece787e2b4a5782852f";
    int errorOid = git_oid_fromstr(oid, sha);

    /* Make a shortened printable string from an OID */
    char shortsha[10] = {0};
    git_oid_tostr(shortsha, 9, oid);

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

void create_blob(git_repository *repo, git_oid *blob_oid) {
//    int error = git_blob_create_fromworkdir(&oid, repo, "README.md");
//    error = git_blob_create_fromdisk(&oid, repo, "/etc/hosts");

    const char str[] = "# Hello there!";
    int error = git_blob_create_frombuffer(blob_oid, repo, str, strlen(str));
    error_check(error);

    return;
}

void create_tree_with_blob(git_repository *repo, git_oid *out_tree_id, const git_oid *blob_oid) {
    git_treebuilder *bld = NULL;
    int error = git_treebuilder_new(&bld, repo, NULL);

/* Add some entries */
    git_object *obj = NULL;
//    error = git_revparse_single(&obj, repo, "HEAD:README.md");
    error = git_treebuilder_insert(NULL, bld,
                                   "create_by_code.md",        /* filename */
                                   blob_oid, /* OID */
                                   GIT_FILEMODE_BLOB); /* mode */
//    git_object_free(obj);
//    error = git_revparse_single(&obj, repo, "v0.1.0:foo/bar/baz.c");
//    error = git_treebuilder_insert(NULL, bld,
//                                   "d.c",
//                                   git_object_id(obj),
//                                   GIT_FILEMODE_BLOB);
//    git_object_free(obj);

    error = git_treebuilder_write(out_tree_id, bld);

    git_treebuilder_free(bld);
}

void lookup_commit_tree(git_commit* commit, git_tree *tree) {
    int error = git_commit_tree(&tree, commit);
    error_check(error);
    return;
}
void create_commit(git_repository *repo,
                   git_oid *out_new_commit_id,
                    const git_commit *parent,
                    const git_tree *tree) {
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
            "Flooberhaul the whatnots2222",  /* message */
            tree,                        /* root tree */
            1,                           /* parent count */
            parent);                    /* parents */
    error_check(error);

    return;
}

int main() {

    git_libgit2_init();

    //init repo
//    git_repository *repo = NULL;
//    int error = git_repository_init(&repo, "/Users/lorancechen/tmp/gitgud_repo/c-test", true);

    git_repository *repo = open_repo();

    //open repo
//    int error = git_repository_open(&repo, "/Users/lorancechen/tmp/gitgud_repo/test_repo");
//    if (error < 0) {
//        const git_error *e = giterr_last();
//        printf("Error %d/%d: %s\n", error, e->klass, e->message);
//        exit(error);
//    }
    git_oid tree_id, parent_id, commit_id;
//    git_oid oid;
//    oid_test(&oid);
    head_oid(&parent_id, repo);
    git_commit *commit;
    git_blob *blob;
    git_tree *tree;
    git_tag *tag;

    // lookup commit
    lookup(repo, &parent_id,
            &commit,
            NULL,
            NULL,
            NULL,
            1);
    //get tree of the commit
//    lookup_commit_tree(commit, tree);
    git_index *idx = NULL;
    int error = git_repository_index(&idx, repo);
    error_check(error);

    error = git_index_write_tree(&tree_id, idx);
    error_check(error);

    // lookup tree
    lookup(repo, &tree_id,
           NULL,
           &tree,
           NULL,
           NULL,
           2);

    //create a commit
//    create_commit(repo, &commit_id, commit, tree);

    //create blob, tree and commit
        //create blob
    git_oid blob_oid;
    create_blob(repo, &blob_oid);
        //create tree
    git_oid tree_oid;
    create_tree_with_blob(repo, &tree_oid, &blob_oid);
        // lookup tree struct
    git_tree *new_create;
    lookup(repo, &tree_oid,
           NULL,
           &new_create,
           NULL,
           NULL,
           2);
        //create commit
    git_oid commit_oid;
    create_commit(repo,
            &commit_oid,
            commit,//HEAD commit
            new_create);

    std::cout << "Hello, World!" << std::endl;
    git_repository_free(repo);

    return 0;
}
