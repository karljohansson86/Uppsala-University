#ifndef __cart__
#define __cart__
#include "list.h"
#include "product.h"

/**
 *  \file cart.h
 *  \brief Cart module for lagersys.
 *  \author Karl Johansson
 *  \author Simon Lövgren
 *  
 *  Cart consists of cart functionality, as
 *  add to cart, calculate total, calculate
 *  optimal pick-up route etc.
 *
 */

/**
 *  \typedef cart_t
 *
 *  Object holding all cart data
 */
typedef list_t cart_t;

/**
 *  \brief Creates a new cart
 *
 *  Creates a new cart. When a cart is no longer needed,
 *  use cart_destroy to free the memory used by the cart.
 *  This circumvents memory leaks.
 *
 *  \return cart_t* pointer to new cart
 */
cart_t* cart_new();

/**
 *  \brief Empty a cart
 *
 *  Empties a cart. Removes all items from
 *  supplied cart, including item count, but
 *  leaves the cart itself intact.
 *
 *  \param cart Cart to empty
 *  \return bool True on success, otherwise False
 */
bool cart_empty(cart_t* cart);

/**
 *  \brief Destroy a cart (free associated memory)
 *
 *  Frees all memory used by supplied cart. Use this
 *  when a cart no longer is needed to circumvent
 *  memory leaks.
 *
 *  \param cart pointer to cart that is to be destroyed
 *  \return return void
 */
void cart_destroy(cart_t* cart);

/**
 *  \brief Add item to cart
 *
 *  Add add a item to the cart.
 *
 *  \param cart cart to which the product is to be added.
 *  \param item pointer to the item
 *  \param amount number of items to add to cart
 *  \return boolean True on success, otherwise False
 */
bool cart_addItem(cart_t* cart, product_t* item, int amount);


/**
 *  \brief Return index of product in cart, -1 if not found
 *
 *  \param cart Cart to search in
 *  \param cart Product to search for
 *  \return int  Index in cart, -1 if not found
 */
int cart_findProductIndex(cart_t* cart, product_t* item);

/**
 *  \brief Return count of product in cart, 0 if not present
 *
 *  \param cart Cart to search in
 *  \param cart Product to search for
 *  \return int  Count of products in cart
 */
int cart_findProductsInCart(cart_t* cart, product_t* item);


/**
 *  \brief Remove item from cart
 *
 *  Remove item from cart.
 *
 *  Example (Assume cart has more than 3 items):
 *
 *  // Remove first item
 *
 * **cart_remove(cart, 0);**
 *
 *  // Remove last item
 *
 *  **cart_remove(cart, -1);**
 *
 *  // Remove the third item
 *
 *  **cart_remove(cart, 2);**
 *
 *  \param cart cart from which a item is to be removed
 *  \param index index of item to be removed
 *  \return boolean True on success, otherwise False
 */
bool cart_removeItem(cart_t* cart, int index);

/**
 *  \brief Get total cart price
 *
 *  Get total price of items in cart.
 *
 *  \param cart Cart to calculate total of
 *  \return return float Total price
 */
float cart_getTotal(cart_t* cart);

/**
 *  TODO: decide on shelf locations parameters and return value
 */
void cart_getShelfLocations();

/**
 *  \brief Get total items in cart
 *
 *  Get total item count of cart
 *
 *  \param cart Cart to calculate total items of
 *  \return return int item count of cart
 */
int cart_itemCount(cart_t* cart);

/**
 *  \brief Get number of unique items in cart
 *
 *  \param cart to count
 *  \return int  Unique items in cart
 */
int cart_uniqueItems(cart_t *cart);

/**
 *  \brief Print cart contents
 *
 *  \param cart Cart to print contents of
 *  \return void
 */
void cart_printContents(cart_t *cart);

#endif
