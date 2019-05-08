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

//    core_commit commit1;
//    commit1.content = "12";
//    core_create_commit(repo, &commit1);

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

    //create a commit
//    create_commit(repo, &commit_id, commit, tree);

    //tree op

    //create blob, tree and commit
        //create blob
    git_oid blob_oid;
    create_blob(repo, &blob_oid, content);
        //create tree
    git_oid tree_oid;
    create_tree_with_blob(repo, &tree_oid, &blob_oid, tree);
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
    create_tree_with_blob(repo, &tree_oid, &blob_oid, NULL);
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

    main_merge_bare_repo_without_conflict("/Users/lorancechen/tmp/gitgud_repo/libgit2-test/simple-repo-clone");

    //test first commit
//    int rst = main_init_commit(true, "/Users/lorancechen/tmp/gitgud_repo/libgit2-test/bare-repo3");
//    int rst = main_init_commit(false, "/Users/lorancechen/tmp/gitgud_repo/libgit2-test/simple-repo");
    return 0;

}