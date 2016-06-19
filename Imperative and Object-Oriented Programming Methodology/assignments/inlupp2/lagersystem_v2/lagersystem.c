#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "controller.h"
#include "storage.h"
#include "history.h"
#include "ui.h"


/**
 *  Lagersystem - Uppgift 2
 *  =======================
 *  IOOPM-UU 2015
 *  Karl Johansson & Simon Lovgren
 */


int main(int argc, char **argv) {
  storage_t* s = storage_new();
  history_t* h = history_new();
  cart_t* c = cart_new();

  // Populate storage w/ products
  storage_populate(s, 32);
  
  ui_splashScreen();

  bool quit = false;
  while (!quit){
    ui_mainMenu();
    
    char choice = controller_askForMenuChoice();
    controller_actOnMenuChoice(s, choice, &quit,c);
  }
  cart_destroy(c);
  storage_free(s);
  history_free(h);

  return 0;
}
