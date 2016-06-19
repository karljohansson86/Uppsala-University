#ifndef __BTREE__
#define __BTREE__

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


#include "list.h"

/**
 *  \file btree.h
 *  \brief Binary searchtree
 *  \author Tobias Wrigstad
 *  \author Karl Johansson
 *  \author Simon Lövgren
 *
 *  Binary searchtree and related functions
 *
 */



/**
 *  \typedef tree
 *  Tree datatype
 */
typedef struct tree tree_t;



/** Creates a new tree
 *
 * \returns: empty tree
 */
tree_t *tree_new();

/** 
 * \brief Get the size of the tree 
 * \param tree pointer to the tree
 * \returns: the number of nodes in the tree
 */
int tree_size(tree_t *tree);

/** 
 *  \brief Get the depth of the tree 
 *
 * \param tree pointer to the tree
 * \returns: the depth of the deepest subtree
 */
int tree_depth(tree_t *tree);

/** 
 *  \brief Checks if a is located in the tree
 *
 * \param tree pointer to the tree
 * \param key represent the name the function checks for in the tree
 * \returns: NULL if key is not in the tree otherwise it returns a ptr to the data
 */
void *tree_findData(tree_t *tree, char *key);


/** 
 *  \brief Checks if a is located in the tree and returns NODE
 *
 * \param tree pointer to the tree
 * \param key represent the name the function checks for in the tree
 * \returns: NULL if key is not in the tree otherwise it returns a ptr to NODE
 */
void *tree_find(tree_t *tree, char *key);

/** 
 *  \brief Inserts a element in the tree
 *
 * \param key represent the name the function checks for in the tree and insert if possible
 * \param data represents the data to be inserted in the tree(ptr to the data)
 * \returns: true if possible to insert otherwise false, example if key already in the tree 
 */
bool tree_insert(tree_t *tree, char *key, void *data);

/** 
 *  \brief Removes a specific element from the tree
 *  **NOTE it does not free the data**  
 *
 *  \param key represent the name the function checks for in the tree and removes it and it's
 *  leaf if possible 
 *  \returns: true if it was able to remove otherwise false
 */
bool tree_remove(tree_t *tree, char *key);


/** 
 * \brief Creates a list with pointers to the data from the data pointers in the tree
 *
 * \returns: A ptr to to the list
 */
list_t *tree_getAll(tree_t *tree);

/** 
 * \brief Destroys the tree, removes and free all the nodes and node keys
 * **NOTE it does not free the data** 
 * 
 * \returns: nothing
 */ 
void tree_destroy(tree_t *tree);

/**
 *  \brief Prints all key in the tree in alphabetical order
 *
 *  \returns void
 */
void tree_printInOrder(tree_t *tree);

#endif
