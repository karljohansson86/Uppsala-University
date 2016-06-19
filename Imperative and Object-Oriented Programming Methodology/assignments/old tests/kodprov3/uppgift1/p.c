// Här skall du skriva koden 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char *reverse_string(char *str){
  int len = strlen(str);
  char *new = calloc(len+1,sizeof(char));//malloc(sizeof(char)*(len+1));
  for(int i = 0; i < len; ++i){
    new[i] = str[len-i-1];
  }
  return new;
}

bool strcmp_reverse(char *str1, char *str2){
  
  char *new = reverse_string(str2);
  int val = strcmp(new,str1); 
  free(new);
  if(val == 0){
    return true;
  }
  else{
    return false;
  }

}

int main(int argc, char* argv[]){
  if(argc<3){
    puts("för få argument");
    return 0;
  }
  int str1 = strlen(argv[1]);
  int str2 = strlen(argv[2]);
  if(str1 != str2){
    puts("Inte samma");
    return 0;
  }
  bool result = strcmp_reverse(argv[1],argv[2]);
  if(result){
    puts("SAMMA");
  }
  else{
    puts("inte samma");
  }
  
  return 0;
}
