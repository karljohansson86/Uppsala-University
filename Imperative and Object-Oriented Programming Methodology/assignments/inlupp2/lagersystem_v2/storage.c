#include "storage.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h> 
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <assert.h>

#include "list.h"
#include "product.h"
#include "btree.h"
#include "history.h"
#include "ui.h"


// Defines

#define STORAGE_BASIC_FREE NULL
#define PRODUCTS_PER_PAGE 20

// Structs

struct shelf_s {
  char section;
  int shelf;
  int count;
};

struct storage_iterator {
  /* TODO:  Stuff for iterator */
  list_t* productList;
  list_iterator_t* iterator;
};

// Internal prototype
void _storage_freeShelf(shelf_t* s);

// Storage handling

storage_t* storage_new() {
  return tree_new();
}

int storage_totalProducts(storage_t *s) {
  return tree_size(s);
}

void storage_free(storage_t* s) {
  list_t *prodList = tree_getAll(s);
  list_iterator_t *prodIt = list_iterator(prodList);
  while(list_more(prodIt)) {
    product_t *p = (product_t *) list_next(prodIt);
    
    if(p != NULL) {
      list_iterator_t *shelfIt = list_iterator(product_getShelvesList(p));
      while(list_more(shelfIt)) {
        shelf_t *sh = (shelf_t *) list_next(shelfIt);
        if(sh != NULL) {
          _storage_freeShelf(sh);
        }
      }
      product_destroy(p);
      list_freeIterator(shelfIt);
    }
  }
  list_freeIterator(prodIt);
  list_destroy(prodList);

  tree_destroy(s);
}

bool storage_insert(storage_t* s, product_t* p) {
  char* key = product_getName(p);
  bool result = tree_insert(s, key, p);
  free(key);
  return result;
}

void storage_reinsert(storage_t *s, product_t *p, char * old){
  product_t *copy = p;
  tree_remove(s,old);
  storage_insert(s, copy);
}

product_t *storage_find(storage_t* s,char *key){
  return (product_t *)tree_findData(s,key);
}

bool storage_remove(storage_t* s, char* key) {
  product_t *p = (product_t *) tree_findData(s, key);
  if(p != NULL) {
    list_t *shelves = product_getShelvesList(p);
    if(shelves != NULL) {
      list_iterator_t *shelfIt = list_iterator(shelves);
      while(list_more(shelfIt)) {
        shelf_t *sh = (shelf_t *) list_next(shelfIt);
        if(sh != NULL) {
          _storage_freeShelf(sh);
        }
      }
      list_freeIterator(shelfIt);
    }
    product_destroy(p);
  }  

  return tree_remove(s, key);
}

product_t* storage_findProductByIndex(storage_iterator_t* it, int index) {
  return (product_t*)list_get(it->productList, index);
}

// Shelf handlers
void _storage_freeShelf(shelf_t* s) {
  free(s);
}

int _storage_shelfInList(list_t* list, char section, int shelf) {
  section = toupper(section);
  list_iterator_t* it = list_iterator(list);
  bool found = false;
  int index = 0;
  while(list_more(it)){
    shelf_t* s = (shelf_t*)list_next(it);
    if(s != NULL && s->section == section && s->shelf == shelf) {
      found = true;
      break;
    }
    ++index;
  }
  list_freeIterator(it);
  return found ? index : -1;
}

bool _storage_verifySection(char section) {
  section = toupper(section);
  if(section > 64 && section < 91) {
    return true;
  }
  return false;
}

int storage_getShelfIndex(product_t *p, char section, int shelf) {
  list_t *l = product_getShelvesList(p);
  int idx = _storage_shelfInList(l, section, shelf);
  return idx;
}

shelf_t* _storage_createShelf(char section, int shelf, int count) {
  shelf_t* new = malloc(sizeof(shelf_t));
  new->section = section;
  new->shelf = shelf;
  new->count = count;
  return new;
}


void storage_setShelfSection(shelf_t* shelf, char section) {
  shelf->section = section;
}

char storage_getShelfSection(shelf_t* shelf) {
  return shelf->section;
}

void storage_setShelfNumber(shelf_t* shelf, int number) {
  shelf->shelf = number;
}

int storage_getShelfNumber(shelf_t* shelf) {
  return shelf->shelf;
}

void storage_setShelfCount(shelf_t* shelf, int count) {
  shelf->count = count;
}

int storage_getShelfCount(shelf_t* shelf) {
  return shelf->count;
}

shelf_t*_storage_copyShelf(shelf_t* shelf) {
  return _storage_createShelf(shelf->section, shelf->shelf, shelf->count);
}

bool storage_productOnShelf(product_t* p, char section, int shelf) {
  int shelfIndex = _storage_shelfInList(product_getShelvesList(p), section, shelf);
  return (shelfIndex >= 0);
}

int storage_getCount(product_t *p) {
  list_iterator_t *it = list_iterator(product_getShelvesList(p));

  int total = 0;
  while(list_more(it)) {
    shelf_t *s = (shelf_t *)list_next(it);
    total += storage_getShelfCount(s);
  }
  list_freeIterator(it);
  return total;
}

void storage_copyShelves(list_t* from, list_t* to) {
  assert(from != NULL && "From-list NULL in storage_copyShelves");
  assert(to != NULL && "To-list NULL in storage_copyShelves");

  list_iterator_t* it = list_iterator(from);
  while(list_more(it)) {
    shelf_t* shelf = (shelf_t*) list_next(it);
    list_append(to, _storage_copyShelf(shelf));
  }
}

product_t* storage_findProductByShelf(storage_t* storage, char section, int shelf) {
  list_t *products = tree_getAll(storage);
  list_iterator_t* it = list_iterator(products);
  
  product_t *p = NULL;
  while(list_more(it)) {
    p = (product_t *)list_next(it);
    if(p != NULL && _storage_shelfInList(product_getShelvesList(p), section, shelf) > -1) {
      break;
    } else {
      p = NULL;
    }
  }

  list_freeIterator(it);
  list_destroy(products);
  return p;
}

int storage_addToShelf(storage_t* s, product_t* p, char section, int shelf, int count) {
  section = toupper(section);
  if(p != NULL && _storage_verifySection(section) && shelf > -1 && count > -1) {
    int shelfIndex = _storage_shelfInList(product_getShelvesList(p), section, shelf);
    if(shelfIndex >= 0) {
      // Shelf in current product - Add to shelf count
      shelf_t* existing = list_get(product_getShelvesList(p), shelfIndex);
      storage_setShelfCount(existing, storage_getShelfCount(existing) + count);

      return STORAGE_UPDATED_SHELFCOUNT;
    } else if (storage_findProductByShelf(s, section, shelf) == NULL) {
      // No other product has the shelf, append to product
      shelf_t* new = _storage_createShelf(section, shelf, count);
      list_append(product_getShelvesList(p), new);
      storage_sortShelves(product_getShelvesList(p));
      return STORAGE_ADDED_SHELF;
    }
  }
  // Shelf taken by other product
  return STORAGE_SHELF_TAKEN;
}

void storage_sortShelves(list_t *sl) {

  //  return;
  // BLOCKED OFF AS IT CORRUPTS THE LIST

  bool moved = false;
  do{
    moved = false;
    int len = list_length(sl);
    
    if(len > 1) {
      for(int i = 0; i < len-1; ++i) {
        shelf_t *s1 = list_get(sl, i);
        shelf_t *s2 = list_get(sl, i+1);
        if(s1->count < s2->count) {
          list_remove(sl, i);
          if(i+2 == len) {
            list_append(sl, s1); 
          } else {
            list_insert(sl, i+1, s1);
          }
          moved = true;
        }
      }
    }
  } while(moved);
}

bool storage_removeFromShelf(product_t* p, char section, int shelf) {
  section = toupper(section);
  list_t* list = product_getShelvesList(p);
  int shelfIndex = _storage_shelfInList(list, section, shelf);
  if(shelfIndex > -1) {
    shelf_t* s = list_get(list, shelfIndex);
    _storage_freeShelf(s);
    list_remove(list, shelfIndex);
    return true;
  }
  return false;
}

bool storage_moveToShelf(storage_t* s, product_t* p, char fromSection, int fromShelf,
                         char toSection, int toShelf) {
  list_t* list = product_getShelvesList(p);
  int shelfIndex = _storage_shelfInList(list, fromSection, fromShelf);
  if(shelfIndex > -1) {
    shelf_t* sh = list_get(list, shelfIndex);
    if(storage_addToShelf(s, p, toSection, toShelf, sh->count)) {
      list_remove(list, shelfIndex);
      _storage_freeShelf(sh);
      storage_sortShelves(list);
      return true;
    }
  }
  return false;
}

void storage_removefromAllShelves(product_t *p) {
  list_iterator_t *it = list_iterator(product_getShelvesList(p));
  while(list_more(it)) {
    shelf_t *s = (shelf_t *) list_next(it);
    _storage_freeShelf(s);
  }
  list_freeIterator(it);
}

bool storage_updateShelfCount(product_t* p, char section, int shelf, int newCount) {
  if (newCount >= 0) { 
    list_t* list = product_getShelvesList(p);
    int shelfIndex = _storage_shelfInList(list, section, shelf);
    if(shelfIndex > -1) {
      shelf_t *s = list_get(list, shelfIndex);
      printf("New count: %d\n", newCount);
      s->count = newCount;
      storage_sortShelves(list);
      return true;
    }
  }
  return false;
}

// Iterator
storage_iterator_t* storage_iterator(storage_t* s) {

  storage_iterator_t* it = 
    (storage_iterator_t*) malloc(sizeof(storage_iterator_t));
  
  if(it != NULL) {
    it->productList = (list_t*) tree_getAll(s);
    it->iterator = list_iterator(it->productList);
    if(it->productList == NULL || it->iterator == NULL) {
      storage_freeIterator(it);
      it = NULL;
    }
  }

  return it;
}

void storage_freeIterator(storage_iterator_t* it) {
  if(it != NULL) {
    list_freeIterator(it->iterator);
    list_destroy(it->productList);
    free(it);
  }
}

product_t* storage_next(storage_iterator_t* it) {
  return (product_t*) list_next(it->iterator);
}

bool storage_more(storage_iterator_t* it) {
  return list_more(it->iterator);
}

void storage_listProducts(char * listOfItems[],int productPerPage, int page, int amountOfItems){
  int count = (productPerPage * page)+1;
  int end = count + productPerPage;
  ui_printListHeader(page+1);
  for(; count<end && count <= amountOfItems ; ++count){
    if(listOfItems[count-1] != NULL)
       ui_printListItem(listOfItems[count-1],count);
  }
  ui_printListFoot();      
  ui_listOptions();
}

void storage_removeFromList(char **listOfItems, int index){
  if(listOfItems[index] != NULL){
    free(listOfItems[index]);
    listOfItems[index] = NULL;
    }
}

void storage_freeList(char **listOfItems, int amountOfItems){
  for(int i = 0; i < amountOfItems; ++i){
    storage_removeFromList(listOfItems, i);
  }
}


// History handlers

struct storage_history {
  storage_t* storage;
  product_t* product;
  product_t* original; // Used for edited products
};

void _storage_freeHistory(void* data) {
  struct storage_history* h = (struct storage_history*) data;
  product_destroy(h->product);
  product_destroy(h->original);
  free(h);
}

void _storage_freeHistoryEdited(void* data) {
  struct storage_history* h = (struct storage_history*) data;
  // Remove reference to product that is currently
  // stored in tree as to not inadvertedly free
  // a currently used peoduct
  h->product = NULL;
  
  _storage_freeHistory(data);
}

void _storage_setHistoryState(storage_t* storage, product_t* product,
                              product_t* original, history_handler handler,
                              history_freeData freeFunction) {
  struct storage_history* historyData = 
    (struct storage_history*) malloc(sizeof(struct storage_history));

  assert(historyData != NULL && "Cannot create history step - Out of memory.");

  historyData->storage  = storage;
  historyData->product  = product;
  historyData->original = original;

  history_state_t* state = history_newState();
  history_setData(state, historyData);
  history_setHandler(state, handler);

  if(freeFunction != NULL) {
    history_setFreeFunction(state, freeFunction);
  } else {
    history_setFreeFunction(state, _storage_freeHistory);
  }
}

// -- Add product

void _storage_hHandlerAddedProduct(void* data, int action) {
  struct storage_history* h = (struct storage_history*) data;
  if(action == HISTORY_ACTION_UNDO) {
    // UNDO
    char* key = product_getName(h->product);
    storage_remove(h->storage, key);
    free(key);
  } else if(action == HISTORY_ACTION_REDO) {
    // REDO
    storage_insert(h->storage, h->product);
  }
}

void storage_historyAddedProduct(storage_t* storage, product_t* product) {
  _storage_setHistoryState(storage, product, NULL, _storage_hHandlerAddedProduct, STORAGE_BASIC_FREE);
}

// -- Remove product

void _storage_hHandlerRemovedProduct(void* data, int action) {
  struct storage_history* h = (struct storage_history*) data;
  if(action == HISTORY_ACTION_UNDO) {
    // UNDO
    storage_insert(h->storage, h->product);
  } else if(action == HISTORY_ACTION_REDO) {
    // REDO
    char* key = product_getName(h->product);
    storage_remove(h->storage, key);
    free(key);
  }
}

void storage_historyRemovedProduct(storage_t* storage, product_t* product) {
  _storage_setHistoryState(storage, product, NULL, _storage_hHandlerRemovedProduct, STORAGE_BASIC_FREE);
}

// -- Edit product

void _storage_hHandlerEditedProduct(void* data, int action) {
  struct storage_history* h = (struct storage_history*) data;

  // Get old/new (revert) name
  char* revertName    = product_getName(h->original);
  // Get current name
  char* currentName = product_getName(h->product);

  // Remove current from storage and insert old/new
  tree_remove(h->storage, currentName);
  tree_insert(h->storage, revertName, h->original);

  // Switch original and product in storage_history
  // to reverse the effect on next call as well as
  // guard from freeing the product put in tree.
  product_t* tmp = h->original;
  h->original = h->product;
  h->product = tmp;
}

void storage_historyEditedProduct(storage_t* storage, product_t* product, product_t* original) {
  _storage_setHistoryState(storage, product, original, _storage_hHandlerEditedProduct, _storage_freeHistoryEdited);
}



///////////////////  TEST/DEBUG FUNCTIONS /////////////
void storage_populate(storage_t *s, int count) {
  char section = 'A';
  int shelf = 0;
  
  for(int i = 0; i<count; ++i){
    
    product_t *p = product_new();

    char name[20] = "";
    if(p != NULL) {
      sprintf(name, "Product #%d", i);
      product_setName(p, name);
      product_setDescription(p, name);
      product_setPrice(p, (i+1) * 100);
      
      while(storage_addToShelf(s, p, section, shelf, (i+1) * 21)
            == STORAGE_SHELF_TAKEN)
        {
          if(section == 'Z')
            {
              section = 'A';
              ++shelf;
            }
          else
            {
              ++section;          
            }
        }
      storage_insert(s,p);
    }
  }
}
