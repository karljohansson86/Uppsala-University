#include "warehousesystem.h" //headerfile
//#include "undo.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define Clear_stdin while (getchar() != '\n');

int run_once = 0;
char status = '0';



int edit_item()
{
  //last_action->type = 3; //sätter typen i undo struct till 3 för edit
  char edit_response = 0;
  list_inventory(inventory);
  int choice = get_int("Which product do you want to edit?\n");
  //last_action->pos = choice-1;
  //last_action->goods_copy = inventory[choice-1];
  add_action(3, choice-1, inventory[choice-1].name,  inventory[choice-1].description,  inventory[choice-1].price, inventory[choice-1].shelf.row, inventory[choice-1].shelf.number, inventory[choice-1].in_stock);//sätter typen i undo struct till 3 för edit
  printf("\n Choose what to edit \n [N]ame \n [D]escription \n [P]rice \n [S]helf \n [I]n stock\n \n");
  edit_response = get_char("Please enter a character", "NDPSIQ");

   switch(edit_response)
	{
	case 'N' :
          printf("\nName is: %s\n", inventory[choice-1].name);
          puts("--------------------");
          get_string("Change to: ", inventory[choice-1].name);
          printf ("name changed to %s \n", inventory[choice-1].name);
	  break;

	case 'D' :
          printf("\nDescription is: %s\n", inventory[choice-1].description);
          puts("--------------------");
          get_string("Change to: ", inventory[choice-1].description);
          printf ("Name changed to %s \n", inventory[choice-1].description);
          break;

	case 'P' :
          printf("\nPrice is: %d\n", inventory[choice-1].price) ;
          puts("-------------------");
          inventory[choice -1].price = get_int("Change to: ");
          printf ("The price of %s is now %d\n",inventory[choice -1].name ,
                  inventory[choice -1].price);
	  break;

	case 'S' :
          printf("\nItem shelf: %c%d\n", inventory[choice-1].shelf.row,
               inventory[choice-1].shelf.number);
          puts("--------------------");
          inventory[choice -1].shelf = add_shelf();
          printf ("\nShelf for item: %s changed to %c%d\n",inventory[choice -1].name ,
                  inventory[choice -1].shelf.row, inventory[choice -1].shelf.number);
	  break;
	case 'I' :
          printf("\nIn stock now: %d\n", inventory[choice-1].in_stock) ;
          puts("-------------------");
          inventory[choice -1].in_stock = get_int("Change to: ");
          printf ("%s in stock changed to %d\n",inventory[choice -1].name ,
                  inventory[choice -1].in_stock);
	  break;
	}
   edit_response = 0;
   
   return 0;
}


int next_empty_spot_in_inventory()
{
    char a[20];
    strcpy(a, "empty");
    for(int i = 0; i<INVENTORY_SIZE; ++i)
    {
        if(strcmp (a,inventory[i].name) == 0)// om lika returnera i
        {
            return i;
        }
    }
    return 0; //??? om den är full
}


//Function used to add shelf for items
shelf add_shelf()
{
    shelf tmp_shelf;// deklarerar en temporär shelf
    tmp_shelf.row = get_char("Change to row", "ABCDEF");
    tmp_shelf.number = get_int("Change row number to:");
    if(!valid_shelf (tmp_shelf))//kollar ifall positionen är upptagen
    {
        do
        {
            puts("\nShelf is already occupied, please insert another shelf: ");
            tmp_shelf.row = get_char("Change to row", "ABCDEF");
            tmp_shelf.number = get_int("Change row number to: ");
        }while(!valid_shelf (tmp_shelf));
    }
    return tmp_shelf;
}

//Aux functions for add_shelf to check if user input is valid
bool valid_shelf(shelf shelf)
{
    for(int i = 0; i<INVENTORY_SIZE; ++i)
    {
      if(shelf.row == inventory[i].shelf.row)//kollar om "i" ligger på samma row
        {
          if(inventory[i].shelf.number == shelf.number)//kollar ifall samma nr
            {
              return false;
            }
        }
    }
  return true;
}


void add_item()
{
  //last_action->type = 1;
  int next = next_empty_spot_in_inventory();
  //last_action->pos = next;
  get_string("Enter name:", inventory[next].name);
  get_string("Enter description: ", inventory[next].description);

  inventory[next].price = get_int("Enter Price:");
  inventory[next].shelf = add_shelf();
  inventory[next].in_stock = get_int("Enter amount in stock:");
  //last_action->goods_copy = inventory[next];
  add_action(1, next, inventory[next].name,  inventory[next].description,  inventory[next].price, inventory[next].shelf.row, inventory[next].shelf.number, inventory[next].in_stock);//type = 1 for add
  puts("Added item to inventory:");
  print_item(inventory, next);

 
}

/*
void add_item_undo(action *last_action)
{

  int next = last_action->pos;
  inventory[next] = last_action->goods_copy;
  //get_string("Enter description: ", inventory[next].description);

  //inventory[next].price = get_int("Enter Price:");
  //inventory[next].shelf = add_shelf();
  //inventory[next].in_stock = get_int("Enter amount in stock:");
  last_action->type = 0;

}*/

void remove_item()
{
    list_inventory(inventory);
    //int last_item = next_empty_spot_in_inventory();
    // char * last_item_str = (, &last_item); 
    int answer_delete = get_int( "Choose 1-20 to delete:");
    print_item(inventory, answer_delete-1);
    char yes_or_no = get_char("Are you sure you want to delete? yes/no", "YN");
    //last_action->type = 2;
    //last_action->pos = answer_delete-1;
    //last_action->goods_copy = inventory[answer_delete-1];
    add_action(2, answer_delete-1,inventory[answer_delete-1].name,  inventory[answer_delete-1].description,  inventory[answer_delete-1].price, inventory[answer_delete-1].shelf.row, inventory[answer_delete-1].shelf.number, inventory[answer_delete-1].in_stock);
    if (yes_or_no == 'Y')
    {
        printf("%s deleted \n", inventory[answer_delete-1].name );
        inventory[answer_delete-1] =
        (goods){
            .name = "empty",
            .description = "_",
            .price = 0,
            .shelf.row = '_',
            .shelf.number = 0,
            .in_stock = 0 };

        update_inventory();
    }
    else
    {
        printf("%s was not deleted \n", inventory[answer_delete-1].name );
    }

}



//Updates the inventory list so all item in the array are located after each other and the 
//empty elements are in the end
void update_inventory()
{
  char a[20];
  char b[20];
  char c[20];
  strcpy(a, "empty");// a = empty

  for(int n = 0; n < INVENTORY_SIZE-1; ++n)
    {
      strcpy(b, inventory[n].name); // b lika med namn på innehall i pos
      if (strcmp (a,b) == 0)// om b = empty
        {
          strcpy(c, inventory[n+1].name);//c lika med namn på posn+1
          if (strcmp (c, a) != 0)//om c inte lika med empty 
            {
              inventory[n] = inventory[n+1];
              strcpy(inventory[n+1].name, a); // lägger in empty i nästa pos

            }
        }
    }
}

//
void user_choice(char response)
{
    switch(response)
    {
        case 'A' :
            add_item();

            break;
        case 'R' :
            remove_item();
            break;
        case 'E' :
            edit_item();
            break;
        case 'L' :
            printf("Inventory below: \n");
            list_inventory(inventory);
            break;
        case 'D' :
            printf("Inventory below: \n");
            display_inventory(inventory);
            break;
        case 'U' :
	    undo_action();
            break;
        case 'Q' :
            printf("You chose to quit\n");
            status = 'Q';
            break;
    }

}




//FIX ME, ändra så att funktion även tar en int som är limit för antal bokstäver
void get_string(char *q, char *answer)
{
    printf("%s\n",q);
    scanf("%[^\n]", answer); // max 20 tecken 20s ||
    Clear_stdin;
}

int get_int(char *q)
{
    char str[20];
    bool only_int = false;
    //char *tmp;-----

    do {
        printf("%s\n",q);
        scanf("%[^\n]", str); // läser input
        
        int length = strlen(str);

        for(int i = 0; i < length; ++i)//loopar igenom strängen och kollar om det bara är nummer
            {
                if (!isdigit(str[i]) || length >= 20)
                    { only_int = false; }
                else
                    { only_int = true;  }
            }
        if (!only_int || atoi(str) < 0) { puts("Bad inut, try again."); }
        Clear_stdin;
    } while (!only_int || atoi(str) < 0);// ANDRA TILL 1?

    return atoi(str); //gör om stringen till en long och returnerar, OK MED INT?
}


char get_char(char *q, char *alt)
{
  char input = 0;
  printf("%s [%s]\n", q,alt);
  do {
    if (input)
      {
        printf("Bad input '%c', try again [%s]\n", input, alt);
        Clear_stdin;
      }
    input = toupper(getchar());
  } while( strchr(alt, input) == NULL);
  Clear_stdin;
  return input;
}


//Lists the inventory, 20 items per page
//FIX ME, måste visa 1-20 oavsett sida
void list_inventory(goods *inventory)
{
  int counter = 0;
  char a[20];
  strcpy(a, "empty");
  int start = 0;
  int end = 20;
  int loop = 1;
  char page;
  int i;

  while (loop == 1)
    {
      //int n = 0;
      for(i = start; i < end; i++)
	{
	  if (strcmp (a,inventory[i].name) != 0) //kollar om namnet på varan inte är "empty"
	    {
	      printf("%d.\t %s\n", i+1 , inventory[i].name);
	      ++ counter;// rakna antalet olika varor/positioner
            }
	}

      if(counter == 0)
      	{ puts("\nNo articles in the inventory");}

      page = get_char("Choose [P]revious, [N]ext page or [D]one", "PND");

      if (page == 'N')
	  {
	    int stop = next_empty_spot_in_inventory();
	    if (end > stop){
	      puts ("No more item, can't go forward");
	    }
	    else
	      {
	    start += 20;
	    end   += 20;
	      }
	    }

      if (page == 'P')
	{
	if (start == 0)
	  {
	    puts ("Can't go back");
	  }
	else
	  {
	   start -= 20;
	   end   -= 20;
	  }
	}

      if (page == 'D')
      	  {
      	    loop = 0;
      	  }
    }
  //return 0;

}

//A recursive function only used for demonstration of one goal
void list_inventory_rec(goods *inventory)
{
    char a[20];
    strcpy(a, "empty");
    list_inventory_rec_aux(0, a);

}

//Aux function for list_inventory_rec
void list_inventory_rec_aux(int counter, char *a)
{
    //if (counter < 0)
    //return 0; error?
    if(counter < INVENTORY_SIZE)
    {
        if (strcmp (a,inventory[counter].name) != 0) //kollar om namnet på varan inte är "empty"
        {
            printf("%d.\t %s\n", counter+1 , inventory[counter].name);
        }
        list_inventory_rec_aux(counter+1 , a);

    }

}


//Used to list all items in the inventory and choose to display/view on of them
int display_inventory(goods *inventory) /// display inventory
{
    list_inventory(inventory);
    int choice = get_int("Which product do you want to look at?\n");
    print_item(inventory , choice-1);
    return 0;
}

//Prints all information of one item in the inventory
void print_item(goods *inventory, int i)
{
    printf("Name: %s \nDescription: %s \nPrice: %d\nShelf: %c%d\nIn stock: %d\n",
           inventory[i].name, inventory[i].description, inventory[i].price,
           inventory[i].shelf.row, inventory[i].shelf.number, inventory[i].in_stock);
}


//Function that adds 20 items in the inventory, used for testing during the development
void add5_items(goods *inventory )
{
    inventory[0] = (goods) {.name = "Nudlar", .description = "mat", .price = 5,
        .shelf.row = 'A', .shelf.number = 1 , .in_stock = 99};
    inventory[1] = (goods) {.name = "Billys", .description = "frysmat", .price = 13,
        .shelf.row = 'A', .shelf.number = 2, .in_stock = 1};
    inventory[2] = (goods) {.name = "Gorbys", .description = "frysmat", .price = 10,
        .shelf.row = 'B', .shelf.number = 1, .in_stock = 2};
    inventory[3] = (goods) {.name = "Thaimat", .description = "asiatiskmat", .price = 65,
        .shelf.row = 'C', .shelf.number = 1, .in_stock = 3};
    inventory[4] = (goods) {.name = "Kebab", .description = "fyllemat", .price = 80,
        .shelf.row = 'C', .shelf.number = 2, .in_stock = 1};
    inventory[5] = (goods) {.name = "Nudlar", .description = "mat", .price = 5,
        .shelf.row = 'A', .shelf.number = 1 , .in_stock = 0};
    inventory[6] = (goods) {.name = "Billys", .description = "frysmat", .price = 13,
        .shelf.row = 'A', .shelf.number = 2, .in_stock = 1};
    inventory[7] = (goods) {.name = "Gorbys", .description = "frysmat", .price = 10,
        .shelf.row = 'B', .shelf.number = 1, .in_stock = 2};
    inventory[8] = (goods) {.name = "Thaimat", .description = "asiatiskmat", .price = 65,
        .shelf.row = 'C', .shelf.number = 1, .in_stock = 3};
    inventory[9] = (goods) {.name = "Kebab", .description = "fyllemat", .price = 80,
        .shelf.row = 'C', .shelf.number = 2, .in_stock = 2};
    inventory[10] = (goods) {.name = "Nudlar", .description = "mat", .price = 5,
        .shelf.row = 'A', .shelf.number = 1 , .in_stock = 0};
    inventory[11] = (goods) {.name = "Billys", .description = "frysmat", .price = 13,
        .shelf.row = 'A', .shelf.number = 2, .in_stock = 1};
    inventory[12] = (goods) {.name = "Gorbys", .description = "frysmat", .price = 10,
        .shelf.row = 'B', .shelf.number = 1, .in_stock = 2};
    inventory[13] = (goods) {.name = "Thaimat", .description = "asiatiskmat", .price = 65,
        .shelf.row = 'C', .shelf.number = 1, .in_stock = 3};
    inventory[14] = (goods) {.name = "Kebab", .description = "fyllemat", .price = 80,
        .shelf.row = 'C', .shelf.number = 2, .in_stock = 3};
    inventory[15] = (goods) {.name = "Nudlar", .description = "mat", .price = 5,
        .shelf.row = 'A', .shelf.number = 1 , .in_stock = 0};
    inventory[16] = (goods) {.name = "Billys", .description = "frysmat", .price = 13,
        .shelf.row = 'A', .shelf.number = 2, .in_stock = 1};
    inventory[17] = (goods) {.name = "Gorbys", .description = "frysmat", .price = 10,
        .shelf.row = 'B', .shelf.number = 1, .in_stock = 2};
    inventory[18] = (goods) {.name = "Thaimat", .description = "asiatiskmat", .price = 65,
        .shelf.row = 'C', .shelf.number = 1, .in_stock = 3};
    inventory[19] = (goods) {.name = "Kebab", .description = "fyllemat", .price = 80,
        .shelf.row = 'C', .shelf.number = 2, .in_stock = 4};
    inventory[20] = (goods) {.name = "Nudlar", .description = "mat", .price = 5,
        .shelf.row = 'A', .shelf.number = 1 , .in_stock = 0};
    inventory[21] = (goods) {.name = "Billys", .description = "frysmat", .price = 13,
        .shelf.row = 'A', .shelf.number = 2, .in_stock = 1};
    inventory[22] = (goods) {.name = "Gorbys", .description = "frysmat", .price = 10,
        .shelf.row = 'B', .shelf.number = 1, .in_stock = 2};
    inventory[23] = (goods) {.name = "Thaimat", .description = "asiatiskmat", .price = 65,
        .shelf.row = 'C', .shelf.number = 1, .in_stock = 3};
    inventory[24] = (goods) {.name = "Kebab", .description = "fyllemat", .price = 80,
        .shelf.row = 'C', .shelf.number = 2, .in_stock = 5};
}


//Prints start menu
void print_menu()
{
    puts("\nWelcome to WarehouseSystem 1.0");
    puts("==============================\n");
    puts("[A]dd article");
    puts("[R]emove article");
    puts("[E]dit article");
    puts("[L]ist inventory");
    puts("[D]isplay inventory");
    puts("[U]ndo previous action");
    puts("[Q]uit\n");
}


void run_start ()
{

    if(run_once == 0)
    {
        char src[20];
        //set name of in array to NULL to so arrays will be traceable..
        for(int i = 0; i < INVENTORY_SIZE; ++i)
        {
            strcpy(src, "empty");
            strcpy(inventory[i].name, src);
        }
        add5_items(inventory);
        ++run_once;

    }
}

void main_menu()
{

  run_start(run_once);
  print_menu();
  char* option_list = "ARELDUQ";
  char* start_menu_question = "What do you want to do?";
  char response = get_char(start_menu_question, option_list);
  user_choice(response);

  if (status != 'Q')
    {
      main_menu();
    }

}

int main()
{
  //action *last_action = malloc(sizeof(action));      //->type = 0;// .pos = 0};

  main_menu();
  
  return 0;
}
