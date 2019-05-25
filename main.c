#include <git2.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "wapper.h"

// append commit to HEAD
int main_append_commit(bool isBare, const char path[], const char content[]) {

    git_libgit2_init();

    //init repo
    git_repository *repo;
    if(isBare) {
        repo = open_bare_repo(path);
    } else {
        repo = open_repo(path);
    }

    git_oid tree_id, parent_id, commit_id;
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

    //tree op

    //create blob, tree and commit
        //create blob
    git_oid blob_oid;
    create_blob(repo, &blob_oid, content);
        //create tree
    git_oid tree_oid;
    create_tree_with_blob(repo, &tree_oid, &blob_oid, tree, "create_file.txt");
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
                  new_create,
                  "Flooberhaul the whatnots2222333");

//    std::cout << "Hello, World!" << std::endl;
    printf("hello, world\n");
    git_repository_free(repo);

    return 0;
}


int main_init_commit(bool isBare, const char path[]) {

    git_libgit2_init();
    git_repository *repo;

    //init repo
    if(isBare) {
        repo = open_bare_repo(path);
    } else {
        repo = open_repo(path);
    }

    //create blob, tree and commit
        //create blob
    git_oid blob_oid;
    create_blob(repo, &blob_oid, "init commit. Hello there!");
        //create tree
    git_oid tree_oid;
    create_tree_with_blob(repo, &tree_oid, &blob_oid, NULL, "create_file.txt");
        // lookup tree struct
    git_tree *new_tree;
    lookup(repo, &tree_oid,
           NULL,
           &new_tree,
           NULL,
           NULL,
           2);
    //create commit
    git_oid commit_oid;
    create_init_commit(repo,
                  &commit_oid,
                  new_tree,
                  "init commit!");


    printf("init commit completed!\n");
    git_repository_free(repo);

    return 0;
}

// append commit to HEAD
//int main_append_merge_commit(bool isBare, const char path[], const char content[]) {
//
//    git_libgit2_init();
//
//    //init repo
//    git_repository *repo;
//    if(isBare) {
//        repo = open_bare_repo(path);
//    } else {
//        repo = open_repo(path);
//    }
//
//    //create blob, tree and commit
//    //create blob
////    git_oid blob_oid;
////    create_blob(repo, &blob_oid, content);
//    //create tree
////    git_oid tree_oid;
////    create_tree_with_blob(repo, &tree_oid, &blob_oid, tree);
////     lookup tree struct
////    git_tree *new_create;
////    lookup(repo, &tree_oid,
////           NULL,
////           &new_create,
////           NULL,
////           NULL,
////           2);
//    //create commit
//        // find commit1
//    git_oid commit1_oid, commit2_oid;
//    git_commit *git_commit1, *git_commit2;
//    oid_from_str(&commit1_oid, "118e8c507dffbdf6006ecc57a56285fe26f18a28");
//    lookup(repo, &commit1_oid,
//           &git_commit1,
//           NULL,
//           NULL,
//           NULL,
//           1);
//    oid_from_str(&commit2_oid, "d6affc1ce2ed406f718ed7ccc35a6b40c810dc24");
//    lookup(repo, &commit2_oid,
//           &git_commit2,
//           NULL,
//           NULL,
//           NULL,
//           1);
//
//    git_oid commit_oid;
//    create_merge_commit(repo,
//                  &commit_oid,
//                  git_commit1,//HEAD commit
//                  git_commit2,
//                  NULL,
//                  "merge");
//
//
//    std::cout << "Hello, World!" << std::endl;
//    git_repository_free(repo);
//
//    return 0;
//}
//

void main_merge_non_bare_repo_without_conflict(const char path[]) {
    git_libgit2_init();

    //init repo
    git_repository *repo;
    repo = open_repo(path);

    //create commit
    // find commit1
    git_oid commit1_oid, commit2_oid;
    git_commit *git_commit1, *git_commit2;
    oid_from_str(&commit1_oid, "153716842092745ff856c718bea583539833e7d7");
    lookup(repo, &commit1_oid,
           &git_commit1,
           NULL,
           NULL,
           NULL,
           1);
    oid_from_str(&commit2_oid, "76aa17b0076635aee3d93a44ddcaf823de614ea9");
    lookup(repo, &commit2_oid,
           &git_commit2,
           NULL,
           NULL,
           NULL,
           1);

    git_index *index;
    int error = git_merge_commits(&index, repo, git_commit1, git_commit2, NULL);
    int conflict_count = git_index_has_conflicts(index);

    git_oid index_tree_oid, index_commit_oid;
    git_tree *index_tree;
    if(conflict_count == 0) {
        git_index_write_tree_to(&index_tree_oid, index, repo);
        lookup(repo,&index_tree_oid,
                NULL,
                &index_tree,
                NULL,
                NULL,
                2);
        create_merge_commit(repo,
                &index_commit_oid,
                git_commit1, git_commit2,
                index_tree, "merge without conflict~");
//        git_index_write(index);
    } else {
        // solve conflict
        printf("some conflict occurred!");
    }

    error_check(error);
}

void main_merge_bare_repo_without_conflict(const char path[]) {
    git_libgit2_init();

    //init repo
    git_repository *repo;
    repo = open_bare_repo(path);

    //create commit
    // find commit1
    git_oid commit1_oid, commit2_oid;
    git_commit *git_commit1, *git_commit2;
    oid_from_str(&commit1_oid, "153716842092745ff856c718bea583539833e7d7");
    lookup(repo, &commit1_oid,
           &git_commit1,
           NULL,
           NULL,
           NULL,
           1);
    oid_from_str(&commit2_oid, "76aa17b0076635aee3d93a44ddcaf823de614ea9");
    lookup(repo, &commit2_oid,
           &git_commit2,
           NULL,
           NULL,
           NULL,
           1);

    git_index *index;
    int error = git_merge_commits(&index, repo, git_commit1, git_commit2, NULL);
    int conflict_count = git_index_has_conflicts(index);

    git_oid index_tree_oid, index_commit_oid;
    git_tree *index_tree;
    if(conflict_count == 0) {
        git_index_write_tree_to(&index_tree_oid, index, repo);
        lookup(repo,&index_tree_oid,
               NULL,
               &index_tree,
               NULL,
               NULL,
               2);
        create_merge_commit(repo,
                            &index_commit_oid,
                            git_commit1, git_commit2,
                            index_tree, "merge without conflict~");
//        git_index_write(index);
    } else {
        // solve conflict
        printf("some conflict occurred!");
    }

    error_check(error);
}

void main_commit_amend_only_commit_msg() {
    git_libgit2_init();

    //init repo
    git_repository *repo;
    repo = open_repo("/Users/lorancechen/tmp/gitgud_repo/libgit2-test/amend-commit-repo");

//    const char content[] = "new file content";
    const char commit_msg[] = "amend commit message";

    //former commit id
    git_oid now_commit_oid, new_commit_oid;
    git_commit *now_commit;
    oid_from_str(&now_commit_oid, "317ab3c6cf4fb4f2d3c621a6e9c64a29e00a27d1");
    lookup(repo, &now_commit_oid, &now_commit, NULL, NULL, NULL, 1);

    git_tree *now_tree;
    //get tree
    git_commit_tree(&now_tree, now_commit);

    //create amend commit
    git_commit_amend(&new_commit_oid,
            now_commit,
            "HEAD",
            NULL,
            NULL,
            NULL,
            commit_msg,
            now_tree
            );

}

void main_commit_amend() {
    git_libgit2_init();

    //init repo
    git_repository *repo;
    repo = open_repo("/Users/lorancechen/tmp/gitgud_repo/libgit2-test/amend-commit-repo");

    const char content[] = "new file content222";
    const char commit_msg[] = "amend commit message with new file3234455556000";

    //former commit id
    git_oid now_commit_oid, new_commit_oid, new_tree_oid, new_blob_oid;
    git_commit *now_commit;
    oid_from_str(&now_commit_oid, "178d11cc4bd7ab00e2287e9a74edd1fedabc2897");
    lookup(repo, &now_commit_oid, &now_commit, NULL, NULL, NULL, 1);

    //get now tree
    git_tree *now_tree;
    git_commit_tree(&now_tree, now_commit);


    //create new blob
    create_blob(repo, &new_blob_oid, content);

    //create new tree from new blob_oid
    create_tree_with_blob(
            repo,
            &new_tree_oid,
            &new_blob_oid,
            now_tree,
            "file3.txt"
    );

    git_tree *new_create_tree;
    lookup(repo, &new_tree_oid,
           NULL,
           &new_create_tree,
           NULL,
           NULL,
           2);


    //create amend commit
    int error = git_commit_amend(&new_commit_oid,
                     now_commit,
                     "HEAD",
                     NULL,
                     NULL,
                     NULL,
                     commit_msg,
                     new_create_tree
    );
    error_check(error);

}

void main_commit_amend_bare_repo() {
    git_libgit2_init();

    //init repo
    git_repository *repo;
    repo = open_repo("/Users/lorancechen/tmp/gitgud_repo/libgit2-test/amend-commit-repo-bare");

    const char content[] = "new file content222 bare repo";
    const char commit_msg[] = "amend commit message with new file3234ssss455556000";

    //former commit id
    git_oid now_commit_oid, new_commit_oid, new_tree_oid, new_blob_oid;
    git_commit *now_commit;
//    oid_from_str(&now_commit_oid, "4a2d991f4069a3cbb2e32b77633bedfe55c5a180");
    head_oid(&now_commit_oid, repo);

    lookup(repo, &now_commit_oid, &now_commit, NULL, NULL, NULL, 1);

    //get now tree
    git_tree *now_tree;
    git_commit_tree(&now_tree, now_commit);


    //create new blob
    create_blob(repo, &new_blob_oid, content);

    //create new tree from new blob_oid
    create_tree_with_blob(
            repo,
            &new_tree_oid,
            &new_blob_oid,
            now_tree,
            "file3.txt"
    );

    git_tree *new_create_tree;
    lookup(repo, &new_tree_oid,
           NULL,
           &new_create_tree,
           NULL,
           NULL,
           2);


    //create amend commit
    int error = git_commit_amend(&new_commit_oid,
                                 now_commit,
                                 "HEAD",
                                 NULL,
                                 NULL,
                                 NULL,
                                 commit_msg,
                                 new_create_tree
    );
    error_check(error);

}

void main_merge_and_output_conflict() {
    git_libgit2_init();

    //init repo
    git_repository *repo;
    repo = open_repo("/Users/lorancechen/tmp/gitgud_repo/libgit2-test/conflict-repo");
    git_index *idx;
//    git_repository_index(&idx, repo);

    //create commit
    // find commit1
    git_oid commit1_oid, commit2_oid;
    git_commit *git_commit1, *git_commit2;
    oid_from_str(&commit1_oid, "c7a3c215977f518c296f1fd263e5a3d92654c24c");
    lookup(repo, &commit1_oid,
           &git_commit1,
           NULL,
           NULL,
           NULL,
           1);
    oid_from_str(&commit2_oid, "1b2493f5ae8e68d7dfd0f307885f2875cfa49c96");
    lookup(repo, &commit2_oid,
           &git_commit2,
           NULL,
           NULL,
           NULL,
           1);

    git_index *index;
    int error = git_merge_commits(&index, repo, git_commit1, git_commit2, NULL);
    int conflict_count = git_index_has_conflicts(index);

    git_oid index_tree_oid, index_commit_oid;
    git_tree *index_tree;
    if(conflict_count == 0) {
        git_index_write_tree_to(&index_tree_oid, index, repo);
        lookup(repo,&index_tree_oid,
               NULL,
               &index_tree,
               NULL,
               NULL,
               2);
        create_merge_commit(repo,
                            &index_commit_oid,
                            git_commit1, git_commit2,
                            index_tree, "merge without conflict~");
//        git_index_write(index);
    } else {
        // solve conflict
        /* If you know the path of a conflicted file */
        git_repository_index(&idx, repo);


        const git_index_entry *ancestor = NULL,
                *ours = NULL,
                *theirs = NULL;
        error = git_index_conflict_get(
                &ancestor,
                &ours,
                &theirs,
                index,
                "file");
        error_check(error);

        git_blob *ancestor_blob, *ours_blob, *theirs_blob;
        error = git_blob_lookup(&ancestor_blob, repo, &ancestor->id);
        error_check(error);

        error = git_blob_lookup(&ours_blob, repo, &ours->id);
        error_check(error);

        error = git_blob_lookup(&theirs_blob, repo, &theirs->id);
        error_check(error);
        char *content = (char *)git_blob_rawcontent(ancestor_blob);
        char *content_ours = (char *)git_blob_rawcontent(ours_blob);
        char *content_theirs = (char *)git_blob_rawcontent(theirs_blob);
        size_t size = git_blob_rawsize(ancestor_blob);

        printf("content:\n%s", content);
        printf("\ncontent_ours:\n%s", content_ours);
        printf("\ncontent_theirs:\n%s", content_theirs);
        printf("some conflict occurred!");
    }

    error_check(error);

}

void main_merge_repo_with_conflict() {}

void main_merge_bare_repo_with_conflict() {}

void main_create_pathed_file_tree() {}

void main_create_commit_with_path_byindex() {
    git_libgit2_init();

    //init repo
    git_repository *repo;
    repo = open_repo("/Users/lorancechen/tmp/gitgud_repo/libgit2-test/simple-repo");

    git_oid tree_id;

    create_tree_content_with_index(repo, &tree_id, "ab/cd/file.txt", "abcd file.txt");
    git_tree *tree;

    // lookup tree
    lookup(repo, &tree_id,
           NULL,
           &tree,
           NULL,
           NULL,
           2);

    //lookup commit
    git_oid head_id;
    git_commit *commit;
    head_oid(&head_id, repo);

    lookup(repo, &head_id,
           &commit,
           NULL,
           NULL,
           NULL,
           1);

    git_oid new_commit_id;
    create_commit(repo, &new_commit_id, commit, tree, "hello create ab/cd file from index");

}

int main() {
    // test append commit
//    int rst = main_append_commit(true, "/Users/lorancechen/tmp/gitgud_repo/libgit2-test/bare-repo3");
//    int rst = main_append_commit(false,
//            "/Users/lorancechen/tmp/gitgud_repo/libgit2-test/simple-repo",
//            "append commit2");

//    int rst = main_append_merge_commit(false,
//                                 "/Users/lorancechen/tmp/gitgud_repo/libgit2-test/simple-repo",
//                                 "append commit2");

//    merge_non_bare_repo_without_conflict("/Users/lorancechen/tmp/gitgud_repo/libgit2-test/simple-repo");

//    main_merge_bare_repo_without_conflict("/Users/lorancechen/tmp/gitgud_repo/libgit2-test/simple-repo-clone");

//    main_commit_amend_only_commit_msg();

//    main_commit_amend();
//    main_commit_amend_bare_repo();

//    main_commit_with_path();
//    main_create_commit_with_path_byindex();
    main_merge_and_output_conflict();

    //test first commit
//    int rst = main_init_commit(true, "/Users/lorancechen/tmp/gitgud_repo/libgit2-test/bare-repo3");
//    int rst = main_init_commit(false, "/Users/lorancechen/tmp/gitgud_repo/libgit2-test/simple-repo");
    return 0;

}