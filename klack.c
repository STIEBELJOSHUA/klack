#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "klack.h"

//macros
#define IS_IGNORE(ch) (ch==' ' || ch=='\n' || ch=='\0')
#define IS_NUM(ch) ((ch > 47 && ch < 58)||(ch=='.')||(ch=='-'))

//globals
//points to head of stack
static Token* stackhead = NULL;

//helper functions

//append to double array
void darrayappend(Token* tk, double dub){
  if(tk->size == tk->len){
    double* temp = realloc(tk->matrix.d,(tk->size*2)*sizeof(double));
    tk->matrix.d = temp;
    tk->size=tk->size*2;
  }
  tk->matrix.d[tk->len] = dub;
  tk->len++;
}


void printarray(Token* tk){
  int prints = tk->len/tk->rows;
  if(tk->matrix.d==NULL){
    printf("()\n");
    return;
  }
  printf("(");
  for(int i=0;i<(tk->len);i++){
    if((i)==prints)
      printf("\n ");
    double db = tk->matrix.d[i];
    if((int)db==db)
      printf("%d",(int)tk->matrix.d[i]);
    else
      printf("%.4f",tk->matrix.d[i]);
    if(i+1!=tk->len)
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
void (*prim_func[128])();

void init_prims(){
  prim_func[(int)'+']=add;
  prim_func[(int)'-']=sub;
  prim_func[(int)'p']=printstack;
}

//defined function "dict"
char* def_funcs[128];
//defined variables "dict"
char* def_vars[128];
//execute a chunk of code MAKE SURE ITS NULL TERMINATED
void execline(char* line){
  int i = strlen(line);
  
  for(int j=0;j<i;j++){
    char ch = line[j];
    
    //ignore spaces and newlines
    if(IS_IGNORE(ch)){
      continue;
    }
    //check for defined function
    else if(ch=='_'){
      ch=line[++j];
      execline(def_funcs[(int)ch]);
    }
    //check for defined variables
    else if(ch=='~'){
      ch=line[++j];
      execline(def_vars[(int)ch]);
    }
    //check for primitaves
    else if(strchr(prims,ch)){
      (*prim_func[(int)ch])();
    }
    //array parser
    else if(ch=='('){
      
      Token* tok = malloc(sizeof(Token));
      tok->len=0;
      tok->rows=1;
      tok->prev=stackhead;
      stackhead = tok;
      
      int headflag = 0;
      
      while(ch!=')'){
        ch=line[++j];
        
        if(IS_NUM(ch)){
          int start = j;
          while(IS_NUM(ch)){
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
            tok->matrix.d=malloc(sizeof(double)*10);
            tok->size=10;
            tok->matrix.d[tok->len] = var;
          }
          darrayappend(tok,var);
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
    
    //define function
    else if(ch==':'){
          int start=j;
          ch=line[++j];
          char name = ch;
          ch=line[++j];
          while(ch!=':'){
            if(ch=='\0'){
              fprintf(stderr,"missing ending :\n");
              return;
            }
            ch=line[++j];
          }
          int len = j-start-2;
          char* stri = malloc(len+1);
          stri[len]='\0';
          strncpy(stri,&line[start+2],len);
          free(def_funcs[(int)name]);
          def_funcs[(int)name]=stri;
        }
     
    //define variable 
    else if(ch==';'){
          int start=j;
          ch=line[++j];
          char name = ch;
          ch=line[++j];
          while(ch!=';'){
            if(ch=='\0'){
              fprintf(stderr,"missing ending ;\n");
              return;
            }
            ch=line[++j];
          }
          int len = j-start-2;
          char* stri = malloc(len+1);
          stri[len]='\0';
          strncpy(stri,&line[start+2],len);
          free(def_vars[(int)name]);
          def_vars[(int)name]=stri;
        }
    
    else{
      fprintf(stderr, "%c is not a valid character!\n",ch);
      return;
    }
  }  
}




