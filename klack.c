#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "klack.h"


  // for token:
  //   if token is number:
  //   allocate token int
  //   point head to token int
  //   repeat for other scalars
  //   if token in dict of builtins:
  //     execute that built in
  //   else:
  //     fail 

//helper functions (put these in a header file later)


//primitive implementations

void add(){
  printf("add test\n");
}

void sub(){
  printf("subtract test\n");
}

//primitave "dict"
void (*prim_func[1])() = {'\0'};

void init_prims(){
  prim_func[(int)'+']=add;
  prim_func[(int)'-']=sub;
}


//points to head of stack
static Token* stackhead = NULL;


//execute a chunk of code MAKE SURE ITS NULL TERMINATED
void execline(char* line){
  int i = strlen(line);
  int t = 0;
  
  for(int j=0;j<i;j++){
    char ch = line[j];
    char ch_look = line[j+1];
    
    //ignore spaces and newlines
    if(ch==' ' || ch=='\n'){
      continue;
    }
    //check for primitaves
    else if(((ch>32 && ch<48) || (ch>57 && ch<127)) && (ch_look==' ' || ch_look=='\n' || ch_look=='\0')){
      printf("test %i\n",(int)ch);
      (*prim_func[(int)ch])();
      t++;
      continue;
    }
    // else if(strchr(adverb_chars,ch)!=NULL){
    //   token_list[t].type=ADVERB;
    //   token_list[t].val.c = ch;
    //   t++;
    //   continue;
    // }
    else{
      fprintf(stderr, "%c is not a valid character!",ch);
      exit(1);
    }
  }  
}




