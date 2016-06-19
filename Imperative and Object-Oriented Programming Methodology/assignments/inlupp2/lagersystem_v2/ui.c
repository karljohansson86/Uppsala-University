#include "ui.h"

//////////////////////////////// Datatypes /////////////////////////////////



/////////////////// Hidden (internal) function prototypes //////////////////



///////////////////////// Function Implementations /////////////////////////

void ui_alert(char * message) {
  puts(message);
  puts("Press [ENTER] to continue");
  Clear_stdin
}

void ui_splashScreen() {
  FILE* splash = fopen("splash.txt", "r");
  char buffer[128];
  while(fgets(buffer, 128, splash)) {
    printf("%s", buffer);
  }
  fclose(splash);
  ui_alert("");
}

void ui_mainMenu()
{

  puts("");
  puts("\n******************************");
  puts("MAIN MENU\n");
  puts("[A]dd product");
  puts("[R]emove product");
  puts("[E]dit product");
  puts("Show [C]art");
  //puts("[U]ndo last action");
  puts("[L]ist all products");
  puts("[Q]uit");
  puts("");

}

void ui_editMenu() {

  puts("");
  puts("[N]ame");
  puts("[D]escription");
  puts("[P]rice");
  puts("[S]helves");
  puts("[E]xit");
  puts("");

}

void ui_listOptions() {

  puts("");
  puts("[D]isplay product");
  puts("[E]dit product");
  puts("[R]emove product");
  puts("[A]dd product to cart");
  puts("[N]ext page");
  puts("[P]revious page");
  puts("[M]enu");
  puts("");

}

void ui_printShelfEditMenu(product_t *p) {
  puts("");
  ui_printShelves(p);
  puts("");
  puts("[A]dd to shelf");
  puts("[R]emove from shelf");
  puts("[U]pdate product count on shelf");
  puts("[E]xit shelf editing");
  puts("");
}

void ui_printCartMenu() {
  puts("");
  puts("[R]emove product");
  puts("[E]xit to menu");
  puts("");
}

void ui_printShelves(product_t *p) {
  list_iterator_t *it = list_iterator(product_getShelvesList(p));
  if(it != NULL) {
    puts("Shelf locations:");
    while(list_more(it)) {
      shelf_t *shelf = (shelf_t *)list_next(it);
      printf("   %c%d\tAvailable: %d\n", 
             storage_getShelfSection(shelf), 
             storage_getShelfNumber(shelf), 
             storage_getShelfCount(shelf));
    }
    list_freeIterator(it);
  }
}


void ui_printProduct(product_t *p) {
  
  puts("===================================================");
  char *name = product_getName(p);
  printf("Name:\t\t%s\n", name);
  if(name != NULL){
    free(name);
  }
  printf("Price:\t\t%d\n", product_getPrice(p));
  puts("");
  ui_printShelves(p);
  puts("");
  puts("Description");
  puts("---------------------------------------------------");
  char *desc = product_getDescription(p);
  printf("%s\n", desc);
  if(desc != NULL){
    free(desc);
  }
  puts("===================================================");
  
}

void ui_printListItem(char * item, int index){
  printf("%d: %s\n", index ,item);
}

void ui_printListHeader(int page){
  printf("Product list page(%d)\n", page);
  puts("===================================================");
}

void ui_printListFoot(){
  puts("===================================================\n");
}

//////////////// Hidden (internal) function implementations ////////////////

