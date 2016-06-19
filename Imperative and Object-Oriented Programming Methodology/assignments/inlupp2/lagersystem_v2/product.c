#include "product.h"

#include <stdlib.h>
#include <string.h>

#include "list.h"



// Typedef to help GCC on some systems
char *strdup(const char *string);

//////////////////////////////// Datatypes /////////////////////////////////

struct product_s {
  char* name;
  char* desc;
  char* available;
  int price;
  list_t* shelves;
};


/////////////////// Hidden (internal) function prototypes //////////////////
/**
 *  \brief Copy a string to memory
 *
 *  Copies a string to memory and deposits
 *  it's pointer in given param `pointer`.
 *
 *  \param pointer  Pointer to where the pointer of the string copy shall be stored 
 *  \param string   String to be copied
 *  \return bool    True on success, False otherwise
 */
bool _product_duplicateString(char** pointer, char* string);

///////////////////////// Function Implementations /////////////////////////

product_t * product_new() {
  product_t* new = malloc(sizeof(struct product_s));
  if(new != NULL) {
    new->name = NULL;
    new->desc = NULL;
    new->available = NULL;
    new->price = 0;
    new->shelves = list_new();
  }
  return new;
}

void product_destroy(product_t *p) {
  if(p != NULL) {
    if(p->name != NULL) {
      free(p->name);
    }
    if(p->desc != NULL) {
      free(p->desc);
    }
    if(p->available != NULL) {
      free(p->available);
    }
    if(p->shelves != NULL) {
      list_destroy(p->shelves);
    }
    free(p);
  }
}

/* TODO:  Do we still need copy? */
// product_t * product_copy(product_t *p);

bool product_setName(product_t *p, char *name) {
  return p != NULL ? _product_duplicateString(&(p->name), name) : false;
}

bool product_setDescription(product_t *p, char *desc){
  return p != NULL ? _product_duplicateString(&(p->desc), desc) : false;
}

bool product_setAvailability(product_t *p, char *avail){
  return p != NULL ? _product_duplicateString(&(p->available), avail) : false;
}

bool product_setPrice(product_t *p, int price) {
  if(p != NULL) {
    p->price = price;
    return p->price == price;
  }
  return false;
}

char * product_getName(product_t *p) {
  if(p != NULL) {
    return strdup(p->name); 
  }
  return NULL;
}

char * product_getDescription(product_t *p) {
  if(p != NULL) {
    return strdup(p->desc); 
  }
  return NULL;
}

bool product_getAvailability(product_t *p, char *avail){
  if(p != NULL) {
    return strdup(p->available);
  }
  return NULL;
}

int product_getPrice(product_t *p) {
  if(p != NULL) {
    return p->price; 
  }
  return -1;
}

list_t* product_getShelvesList(product_t *p) {
  if(p != NULL){
    if(p->shelves != NULL) {
      return p->shelves;
    }
  }
  return NULL;
}

//////////////// Hidden (internal) function implementations ////////////////

bool _product_duplicateString(char** pointer, char* string) {
  if(pointer != NULL) {
    if(*pointer != NULL) {
      free(*pointer);
    }

    *pointer = strdup(string);

    return *pointer ? true : false;
  }
  return false;
}
