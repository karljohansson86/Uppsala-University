#ifndef __ui__
#define __ui__

#include <stdlib.h>
#include <stdio.h>

#include "storage.h"

/**
 *  \file ui.h
 *  \brief UI module for lagersys
 *  \author Karl Johansson, Simon Lövgren
 *  
 *  Contains all functions needed to print
 *  near all of the user interface of the
 *  lagersystem.
 */

#ifndef Clear_stdin
/**
 *  \brief Clears stdin
 *
 *  Clears stdin. Example of usage is to clear
 *  stdin after grabbing a single character, as
 *  to remove possible junk input.
 */
#define Clear_stdin while (getchar() != '\n');
#endif

/**
 *  \brief Print alert message
 *
 *  Prints supplied alert message to screen,
 *  then waits for user to press enter
 *  before continuing as to make sure,
 *  the user sees the message.
 *
 *  Can also be used to "pause" the prodgram
 *  allowing the user to continue at his/her
 *  own pace. For this use, simply supply
 *  an empty string as message.
 *
 *  \param message message to print
 *  \return return void
 */
void ui_alert(char* message);

/**
 *  \brief Prints the Splashscreen
 *
 *  Prints contents of splash.txt to screen
 *
 *  \return void
 */
void ui_splashScreen();

/**
 *  \brief Print main menu options
 *
 *  Prints main menu options
 *
 *  \return void
 */
void ui_mainMenu();

/**
 *  \brief Print edit menu options
 *
 *  Prints main menu options
 *
 *  \return void
 */
void ui_editMenu();

/**
 *  \brief Print listing options
 *
 *  Prints option menu for when viewing
 *  the product listing.
 *
 *  \return void
 */
void ui_listOptions();

/**
 *  \brief Print shelf edit menu
 *
 *  \retur void
 */
void ui_printShelfEditMenu();

/**
 *  \brief Print single product
 *
 *  Print information of a single
 *  product.
 *
 *  \param p Product to be printed
 *  \return void
 */
void ui_printProduct(product_t* p);

/**
 *  \brief Print paged product list
 *
 *  Prints paged product list
 *
 *  \param s Storage from which to list products
 *  \param page Page to diaplay
 *  \param limit Products displayed per page
 *  \return True if more pages exist, otherwise False
 */
bool ui_listProducts(storage_t s, int page, int limit);

void ui_printShelves(product_t *p);

void ui_printListItem(char * item, int index);

void ui_printListHeader(int page);

void ui_printListFoot();

void ui_printCartMenu();

#endif
