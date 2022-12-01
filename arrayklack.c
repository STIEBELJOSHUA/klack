#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arrayklack.h"

//macros
#define IS_IGNORE(ch) (ch==' ' || ch=='\n' || ch=='\0')
#define IS_NUM(ch) ((ch > 47 && ch < 58)||(ch=='.'))

//globals
//points to head of stack
static Token* stackhead = NULL;

//helper functions (put these in a header file later)

void printarray(Token* tk){
  int prints = tk->size/tk->rows;
  if(tk->matrix.d==NULL){
    printf("()\n");
    return;
  }
  printf("(");
  for(int i=0;i<(tk->size);i++){
    if((i)==prints)
      printf("\n ");
    double db = tk->matrix.d[i];
    if((int)db==db)
      printf("%d",(int)tk->matrix.d[i]);
    else
      printf("%.4f",tk->matrix.d[i]);
    if(i+1!=tk->size)
      printf(" ");
  }
  printf(")\n");
  
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
      tok->rows=1;
      tok->prev=stackhead;
      stackhead = tok;
      
      int headflag = 0;
      
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
          char* stri = malloc(len+1);
          char* endptr;
          stri[len]='\0';
          strncpy(stri,&line[start],len);
          
          double var = strtod(stri,&endptr);
          
          if(headflag==0){
            headflag=1;
            tok->type=FLOAT;
            tok->matrix.d=malloc(sizeof(double)*20);
            tok->matrix.d[tok->size] = var;
          }
          //TODO: this will break after 20 vars, make a append and insert[i] functions and use them here (and everywhere) so this shit wont break. the functions should take the token*, i should never deal with the array directly like i am here. also dont use malloc here, use calloc or something that zeros it out first. for the insert  function, if the array is full, just realloc before hand. simple stuff.
          tok->matrix.d[tok->size] = var;
          
          
          
          

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
      
      ch=line[++j];
      
    }
    
    else{
      fprintf(stderr, "%c is not a valid character!\n",ch);
      return;
    }
  }  
}




