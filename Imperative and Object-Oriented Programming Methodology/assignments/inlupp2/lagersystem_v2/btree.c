#include "btree.h"
#include <stdio.h>


//////////////////////////////// Datatypes /////////////////////////////////
typedef struct node node_t;

  struct node {
  char *key;//name
  void *data;//product
  node_t *parent;//parent
  node_t *left;
  node_t *right;
};


Struct tree{
  struct node* root;
  unsigned int size;
};



/////////////////// Hidden (internal) function prototypes //////////////////


int _tree_depthAux(node_t *node);

bool _tree_removeAux(node_t *node,tree_t *tree);
void _tree_updateInParent(node_t *parent, node_t *child, char *key);
void _tree_updateNodeInfo(node_t *to ,node_t *from);
node_t *_tree_getNext(node_t *node);
bool _tree_getAllAux(node_t *tree, list_t *list);


bool _tree_insertAux(node_t * node, char *key,void *data);
node_t *_tree_insertInNode(char *key, void *data);

void *_tree_findAux(node_t *node, char *key);

int _tree_printInOrderAux(node_t *node, int count);
bool _tree_destroyAux(node_t *node);

char *strdupk(const char *string);
char *strdup(const char *str);


///////////////////////// Function Implementations /////////////////////////

// Create Tree or Node
tree_t *tree_new(){
  tree_t *tree = calloc(1,sizeof(tree_t));
  return tree;
}

node_t * node_new(){
  node_t *node = calloc(1,sizeof(node_t));
  return node;
}


// SIZE and DEPTH


int tree_size(tree_t *tree){
  if(tree == NULL){
    return 0;
  }
  return tree->size;
}

int tree_depth(tree_t *tree){
  if(tree == NULL){
    return 0; 
  }
  return _tree_depthAux(tree->root);
}

int _tree_depthAux(node_t *node){
  if(node == NULL){
    return 0;
  }
  int left_depth = _tree_depthAux(node->left);
  int right_depth = _tree_depthAux(node->right);
  if(left_depth<right_depth){
    return right_depth+1;
  }
  return left_depth+1;
}




// INSERT

node_t *_tree_insertInNode(char *key, void *data){
  node_t *node = node_new();
  node->key = strdup(key);
  node->data = data;
  return node;
}


bool tree_insert(tree_t *tree, char * key, void *data){
  if(tree == NULL){
    return false;
  }
  if(tree->root == NULL){
    tree->root = _tree_insertInNode(key,data);
    ++tree->size;
    return true;
  }
  if(tree_find(tree,key)){
    return false;
  }
  ++tree->size;
  return _tree_insertAux(tree->root,key,data);

}

bool _tree_insertAux(node_t * node, char *key,void *data){
  if(node == NULL){
    _tree_insertInNode(key,data);
    return true;
    }
  if(strcasecmp(key,node->key) < 0){
    if(node->left == NULL){
      node_t *new = _tree_insertInNode(key, data);
      new->parent = node;
      node->left = new;
      return true;
    }
    return _tree_insertAux(node->left,key,data);
  }
  else if(strcasecmp(key,node->key) > 0){
    if(node->right == NULL){
      node_t *new = _tree_insertInNode(key, data);
      new->parent = node;
      node->right = new;
      return true;
    }
    return _tree_insertAux(node->right,key,data);
  }
  else{
    return false; 
  }
}



// REMOVE

bool tree_remove(tree_t *tree, char *key){
  if(tree == NULL){
    return false;
  }
  node_t *remove = tree_find(tree,key);
  if(remove == NULL){
    return false;
  }
  --tree->size;
  return _tree_removeAux(remove,tree);
}


bool _tree_removeAux(node_t *remove,tree_t *tree){
  
  if(remove->left != NULL && remove->right != NULL){//Child in both nodes
    node_t *replacement = _tree_getNext(remove->right);//Finds replacing node
    
    if(replacement->right == NULL){//replacing node without right child
      _tree_updateInParent(replacement->parent, replacement->right, replacement->key);
      }  
    else{//replacing node with right child
      _tree_updateInParent(replacement->parent, replacement->right, replacement->key);
      replacement->right->parent = replacement->parent;
    }
    _tree_updateNodeInfo(remove,replacement);
    free(replacement);
    return true;
  }

  else if(remove->left != NULL){//Child in left node
    if(remove->parent != NULL){
      _tree_updateInParent(remove->parent, remove->left, remove->key);
      remove->left->parent = remove->parent;
    }
    else{//Root and Child in left node
      tree->root = remove->left;
      tree->root->parent = NULL;
    }
  }

  else if(remove->right != NULL){//Child in right node
    if(remove->parent != NULL){
      _tree_updateInParent(remove->parent, remove->right, remove->key);
      remove->right->parent = remove->parent;
    }
    else{//Root and Child in right node
      tree->root = remove->right;
      tree->root->parent = NULL;
    }
  }

  else {// No child
    if(remove->parent != NULL){
      _tree_updateInParent(remove->parent, NULL, remove->key);
    }
    else{//No child and root
      tree->root = NULL;
    }
  }
  
  free(remove->key);
  free(remove);
  return true;
}



// If a node will be removed, 
// updates and links the parent and child of the node that will be removed
void _tree_updateInParent(node_t *parent, node_t *child, char *key){
  if(strcasecmp(key, parent->key) < 0){
    parent->left = child;
  }
  else{
     parent->right = child;
  }
}

//Updates key and data from one node to another
void _tree_updateNodeInfo(node_t *to ,node_t *from){
  free(to->key);
  to->key = from->key;
  to->data = from->data; 
}


// finds the next element in "line", to replace with
 node_t *_tree_getNext(node_t *node){
   if(node->left){
     return _tree_getNext(node->left);
   }
   else{
     return node;
   }
 }



void *tree_findData(tree_t *tree, char *key) {
  node_t *n = (node_t*) tree_find(tree, key);
  if(n != NULL) {
    return n->data;
  }
  return NULL;
}

void *tree_find(tree_t *tree, char *key){
  if(tree == NULL){
    return NULL;
  }
  return _tree_findAux(tree->root,key);
}

void *_tree_findAux(node_t *node, char *key){
  if(node == NULL){
    return NULL;
  }
  if(strcasecmp(key ,node->key) == 0){
    return node;
  }
  else if(strcasecmp(key ,node->key) < 0){
    return _tree_findAux(node->left,key);
  }
  else{
    return _tree_findAux(node->right,key);
  }
}


void tree_printInOrder(tree_t *tree){
  int count = 1;
  if (tree->root == NULL){
    puts("EMPTY TREE");
  }
  count =_tree_printInOrderAux(tree->root, count);
}
int _tree_printInOrderAux(node_t *node, int count){
  if(node == NULL){
    return 0; 
  }
  if(node->left){
    count = _tree_printInOrderAux(node->left,count);
  }
  printf("%d: %s\n",count ,node->key);
  ++count;
  if(node->right){
    count = _tree_printInOrderAux(node->right,count);
  }
  return count; 
}



//
list_t *tree_getAll(tree_t *tree){
  list_t* list = list_new();
  _tree_getAllAux(tree->root,list);
  return list;
}

bool _tree_getAllAux(node_t *node,list_t *list){
  if(node == NULL){
    return false; 
  }
  _tree_getAllAux(node->left,list);
  list_append(list, node->data);
  _tree_getAllAux(node->right,list);
  return true; 
}




void tree_destroy(tree_t *tree){
  _tree_destroyAux(tree->root);
  free(tree);
}
bool _tree_destroyAux(node_t *node){
  if(node == NULL){
    return true;
  }
  _tree_destroyAux(node->left);
  _tree_destroyAux(node->right);
  free(node->key);
  free(node);
  return true;
}

//Only used for tests
void tree_test() {
  puts("\n tree test");

  tree_t *t = tree_new();
  
  int x = 0;
  int *ptr = &x;
  char *c1 = "k";
  char *c2 = "j"; 
  char *c3 = "f"; 
  char *c4 = "e";
  //char *c99 = "q";
  char *c5 = "i";
  char *c6 = "g";
  char *c7 = "l";
  char *c8 = "m";
  char *c9 = "h";

  
  tree_insert(t, c1 , ptr);
  tree_insert(t, c2,ptr);
  tree_insert(t, c3,ptr);
  tree_insert(t, c4,ptr);
  tree_insert(t, c5,ptr);
  //tree_insert(t, c99,ptr);
  tree_insert(t, c6,ptr);
  tree_insert(t, c7,ptr);
  tree_insert(t, c8,ptr);
  tree_insert(t, c9,ptr);

  tree_printInOrder(t);


  printf("New root->key : %s\n", t->root->key);
  tree_remove(t, "k");
  printf("borde vara L New root->key : %s\n", t->root->key);
  tree_printInOrder(t);
  
  tree_remove(t, "j");
  tree_printInOrder(t);
  
  tree_remove(t, "l");
  printf("borde vara M New root->key : %s\n", t->root->key);
  tree_printInOrder(t);
 
  printf("New root->key : %s\n", t->root->key);
  tree_remove(t,"m");
  tree_printInOrder(t);

  printf("New root->key : %s\n", t->root->key);
  tree_remove(t,"f");
  tree_printInOrder(t);

  printf("New root->key : %s\n", t->root->key);
  tree_remove(t,"i");
  tree_printInOrder(t);

  tree_remove(t,"g");
  tree_printInOrder(t);

   printf("borde va H New root->key : %s\n", t->root->key);
  tree_remove(t,"h");
  tree_printInOrder(t);

  printf("borde va E New root->key : %s\n", t->root->key);
  tree_remove(t,"e");
  tree_printInOrder(t);

  tree_destroy(t); 
}



/*//Used for gprof goal
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
  for(int i = 50000; i <100000; ++i){
    sprintf(buf, "%d", i);
    char *key = buf;
    tree_find(tree,key);
  }
}



int main(int argc, char *argv[]){
  tree_t * tree = tree_new();
  tree_insert_gprof(tree);
  tree_destroy(tree);
  tree_t * tree2 = tree_new();
  tree_insert_gprof(tree2);
  tree_printInOrder(tree2);
  tree_find_gprof(tree2);
  tree_remove_gprof(tree2);
  tree_printInOrder(tree2);
  tree_destroy(tree2);

  return 0;
}
*/
