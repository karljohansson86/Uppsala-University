#ifndef __UNDO_h
#define __UNDO_h

//#include "warehousesystem.h"


typedef struct action action;

//void add_action(int type, int pos, struct goods *goods_copy);
void add_action(int type, int pos, char * name, char * description, int price, char row, int number, int in_stock);
void add_item_undo();
void edit_item_undo();
void undo_action(); 
void undo_edit();


#endif
