#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


char *reverse_string(char *str2){
  if(str2 == NULL){
    return NULL;
  }
  int len = strlen(str2);
  char *rev = calloc(len+1,sizeof(char));

  for(int i = 0; i<len;++i){
    rev[i] = str2[len-1-i];
  }
  rev[len] = '\0';
  return rev;
}


bool strcmp_reverse(char *str1, char* str2){
  

  if(strlen(str1) != strlen(str2)){
    return false;
  }
  char *new = reverse_string(str2);
  int val = strcmp(str1,new);
  free(new);
  if(val == 0){
    return true;
  } else{
    return false;
  }
}


int main(int argc , char *argv[]){
  if(argc<3){
    puts("för få argument");
    return 0;
  }
  
  bool val = strcmp_reverse(argv[1],argv[2]);
  if(val){
    puts("SAMMA");
  } else{
    puts("Inte samma");
  }

  return 0;
}
