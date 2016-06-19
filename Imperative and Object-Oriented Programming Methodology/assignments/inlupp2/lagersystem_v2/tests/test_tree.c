#include <stdio.h>

#include "../btree.h"




void tree_insert_gprof(tree_t *tree){
  char *data = NULL;
  char buf[10];
  for(int i = 0; i<100000; ++i){
    sprintf(buf, "%d", i);
    char *key = buf;
    tree_insert(tree,key,data);
  }
}

void tree_remove_gprof(tree_t *tree){
  char buf[10];
  for(int i = 0; i<100000; ++i){
    sprintf(buf, "%d", i);
    char *key = buf;
    tree_remove(tree,key);
  }
}

void tree_remove_gprof2(tree_t *tree){
  char buf[10];
  for(int i = 100000; i>=0; --i){
    sprintf(buf, "%d", i);
    char *key = buf;
    tree_remove(tree,key);
  }
}

void tree_find_gprof(tree_t *tree){
  char buf[10];
  for(int i = 50000; i < 100000; ++i){
    sprintf(buf, "%d", i);
    char *key = buf;
    tree_find(tree,key);
  }
}

int main(int argc, char *argv[]){
  tree_t * tree = tree_new();
  tree_insert_gprof(tree);
  tree_print_in_order(tree);
  tree_find_gprof(tree);
  tree_remove_gprof2(tree);
  tree_print_in_order(tree);


  return 0;
}
