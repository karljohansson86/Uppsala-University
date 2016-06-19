#ifndef __warehousesystem_h
#define __warehousesystem_h
#include <ctype.h>
#include <stdbool.h>

#include "undo.h"


//Header file for warehousesystem.c
//


//typedef struct action action;

typedef struct shelf shelf;
struct shelf
{
    char row;
    int number;
};

typedef struct goods goods;
struct goods
{
  char name[20];
  char description[100];
  int price;
  shelf shelf;
  int in_stock;
};


#define INVENTORY_SIZE 100

goods inventory[INVENTORY_SIZE];


//int number_of_elements_in_array = 0;
//int run_once = 0;
//char ask_string[100];
//int next_position = 5;
//char status = '0';

int check_if_correct_input_int(char * cmp);
int edit_item();

int next_empty_spot_in_inventory();
shelf add_shelf();
bool valid_shelf(shelf shelf);

void add_item();
void remove_item();

void update_inventory();

void user_choice(char response);
void get_string(char *q, char *answear);
int get_int(char *q);
char get_char(char *q, char *alt);

void list_inventory(goods *inventory);
void list_inventory_rec_aux(int counter, char *a);
void list_inventory_rec();

int display_inventory(goods *inventory);
void print_item(goods *inventory, int i);
void add5_items(goods *inventory);
void print_menu();
void run_start ();
void main_menu();//action *last_action
int main();




#endif
