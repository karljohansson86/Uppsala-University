#ifndef __PRODUCT__
#define __PRODUCT__

#include "list.h"

/**
 *  \file product.h
 *  \brief Products
 *  \author Karl Johansson
 *  \author Simon Lövgren
 *
 *  Products and product related functions
 *
 */


/**
 *  \typedef product
 *  Product datatype
 */
typedef struct product_s product_t;

/**
 *  \brief Creates a new product
 *
 *  Creates a new product and returns
 *  a pointer to the product in memory.
 *
 *  \note Product is stored on the heap
 *        and so need to be freed at
 *        it's end of life.
 *
 *  \return product* Pointer to product
 */
product_t * product_new();

/**
 *  \brief `Free`s a product from memory
 *
 *  \param p Product to be freed from memory
 *  \return void
 */
void product_destroy(product_t *p);

/**
 *  \brief Set name of product
 *
 *  \param p Product to set name of
 *  \param name Name to set
 *  \return bool True if successful, False otherwise
 */
bool product_setName(product_t *p, char *name);

/**
 *  \brief Set description of product
 *
 *  \param p Product to set description of
 *  \param desc Description to set
 *  \return bool True if successful, False otherwise
 */
bool product_setDescription(product_t *p, char *desc);

/**
 *  \brief Set price of product
 *
 *  Sets price of product. Only accepts
 *  a positive price or 0.
 *
 *  \param p Product to set price of
 *  \param price Price to set
 *  \return bool True on success, False otherwise
 */
bool product_setPrice(product_t *p, int price);

/**
 *  \brief Get name of product
 *
 *  Creates a copy of the product name on the
 *  heap and returns a pointer to said copy.
 *
 *  \note The copy is stored on the heap
 *        and need to be `free`d when no
 *        longer needed.
 *
 *  \param p Product to get name of
 *  \return char* Pointer to copy of name, NULL if mo memory
 */
char * product_getName(product_t *p);

/**
 *  \brief Get description of product
 *
 *  Creates a copy of the product desciption 
 *  on the heap and returns a pointer to 
 *  said copy.
 *
 *  \note The copy is stored on the heap
 *        and need to be `free`d when no
 *        longer needed.
 *
 *  \param p Product to get description of
 *  \return char* Pointer to copy of the description
 */
char * product_getDescription(product_t *p);

/**
 *  \brief Get price of product
 *
 *  Get price of product.
 *
 *  \note The price is expressed in
 *  1/100 of a swedish krona (SEK).
 *
 *  \param p Product to get price of
 *  \return int Price
 */
int product_getPrice(product_t *p);

/**
 *  \brief Get shelves list
 *
 *  \param p Product to shelves list of
 *  \return list_t* pointer to shelves list
 */
list_t* product_getShelvesList(product_t *p);

#endif
