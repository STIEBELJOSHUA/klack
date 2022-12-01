#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "klack.h"

//macros
#define IS_IGNORE(ch) (ch==' ' || ch=='\n' || ch=='\0')
#define IS_NUM(ch) ((ch > 47 && ch < 58)||(ch=='.'))

//globals
//points to head of stack
static Token* stackhead = NULL;

//helper functions (put these in a header file later)

void printarray(Token* tk){
  if(tk->scalhead==NULL){
    printf("()\n");
    return;
  }
  Scalar* scl = tk->scalhead;
  while(scl->next!=NULL){
    printf("%.2f ",scl->val.d);
    scl = scl->next;
  }
  printf("%.2f\n",scl->val.d);
  
}

//primitive implementations

void add(){
  printf("add test\n");
}

void sub(){
  printf("subtract test\n");
}

void printstack(){
  Token* headcopy = stackhead;
  if(headcopy!=NULL){
    while(headcopy->prev!=NULL){
      printarray(headcopy);
      headcopy = headcopy->prev;
    }
    printarray(headcopy);
  }
}

//primitave "dict"
char prims[] = "+-p";
void (*prim_func[128])() = {'\0'};

void init_prims(){
  prim_func[(int)'+']=add;
  prim_func[(int)'-']=sub;
  prim_func[(int)'p']=printstack;
}


//execute a chunk of code MAKE SURE ITS NULL TERMINATED
void execline(char* line){
  int i = strlen(line);
  int t = 0;
  
  for(int j=0;j<i;j++){
    char ch = line[j];
    
    //ignore spaces and newlines
    if(IS_IGNORE(ch)){
      continue;
    }
    //check for primitaves
    else if(strchr(prims,ch)){
      (*prim_func[(int)ch])();
      t++;
      continue;
    }
    //array parser
    else if(ch=='('){
      
      Token* tok = malloc(sizeof(Token));
      tok->size=0;
      tok->prev=stackhead;
      stackhead = tok;
      int headflag = 0;
      Scalar* heady = NULL;
      Scalar* taily = NULL;

      
      
      while(ch!=')'){
        ch=line[++j];
        
        if(IS_NUM(ch)){
          int start = j;
          int float_flag = 0;
          while(IS_NUM(ch)){
            if(ch=='.'){
              if(float_flag==1){
                fprintf(stderr, "%c is not a valid character!\n",ch);
                return;
              }
              float_flag=1;
            }
            ch=line[++j];
          }
          int len = j-start;
          printf("IS FLOAT: %i, LENGTH: %i\n",float_flag,len);
          char* stri = malloc(len+1);
          char* endptr;
          stri[len]='\0';
          strncpy(stri,&line[start],len);
          Scalar* scal = malloc(sizeof(Scalar));
          scal->type=FLOAT;
          scal->val.d=strtod(stri,&endptr);
        
          
          if(headflag == 0){
            heady=scal;
            headflag++;
          }
          else{
            taily->next=scal;
          }
          taily = scal;
          scal->next=NULL;
          tok->size++;
          j--;
        }
        
        
        else if(ch=='\''){
          j++;
          ch = line[j];
          printf("IS CHAR: %c\n",ch);
        }
        
        
        else if(ch=='"'){
          int start=j;
          ch=line[++j];
          while(ch!='"'){
            if(ch=='\0'){
              fprintf(stderr,"missing ending \"\n");
              return;
            }
            ch=line[++j];
          }
          int len = j-start;
          printf("IS STRING OF LENGTH: %i\n",len);
        }
      }
      
      tok->scalhead=heady;
      ch=line[++j];
      
    }
    
    else{
      fprintf(stderr, "%c is not a valid character!\n",ch);
      return;
    }
  }  
}




