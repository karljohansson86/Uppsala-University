// Här skall du skriva koden 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *concatenate_string(char *str1, char *str2){
  int len1 = strlen(str1);
  int len2 = strlen(str2);
  
  char *new = calloc(len1+len2+1,sizeof(char));
  
  for(int i = 0; i<len1;++i){
    new[i]=str1[i];
  }
  int l = 0;
  for(int k = len1;k<(len1+len2);++k){
    new[k]=str2[l];
    ++l;
  }
  
  return new;
}

bool strcmp_concat(char *str1, char *str2, char *str3){
  
  char *new = concatenate_string(str1,str2);
  int val = strcmp(new,str3);
  free(new);
  if(val == 0){
    return true;
  } else{
    return false;
  }
}

int main(int argc, char *argv[]){

  if(argc < 4){
    puts("för få argument");
    return 0;
  }

  bool val = strcmp_concat(argv[1],argv[2],argv[3]);
  if(val){
    puts("SAMMA");
    return 0;
  }else{
    puts("inte samma");
    return 0;
  }

  return 0;
}
