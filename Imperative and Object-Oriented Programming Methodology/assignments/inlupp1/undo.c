#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "undo.h"
#include "warehousesystem.h"

struct action
{
  int type; //NOTHING = 0, ADD = 1, REMOVE = 2, EDIT = 3
  int pos; //position i inventory array
  struct goods goods_copy; //tidigare item
};
//typedef struct action action;


action last_action;

void add_action(int type, int pos, char * name, char * description, int price, char row, int number, int in_stock)
//void add_action(int type, int pos, struct goods *goods_copy)
{
  last_action.type = type;
  last_action.pos = pos;
  strcpy(last_action.goods_copy.name, name);
  strcpy(last_action.goods_copy.description,  description);
  last_action.goods_copy.price = price;
  last_action.goods_copy.shelf.row = row;
  last_action.goods_copy.shelf.number = number;
  last_action.goods_copy.in_stock = in_stock;


}


//Function that undo the last edit_item
void undo_edit()
{
  int pos = last_action.pos;
  inventory[pos] = last_action.goods_copy;
  
}


//Function that undo the last add_item 
void undo_add()
{
  int answer_delete = last_action.pos;
  print_item(inventory, answer_delete);
  char yes_or_no = get_char("Are you sure you want to delete? yes/no", "YN");
  if (yes_or_no == 'Y')
    {
        printf("%s deleted \n", inventory[answer_delete].name );
        inventory[answer_delete] =
        (goods){
            .name = "empty",
            .description = "_",
            .price = 0,
            .shelf.row = '_',
            .shelf.number = 0,
            .in_stock = 0 };

        //update_inventory();
        //last_action.type = 0;
    }
}


//function that undo the last remove_item
void undo_remove()
{
  int pos = last_action.pos;
  int i = INVENTORY_SIZE;
  for(int n = i; n >= pos ; --n) // för att slippa varningen...
    {

      if(n == pos)
        {
          inventory[n+1] = inventory[n];
          inventory[n] = last_action.goods_copy;
        }
      else if(n < INVENTORY_SIZE)
        {
          inventory[n+1] = inventory[n];
        }

    }
}

//Function that controls which undo function to be called
void undo_action()
{
  if(last_action.type == 0)//Nothing to undo
    {
      //printf("%d :\n", last_action.type);
      puts ("Nothing to undo");
    }
  if(last_action.type == 1)//last_action was an ADD
    {
      undo_add(last_action);
    }
  if(last_action.type == 2)//last action was an REMOVE
    {
      undo_remove(last_action);
    }
  if(last_action.type == 3)//last action was an EDIT
    {
      undo_edit(last_action);
    }
  last_action.type = 0;

}




