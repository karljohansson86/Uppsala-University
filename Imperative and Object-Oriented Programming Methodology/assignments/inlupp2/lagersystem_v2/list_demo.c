#include <stdio.h>

#include "list.h"



int main(int argc, char *argv[])
{
  
  list_t* list = list_new();

  char* c[24] = {"Alpha","Beta","Gamma","Delta","Epsilon","Zeta","Eta","Theta","Iota","Kappa","Lambda","Mu","Nu","Xi","Omnicron","Pi","Rho","Sigma","Tau","Upsilon","Phi","Chi","Psi","Omega"};

  for(int i = 0; i < 24; ++i) {
    list_append(list, c[i]);
  }

  for(int i = 0; i < 24; ++i) {
    char* iter = list_get(list, i);
    char* rec = list_get(list, i); 
 
    printf("Iterative @ index %d: %s\n", i, iter);
    printf("Recursive @ index %d: %s\n\n", i, rec);
  }
  
  list_destroy(list);

  return 0;
}
