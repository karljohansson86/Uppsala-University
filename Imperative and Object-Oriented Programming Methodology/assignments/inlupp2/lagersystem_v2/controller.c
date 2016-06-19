#include <assert.h>
#include "controller.h"
#include "input.h"
#include "list.h"
#include "cart.h"

#ifndef Clear_stdin
#define Clear_stdin while (getchar() != '\n');
#endif

#define PRODUCTS_PER_PAGE 20
#define POSITIVE_ONLY     true



// Input functions

//////////////////////////////// Datatypes /////////////////////////////////



/////////////////// Hidden (internal) function prototypes //////////////////


void controller_setProductName(storage_t *s, product_t *p);

void controller_setProductDescription(product_t *p);

void controller_setProductPrice(product_t *p);

void controller_setProductCount(product_t *p);

void controller_setProductShelf(storage_t *s, product_t *p);

void controller_editShelf(storage_t* s, product_t *p);

void controller_editNewAddProduct(storage_t* s, product_t *p);

void controller_addProduct(storage_t* s);

void controller_displayProduct(storage_iterator_t* it, int amountOfItems);

void controller_removeProduct(storage_iterator_t* it, storage_t* s, cart_t *c,int amountOfItems);

void controller_editProduct(storage_t* s,int amountOfItems);

void controller_undoAction(storage_t* s);

void controller_listAllProducts(storage_t* s, cart_t *c);

void controller_quitProgram (bool *quit);

bool controller_verifyRange(int index,int min,int max);

///////////////////////// Function Implementations /////////////////////////

char* controller_askQuestionString(char *q)
{
  puts(q);
  return input_readString(stdin, true);
}

int controller_askQuestionInt(char *q, bool positiveOnly)
{
  puts(q);
  return input_readIntWithMessage(stdin, VERIFY_INT, "Bad input! Please enter an integer: ");
}

char controller_readChar() {
  char *buf  = NULL;
  buf = input_readStringBuf(stdin, 2, buf, STRIP_NEWLINE);
  char input = buf[0];
  free(buf);
  return input;
}

char controller_readCharInRange(char* range) {
  char input = -2;
  do {
    if (input >= 0)
      {
        printf("Bad input '%c', please try again: \n", input);
      }
    input = controller_readChar();
  } while (strchr(range, input) == NULL || input == '\0');
  
  return input;
  
}

char controller_askQuestionChar(char* q, char* opts) {
  char input = -2;

  printf("%s [%s]\n", q, opts);
  do {
    if (input >= 0)
      {
        printf("Bad input '%c', please select one of these: [%s]\n", input, opts);
      }
    input = controller_readChar();
  } while (strchr(opts, input) == NULL || input == '\0');
  
  return input;
}

bool controller_askQuestionBool(char* q) {
  char answer = toupper(controller_askQuestionChar(q, "YyNn"));
  if(answer == 'Y') {
    return true;
  }
  return false;
}

char *controller_readNonEmptyString(char *q, char *errmsg) {
  char *str = NULL;
  do {
    if(str != NULL) {
      free(str);
      str = NULL;
      puts(errmsg);
    }
    str = controller_askQuestionString(q);
  }while(strlen(str) <= 0);

  return str;
}

void controller_removeFromCart(cart_t *c) {
  int choice = -1;
  int itemCount = cart_uniqueItems(c);

  if(itemCount == 0) {
    ui_alert("No products in cart!");
    return;
  }

  do{
    if(choice>=0){
      printf("Index out of range\n");
    }
    printf("Which product do you want to remove? (1-%d)\n", itemCount);
    choice = controller_askQuestionInt("", POSITIVE_ONLY);
  } while(controller_verifyRange(choice, 1, itemCount) == false);

  if(controller_askQuestionBool("Are you sure you want to remove the product?")) {
    cart_removeItem(c, choice-1);
    ui_alert("Removed product from cart!");
  } else {
    ui_alert("Cancelled removal from cart.");
  }
}

void controller_showCart(cart_t * c) {
  bool loop = true;
  do{
    cart_printContents(c);
    ui_printCartMenu();

    char choice = controller_askQuestionChar("What do you want to do?", "RrEe");    
    switch(toupper(choice)) {
    case 'R':
      // Remove product from cart
      controller_removeFromCart(c);
      break;
    case 'E':
      // exit
      loop = false;
      break;
    }    
  } while(loop);
}


// Main menu functions

char controller_askForMenuChoice(){
  char selected = controller_askQuestionChar("What do you want to do now?", "AaRrEeCcLlQq");
  selected = toupper(selected);
  return selected;
}

void controller_actOnMenuChoice(storage_t* s, char choice, bool *quit,cart_t *c) {
    switch(choice){
    case 'A':      
      controller_addProduct(s);
    break;

    case 'R':      
      //      removeProduct(s);
      controller_listAllProducts(s,c);
    break;  
    
    case 'E':
      //      editProduct(s);
      controller_listAllProducts(s,c);
    break;

    /* case 'U': */
    /*   controller_undoAction(s); */
    /* break; */

    case 'C':
      controller_showCart(c);
      break;
      
    case 'L':
      controller_listAllProducts(s,c);
      break;
      
    case 'Q':
      controller_quitProgram(quit);
      break;
    }  
}

//////////////// Hidden (internal) function implementations ////////////////


// Product functions

void controller_setProductName(storage_t *s, product_t *p) {
  char *name = controller_readNonEmptyString("Enter product name", "Product cannot have empty name.");
  product_t *existing = storage_find(s, name);
  if(existing != NULL && existing != p) {
    printf("Product with name '%s' already exists, name not changed", name);
    ui_alert("");
  } else {
    product_setName(p, name);
  } 
  free(name);
}

void controller_setProductDescription(product_t *p) {
  char *description = controller_askQuestionString("Enter product description (max 255 characters):");
  product_setDescription(p, description);
  free(description);
}

void controller_setProductPrice(product_t *p) {
  int price = controller_askQuestionInt("Enter product price:", POSITIVE_ONLY);
  product_setPrice(p, price);
}

void controller_addProductShelf(storage_t *s, product_t *p){

  int insertStatus = STORAGE_SHELF_TAKEN;
  bool abort = false;

  do {
    puts("Enter section (A-Z): ");
    char section = controller_readCharInRange("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz");    
    int shelf = controller_askQuestionInt("Enter shelf number: ", POSITIVE_ONLY);
    int count = controller_askQuestionInt("Enter amount of products on shelf: ", POSITIVE_ONLY);
    insertStatus = storage_addToShelf(s, p, section, shelf, count);

    if(insertStatus == STORAGE_SHELF_TAKEN){
      abort = !controller_askQuestionBool(
                 "Shelf taken, would you like to add to another shelf?"
              );
      if(abort) {
        ui_alert("Aborted adding shelf.");
      }
    }

  } while(insertStatus == STORAGE_SHELF_TAKEN && abort == false);
  
  if(insertStatus == STORAGE_UPDATED_SHELFCOUNT) {
    ui_alert("Product already on shelf, added to product count.");
  } else if (insertStatus == STORAGE_ADDED_SHELF){
    ui_alert("Product added to shelf");
  }
  
}

void controller_removeProductShelf(storage_t *s, product_t *p) { 
  puts("Enter section (A-Z): ");
  char section = controller_readCharInRange("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz");
  int shelf = controller_askQuestionInt("Enter shelf number: ", VERIFY_INT);

  if(storage_productOnShelf(p, section, shelf)) {
    char* productName = product_getName(p);
    printf("Do you want to remove '%s' from shelf '%c%d'?", productName, section, shelf);
    free(productName);
    if(controller_askQuestionBool("")) {
      if(storage_removeFromShelf(p, section, shelf)) {
        ui_alert("Product removed from shelf.");
      }
    } else {
      ui_alert("Aborted shelf removal.");
    }

  } else {
    ui_alert("Product not on shelf.");
  }
}

void controller_updateProductShelfCount(storage_t *s, product_t *p) {
  puts("Enter section (A-Z): ");
  char section = controller_readCharInRange("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz");
  int shelfNumber = controller_askQuestionInt("Enter shelf number: ", VERIFY_INT);
  int sIdx = storage_getShelfIndex(p, section, shelfNumber);
  if(sIdx > -1) {
    list_t *shList = product_getShelvesList(p);
    shelf_t *shelf = (shelf_t *)list_get(shList, sIdx);
    printf("Shelf %c%d\nCurrent count: %d\n",
           storage_getShelfSection(shelf),
           storage_getShelfNumber(shelf),
           storage_getShelfCount(shelf));

    int count = controller_askQuestionInt("Enter new count:", VERIFY_INT);
    if(controller_askQuestionBool("Are you sure you want to update shelf count?")) {
      storage_updateShelfCount(p, section, shelfNumber, count);
      ui_alert("Shelf count updated.");
      
    } else {
      ui_alert("Aborted shelf count update.");
    }

  } else {
    ui_alert("Product not on selected shelf.");
  }
}

void controller_editProductShelves(storage_t *s, product_t *p) {
  
  bool stopEdit = false;
  
  do {
    ui_printShelfEditMenu(p);
    char choice = controller_askQuestionChar("What do you want to do?", "AaRrUuEe");
    
    switch(toupper(choice)) {
    case 'A':
      // Add shelf
      puts("Enter shelf location (e.g. B13):");
      controller_addProductShelf(s,p);
      break;
    case 'R':
      // Remove shelf
      controller_removeProductShelf(s,p);
      break;
    case 'U':
      // Update product count
      controller_updateProductShelfCount(s,p);
      break;
    default:
      // Stop editing shelves
      stopEdit = true;
      break;
    }
  } while(stopEdit == false);
}

void controller_editNewAddProduct(storage_t* s, product_t *p) {
  bool loop = true;
  do {
    ui_printProduct(p);
    ui_editMenu();
    char choice = controller_askQuestionChar("What would you like to edit?", "NnDdPpSsEe");
    switch(toupper(choice)) {
    case 'N':
      puts("");
      char *name = product_getName(p);
      printf("Name: %s\n", name);
      free(name);
      puts("------------------------------------------"); 
      controller_setProductName(s,p);
      break;
    case 'D':
      puts("");
      char * desc = product_getDescription(p);
      printf("Description: %s\n", desc);
      free(desc);
      puts("------------------------------------------");
      controller_setProductDescription(p);
      break;
    case 'P':
      printf("Price: %d\n", product_getPrice(p));
      puts("------------------------------------------");
      controller_setProductPrice(p);
      break;
    case 'S':
      controller_editProductShelves(s,p);
      break;
    case 'E':
      loop = false;
      break;
    }
  } while (loop);
}

void controller_addProduct(storage_t* s) {
  storage_iterator_t* it = storage_iterator(s);
  product_t * p = product_new();

  // Product name
  char *name = controller_readNonEmptyString("Enter product name", "Product cannot have empty name.");
  product_t *existing = storage_find(s, name);
  if(existing != NULL) {
    puts("Product already in database.");
    ui_alert("Using existing price and description.");
    ui_printProduct(existing);
    controller_editProductShelves(s, existing);

    free(name);
    product_destroy(p);
    
  } else {
    product_setName(p, name);
    free(name);
    controller_setProductDescription(p);
    controller_setProductPrice(p);
    controller_editProductShelves(s, p);

    bool loop = true;
    do{
      char choice = toupper(controller_askQuestionChar("Do you want to save the product? (E for edit)", "YyNnEe"));
      if(choice == 'Y') {
        loop = false;
        // TODO : first run tree_find
        // ask if user wants to merge
        if(storage_insert(s, p)){
          ui_alert("Product has been saved!");        
        }
        else {
          ui_alert("Product couldn't be saved!");       
        }
        /* TODO:  Add history state push */
      } else if(choice == 'E') {

        ///* TODO:  custom edit*/
        controller_editNewAddProduct(s,p);
      } else {
        loop = false;
        storage_removefromAllShelves(p);
        product_destroy(p);
        ui_alert("Product discarded!");
      }
    } while (loop);
  }
  storage_freeIterator(it);
}

void controller_displayProduct(storage_iterator_t* it, int amountOfItems) {
  product_t *p = NULL;
  bool loop = true;
  do {
    
    int index = -1;
    do{
      if(index != -1){
	printf("Index out of range\n");
      }
      index = controller_askQuestionInt("Enter product index", POSITIVE_ONLY);  
    }while(controller_verifyRange(index,1,amountOfItems) == false);

    
    p = storage_findProductByIndex(it, index-1);
    
    if (p == NULL) {
      puts("Didn't find product");
      char choice = controller_askQuestionChar("Would you like to try again?","YyNn");
      if (toupper(choice) == 'N') {
	loop = false;
      }
     
    } else {
      loop = false;
      ui_printProduct(p); /* TODO:  Exchange for ui_printProduct */
    }
  } while (loop);
}


void controller_removeProduct(storage_iterator_t* it, storage_t* s, cart_t *c, int amountOfItems) {
  if(storage_totalProducts(s) == 0) {
    ui_alert("Storage is empty.");
    return;
  }

  product_t *p = NULL;
  bool loop = true;
  
  do {
    int index = -1;
    do{
      if(index != -1){
	printf("Index out of range.\n");
      }
      index = controller_askQuestionInt("Enter product index", POSITIVE_ONLY);  
    }while(controller_verifyRange(index,1,amountOfItems) == false);
    
    p = storage_findProductByIndex(it, index-1);
    
    if (p == NULL) {
      puts("Didn't find product");
      char choice = controller_askQuestionChar("Would you like to try again?","YyNn");
      if (toupper(choice) == 'N') {
	index = -1;
	loop = false;
      }
     
    } else {
      ui_printProduct(p); /* TODO:  Exchange for ui_printProduct */
      char choice = controller_askQuestionChar("Are you sure?", "YyNn");
      loop = false;
      if (toupper(choice) == 'Y') {
        /* TODO:  A history snapshot */
	char *key = product_getName(p);
	printf("%s , ",key);
	int indexInCart = cart_findProductIndex(c,p);
	if(indexInCart >=0){
	  cart_removeItem(c,indexInCart);
	}
	storage_remove(s, key);
	free(key);
	ui_alert("Product removed.");
      } else {
	ui_alert("Removal aborted.");
      }
    }
  } while (loop);
}

void controller_editProduct(storage_t* s, int amountOfItems) {
  
  if(storage_totalProducts(s) == false) {
    ui_alert("No products available to edit.");
    return;
  }

  product_t *p = NULL;
  bool loop = true;
  storage_iterator_t* it = storage_iterator(s);

  do {
    int index = -1;
    do{
      if(index != -1){
	printf("Index out of range\n");
      }
      index = controller_askQuestionInt("Enter product index", POSITIVE_ONLY);
    }while(controller_verifyRange(index,1,amountOfItems) == false);
        
    p = storage_findProductByIndex(it, index-1);
    
    if (p == NULL) {
      puts("Didn't find product");
      char choice = controller_askQuestionChar("Would you like to try again?","YyNn");
      if (toupper(choice) == 'N') {
	index = -1;
	loop = false;	
      }      
    } else {
      /* TODO:  Add history snapshot */
      do {
	ui_printProduct(p);
	ui_editMenu();
	char choice = controller_askQuestionChar("What would you like to edit?", "NnDdPpSsEe");
	switch(toupper(choice)) {
	case 'N':
	  puts("");
	  char * old = product_getName(p);
	  printf("Name: %s\n",old);
          puts("------------------------------------------");
	  controller_setProductName(s,p);
	  char *new =product_getName(p);
	  if(strcmp(old,new) != 0){
	    storage_reinsert(s,p,old);
	  }
	  free(old);
	  free(new);
	  break;
	case 'D':
	  puts("");
	  char * desc = product_getDescription(p);
	  printf("Description: %s\n", desc);
	  free(desc);
          puts("------------------------------------------");
	  controller_setProductDescription(p);	  
	  break;
	case 'P':
	  printf("Price: %d\n", product_getPrice(p));
          puts("------------------------------------------");
	  controller_setProductPrice(p);
	  break;
	case 'S':
          controller_editProductShelves(s,p);
	  break;
	case 'E':
	  loop = false;
	  break;
	}
      } while (loop);
    }
    
  } while (loop);
  storage_freeIterator(it);
}


bool controller_verifyRange(int choice, int min,int max){
  if(min<=choice && choice<=max){
    return true;
  } 
  return false;

}

void controller_addToCart(storage_t* s, cart_t * c, char ** listOfItems, int amountOfItems, int page){  
  cart_printContents(c);

  int choice = -1;
  do{
    if(choice>=0){
      printf("Index out of range\n");
    }
    choice = controller_askQuestionInt("\nWhich product do you want to add to the cart?", POSITIVE_ONLY);
  }while(controller_verifyRange(choice,1,amountOfItems) == false);

  char* key = listOfItems[choice-1];
  product_t *p = tree_findData(s,key);
  int inCart = cart_findProductsInCart(c,p);
  int amount = storage_getCount(p);
  if(amount == 0){
    printf("%s not in stock, please select another product\n",key);
    ui_alert("");
    return;
  }
  printf("Name: %s \nAlready in cart: %d \n", key, inCart);
  printf("Choose amount to add to the cart (0 - %d):\n", amount-inCart);
  int add = -1;
  do{
    if(add>=0){
	printf("%d available in stock, please insert correct amount\n",amount-inCart);
    }
    add = controller_askQuestionInt("", POSITIVE_ONLY);
  }while((add > amount-inCart));

  cart_addItem(c,p,add);
  printf("%d of product %s added to cart\n",add, key);
}

void controller_undoAction(storage_t* s) {
  /* TODO:  Use new functions */
  fprintf(stderr, "History is new, use new functionality instead (controller_undoAction)");
  exit(EXIT_FAILURE);
}

void controller_listAllProducts(storage_t* s, cart_t *c) {
  storage_iterator_t* it = storage_iterator(s);
  int amountOfItems = tree_size(s);
  int counter = 0;
  char *listOfItems[amountOfItems];
  while(storage_more(it) && counter < (amountOfItems)) {
    listOfItems[counter] = product_getName(storage_next(it));
    ++counter;
  }
  int page = 0;
  int lastPage = (amountOfItems / PRODUCTS_PER_PAGE); 
  bool menu = false;
  do {
    storage_listProducts(listOfItems, PRODUCTS_PER_PAGE, page, amountOfItems);
    char choice = controller_askQuestionChar("What do you want to do now?", "AaDdEeRrNnPpMm");
    switch (toupper(choice)) {
    case 'A':
      controller_addToCart(s,  c, listOfItems,amountOfItems,page );
      menu = true;
      break;
    case 'D':
      controller_displayProduct(it, amountOfItems);
      ui_alert("");
      menu = true;
      break;
    case 'E':
      controller_editProduct(s,amountOfItems);
      menu = true;
      break;
    case 'R':
      controller_removeProduct(it,s,c,amountOfItems);
      menu = true;
      break;
    case 'N':
      if(page<lastPage) {
	++page;
      } else {
	ui_alert("You're on the last page.");
      }
      break;
    case 'P':
      if (page > 0) {
	--page;
      } else {
	ui_alert("You're on the first page");
      }
      break;
    case 'M':
      menu = true;
      break;
    }
    //menu = true;
  } while (!menu);
  storage_freeList(listOfItems,amountOfItems);
  storage_freeIterator(it);
}

void controller_quitProgram (bool *quit) {
  char selected = controller_askQuestionChar("Do you really want to quit?", "yYnN");
  if (selected == 'y' || selected == 'Y'){
    puts("Good bye, see you soon!");
    *quit = true;
  }
}
