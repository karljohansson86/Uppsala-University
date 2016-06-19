#ifndef __controller__
#define __controller__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "storage.h"
#include "history.h"
#include "ui.h"
#include "cart.h"

/**
 *  \file controller.h
 *  \brief Contains control logic of system
 *  \author Karl Johansson
 *  \author Simon Lövgren
 *
 *  Controller module. Responsible for control flow
 *  of the application
 *
 */

/**
 *  \brief Ask user to input string
 *
 *  \param q Pointer to string containing question
 *  \return char*  Pointer to inputstring on heap
 */
char* controller_askQuestionString(char *q);

/**
 *  \brief Ask user to input integer
 *
 *  \param q Pointer to string containing question
 *  \param positiveOnly True to only allow positive integers
 *  \return int Entered integer
 */
int controller_askQuestionInt(char *q, bool positiveOnly);


/**
 *  \brief Ask user to input single character
 *
 *  \param q Pointer to string containing question
 *  \param alt Accepted charachters
 *  \return char Entered character
 */
char controller_askQuestionChar(char *q, char *alt);

/**
 *  \brief Ask for main menu choice
 *
 *  \return char Menu choics/selection
 */
char controller_askForMenuChoice();

/**
 *  \brief Acts on main menu choice
 *
 *  \param s      Pointer to storage in use
 *  \param choice Main menu choice from `controller_askForMenuChoice`
 *  \param quit   Pointer to bool to be set True to exit main loop
 *  \return void
 */
void controller_actOnMenuChoice(storage_t* s, char choice, bool *quit, cart_t *c);

#endif
