#ifndef __storage__
#define __storage__

#include <stdbool.h>

#include "btree.h"
#include "list.h"
#include "product.h"

/**
 *  \file storage.h
 *  \brief Caters to storage
 *  \author Karl Johansson
 *  \author Simon Lövgren
 *
 *  Caters to storage-related datatypes,
 *  functions. Assigning shelf-space and
 *  so on.
 *
 */

// Macros

#define STORAGE_SHELF_TAKEN 0
#define STORAGE_ADDED_SHELF 1
#define STORAGE_UPDATED_SHELFCOUNT 2

// Datatypes

/**
 *  \typedef
 *  Pointer to storage
 */
typedef tree_t storage_t;

/**
 *  \typedef
 *  Shelf
 */
typedef struct shelf_s shelf_t;

/**
 *  \typedef
 *  Storage iterator
 */
typedef struct storage_iterator storage_iterator_t;

// Functions


// Storage handling

/**
 *  \brief Creates a storage
 *
 *  Creates a storage and returns
 *  pointer to where the storage
 *  is located.
 *
 *  \note  To remove a storage from memory, call
 *         `storage_free`.
 *
 *  \return storage Pointer to storage
 */
storage_t* storage_new();

/**
 *  \brief Frees storage from memory
 *
 *  \param storage Storage to be deleted
 *  \return void
 */
void storage_free(storage_t* s);

/**
 *  \brief Returns amount of products in storage
 *
 *  \param  s    storage to get product count from
 *  \return int  Amount of products in storage
 */
int storage_totalProducts(storage_t *s);

/**
 *  \brief Add a product to storage
 *
 *  \param storage Storage to add product to
 *  \param product* Product to be added
 *  \return bool    True on success, False otherwise
 */
bool storage_insert(storage_t* s, product_t* p);

/**
 *  \brief Remnove a product from storage
 *
 *  \param storage Storage from which to remove product
 *  \param key Name of product to remove
 *  \return bool True on success, False otherwise
 *
 * 
 */
bool storage_remove(storage_t* s, char* key);

/**
 *  \brief Find product by key (name)
 *
 *  \param   s     Storage to search in
 *  \param   name  Name of storage 
 *  \return  product_t *  Pointer to product, NULL if not found
 */
product_t *storage_find(storage_t *s, char *key);

/**
 *  \brief Find product using numeric index
 *
 *  Find product using numeric index
 *
 *  \param storage   Storage to search through
 *  \param index     Numeric index (int)
 *  \return product_t* Pointer to product
 */
product_t* storage_findProductByIndex(storage_iterator_t* it, int index);

// Shelf handlers

/**
 *  \brief Sort shelves in order of most products first
 *
 *  \param sl shelf list to sort
 *  \return void
 */
void storage_sortShelves(list_t *sl);

/**
 *  \brief Get index of shelf in shelf list in product
 *
 *  \param product   Product on shelf
 *  \param section   Section of shelf
 *  \param shelf     Shelf number
 *  \return int      Index in shelf list, -1 if not in list
 */
int storage_getShelfIndex(product_t *p, char section, int shelf);

/**
 *  \brief Set shelf section
 *
 *  \param shelf     Shelf to set section of
 *  \param section   Char denoting section (A-Z)
 *  \return void
 */
void storage_setShelfSection(shelf_t* shelf, char section);

/**
 *  \brief Gets shelf section
 *
 *  \param shelf Shelf to get section of
 *  \return char Section
 */
char storage_getShelfSection(shelf_t* shelf);

/**
 *  \brief Set shelf number of shelf
 *
 *  \param shelf  Shelf to set number of
 *  \param number Shelf number to set
 *  \return void
 */
void storage_setShelfNumber(shelf_t* shelf, int number);

/**
 *  \brief Get shelf number of shelf
 *
 *  \param shelf  Shelf to get number of
 *  \return int   Shelf number
 */
int storage_getShelfNumber(shelf_t* shelf);

/**
 *  \brief Set amount/count of products on shelf
 *
 *  \param shelf  Shelf to set amount/count of
 *  \param count  Number of products to set on shelf
 *  \return void
 */
void storage_setShelfCount(shelf_t* shelf, int count);

/**
 *  \brief Get amount/count of products on shelf
 *
 *  \param shelf  Shelf to set amount/count of
 *  \return int   Number of products on shelf
 */
int storage_getShelfCount(shelf_t* shelf);

/**
 *  \brief Check if product is on a shelf
 *
 *  \param p       Product to check
 *  \param section Shelf section
 *  \param shelf   Shelf number
 *
 *  \return bool   True if product is on shelf, False otherwise
 */
bool storage_productOnShelf(product_t* p, char section, int shelf);

/**
 *  \brief Get total count of products on all shelves
 *
 *  \param p product to get total count of
 *  \return int total product count
 */
int storage_getCount(product_t *p);



/**
 *  \brief Copies shelves from one list to another
 *
 *  Copies shelves from one list to another
 *
 *  \warning  This function does not check if
 *            shelves are taken in any storage
 *            and should only be used in cases
 *            of backing-up products.
 *
 *  \param from  List to copy shelves from
 *  \param to    List to insert copies into
 *  \return void
 */
void storage_copyShelves(list_t* from, list_t* to);

/**
 *  \brief Find product by shelf- section and number
 *
 *  \param storage  Storage to search in
 *  \param section  Section to search for
 *  \param shelf    Shelf number to search for
 *  \return product_t* Pointer to product, NULL if none found
 */
product_t* storage_findProductByShelf(storage_t* storage, char section, int shelf);

/**
 *  \brief Add product to shelf
 *
 *  Add products to shelf. Returns status flag
 *  identifiable by macrod:
 *
 *  **STORAGE_SHELF_TAKEN** - Shelf taken by
 *  other product.
 *
 *  **STORAGE_ADDED_SHELF** - Shelf free, product
 *  was added to shelf.
 *
 *  **STORAGE_UPDATED_SHELFCOUNT** - Product
 *  already on this shelf. Added count to
 *  current count on selected shelf (updated
 *  product count).
 *
 *  \param s       Storage in which product is located
 *  \param p       Product to be added to shelf
 *  \param section Shelf section
 *  \param shelf   Shelf number
 *  \param count   Number of products to set on/add to shelf
 *  \return void
 */
int storage_addToShelf(storage_t* s, product_t* p,
                        char section, int shelf, int count);

/**
 *  \brief Remove product from shelf
 *
 *  Remove product from shelf. Only
 *  works if product is placed on shelf.
 *
 *  \param p       Product in question
 *  \param section Section of shelf to remove product from
 *  \param shelf   Shelf number to remove product from
 *  \return bool   True if OK, False otherwise
 */
bool storage_removeFromShelf(product_t* p, char section, int shelf);

/**
 *  \brief Update shelf count on shelf
 *
 *  \param p        product on shelf
 *  \param section  Shelf section
 *  \param shelf    Shelf number
 *  \param newCount New shelf count
 *  \return bool True if OK, false otherwise
 */
bool storage_updateShelfCount(product_t* p, char section, int shelf, int newCount);

/**
 *  \brief Move product between shelves
 *
 *  Moves products between shelves
 *
 *  \param s           Storage in which product is located
 *  \param p           Product to be moved
 *  \param fromSection Shelf section to move from
 *  \param fromShelf   Shelf number to move from
 *  \param toSection   Shelf section to move to
 *  \param toShelf     Shelf number to move to
 *
 *  \return bool  True if OK, False otherwise
 */
bool storage_moveToShelf(storage_t* s, product_t* p, 
                         char fromSection, int fromShelf,
                         char toSection, int toShelf);

/**
 *  \brief Remove froduct from all shelves
 *
 *  \param p product to remove from shelves
 *  \return void
 */
void storage_removefromAllShelves(product_t *p);

// Iterator

/**
 *  \brief Creates an iterator for gven storage
 *
 *  \param s Storage to create iterator from
 *  \return storage_iterator_t*  Pointer to storage iterator
 */
storage_iterator_t* storage_iterator(storage_t* s);

/**
 *  \brief `Free`s an iterator from memory
 *
 *  \param it  Iterator to free
 */
void storage_freeIterator(storage_iterator_t* it);

/**
 *  \brief Gets next element from iterator
 *
 *  \param it Pointer to iterator
 *  \return product* Product
 */
product_t* storage_next(storage_iterator_t* it);

/**
 *  \brief Checks if iterator has more elements
 *
 *  \param it Pointer to iterator
 *  \return bool True if more elements exist, False otherwise
 */
bool storage_more(storage_iterator_t* it);

/**
 *  \brief Prints the product name of the products in storage in alphabetical order
 *
 *  \param listOfItems      List containing all the names of the products in storage
 *  \param productPerPage   Products to be printed per page
 *  \param page             Pagenumber of the products that will be printed  
 *  \param amountOfItems    The amount of different products in storage
 *
 *  \return void
 */
void storage_listProducts(char * listOfItems[],int productPerPage, int page, int amountOfItems);

/**
 *  \brief Removes the contents at specific location in the list and free it from the memory
 *
 *  \param listOfItems   List containing all the names of the products in storage
 *  \param index   location in the list of the item to be removed
 *
 *  \return void
 */
void storage_removeFromList(char **listOfItems, int index);


/**
 *  \brief Free a list and its contents from the memory
 *
 *  \param listOfItems   List containing all the names of the products in storage
 *  \param amountOfItems   The amount of different products in storage
 *
 *  \return void
 */
void storage_freeList(char **listOfItems, int amountOfItems);

/**
 *  \brief Removes a product from the from the storage and inserts it again with an 
 *         updated name
 *
 *  \param s   Storage in which the product was added
 *  \param p   Product that was copied,removed and inserted again
 *  \param old   Old key/name of the product 
 *
 *  \return void
 */
void storage_reinsert(storage_t *s, product_t *p, char * old);

// History handlers

/**
 *  \brief Save history state for adding a product
 *
 *  \param storage   Storage in which the product was added
 *  \param product   Product that was added
 *
 *  \return void
 */
void storage_historyAddedProduct(storage_t* storage, product_t* product);

/**
 *  \brief Save history state for removing a product
 *
 *  \param storage   Storage from which the product was removed
 *  \param product   Product that was removed
 *
 *  \return void
 */
void storage_historyRemovedProduct(storage_t* storage, product_t* product);

/**
 *  \brief Save history state for editing a product
 *
 *  \param storage   Storage in which the product was edited
 *  \param product   Product that was edited
 *  \param original  Original version of product
 *
 *  \return void
 */
void storage_historyEditedProduct(storage_t* storage, product_t* product, product_t* original);

/**
 *  \brief Populate storage with products for testing*
 *
 *  \param   s       Storage to populate
 *  \param   count   Amount of products to insert
 *  \return  void
 */
void storage_populate(storage_t *s, int count);
#endif
