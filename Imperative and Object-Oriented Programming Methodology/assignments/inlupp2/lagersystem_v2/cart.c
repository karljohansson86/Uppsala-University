#include <stdio.h>
#include "storage.h"
#include "cart.h"

//////////////////////////////// Datatypes /////////////////////////////////


typedef struct cart_entry {
  int count;
  product_t* item;
} cart_entry_t;

/////////////////// Hidden (internal) function prototypes //////////////////



///////////////////////// Function Implementations /////////////////////////

cart_t* cart_new(){
  return list_new();
}

bool cart_empty(cart_t* cart){
  if(cart == NULL){
    return false;
  }
  while(cart_removeItem(cart,0));
  return true;
}

void cart_destroy(cart_t* cart){
  cart_empty(cart);
  list_destroy(cart);
}

bool cart_addItem(cart_t* cart, product_t* item, int amount){  
  if(cart != NULL && item != NULL && amount > 0){
    int existing = cart_findProductIndex(cart, item);
    if(existing >= 0) {
      cart_entry_t *cart_entry = (cart_entry_t *) list_get(cart, existing);
      if(cart_entry != NULL) {
        cart_entry->count += amount;
        return true;
      }
    } else {  
      cart_entry_t *cart_entry = malloc(sizeof(cart_entry_t));
      if(cart_entry != NULL) {
        cart_entry->count = amount;
        cart_entry->item = item;
        list_append(cart,cart_entry);
        return true;
      }
    }
  }
  return false;
}

int cart_findProductIndex(cart_t* cart, product_t* item) {
  int index = -1;
  int counter = 0;
  list_iterator_t *it = list_iterator(cart);
  while(list_more(it)){
    cart_entry_t * e = (cart_entry_t *)list_next(it);
    if(e != NULL && e->item == item){
      index = counter;
      break;
    }
    ++counter;
  }
  list_freeIterator(it);
  return index;
}

int cart_findProductsInCart(cart_t* cart, product_t* item){
  int count = 0;
  list_iterator_t *it = list_iterator(cart);
  while(list_more(it)){
    cart_entry_t * e = (cart_entry_t *)list_next(it);
    if(e != NULL && e->item == item){
      count = e->count;
      break;
    }  
  }
  list_freeIterator(it);
  return count;
}



bool cart_removeItem(cart_t* cart, int index){
  if(cart !=NULL){
    cart_entry_t * entry = list_get(cart,index);  
    if(entry != NULL){
      free(entry);
      list_remove(cart,index);
      return true;
    }
  }
  return false;
}


void cart_printShelfLocations(cart_t *cart) {
  if(cart == NULL) {
    return;
  }

  list_iterator_t *it = list_iterator(cart);
  bool hasItems = list_more(it);

  if(hasItems) {
    while(list_more(it)) {
      cart_entry_t* entry = (cart_entry_t*)list_next(it);
      int totalAvailable = storage_getCount(entry->item);
      if(totalAvailable < entry->count) {
        
      } else if(totalAvailable > 0) {
        list_iterator_t *shelves = list_iterator(product_getShelvesList(entry->item));

        int quota = entry->count;
        while(list_more(shelves)) {
          shelf_t *s = (shelf_t *)list_next(shelves);
          int shelfCount = storage_getShelfCount(s);
          char *name = product_getName(entry->item);
          
          printf("%c%d (%d %s)\n", 
                 storage_getShelfSection(s),
                 storage_getShelfNumber(s),
                 quota-shelfCount > 0 ? shelfCount : quota,
                 name);
          free(name);
          
          quota -= shelfCount;
          if(quota <= 0) {
            break;
          } 
        }
        list_freeIterator(shelves);
      }
    }
  }
  list_freeIterator(it);
}

void cart_printContents(cart_t *cart) {
  if(cart == NULL) {
    return;
  }

  list_iterator_t *it = list_iterator(cart);
  bool hasItems = list_more(it);

  if(hasItems == false) {
    puts("\nCart is empty!");
  } else {
    puts("Your cart:");    
  }

  int i = 1;
  while(list_more(it)) {
    cart_entry_t* entry = (cart_entry_t*)list_next(it);
    char *name = product_getName(entry->item);
    printf("%d\t(%d) %s\n", i++, entry->count, name);
    free(name);
  }
  list_freeIterator(it);

  puts("");
  printf("Cart total: %.2f kr\n", cart_getTotal(cart));

  if(hasItems) {
    cart_printShelfLocations(cart);
  }
}

float cart_getTotal(cart_t* cart){
  if(cart == NULL){
    return 0;// ändrad till NOLL annars är empty cart = -1SEK
  }
  
  float total = 0;
  list_iterator_t *it = list_iterator(cart);
  while(list_more(it)){
    cart_entry_t * cart_entry = list_next(it);
    total += product_getPrice(cart_entry->item) * cart_entry->count;
  }
  list_freeIterator(it);
  total = total/100;
  return total;
}

int cart_itemCount(cart_t* cart){
  if(cart == NULL){
    return 0;
  }
  int count = 0;
  list_iterator_t *it = list_iterator(cart);
  while(list_more(it)){
    cart_entry_t *cart_entry = list_next(it);
    count += cart_entry->count; 
  }
  return count;
}

int cart_uniqueItems(cart_t *cart) {
  if(cart == NULL){
    return 0;
  }
  return list_length(cart);
}


//////////////// Hidden (internal) function implementations ////////////////
