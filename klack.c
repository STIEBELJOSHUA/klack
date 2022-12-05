#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include "klack.h"

//macros
#define IS_IGNORE(ch) (ch==' ' || ch=='\n' || ch=='\0')
#define IS_NUM(ch) ((ch > 47 && ch < 58)||(ch=='.')||(ch=='-'))

//globals
//points to head of stack
static Token* stackhead = NULL;

//helper functions

//pop top value from stack
Token* popped(){
  Token* pop = stackhead;
  if(pop==NULL){
    printf("STACK IS EMTPY\n");
    return NULL;
  }
  stackhead = pop->prev;
  return pop;
}

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


void printarray(Token* tk, int ischar){
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
    if(ischar)
      printf("%c",(char)tk->matrix.d[i]);
    else if((int)db==db)
      printf("%d",(int)tk->matrix.d[i]);
    else
      printf("%.4f",tk->matrix.d[i]);
    if(i+1!=tk->len && !ischar)
      printf(" ");
  }
  printf(")\n");
  
}

void printstack(int ischar){
  Token* headcopy = stackhead;
  if(headcopy!=NULL){
    while(headcopy->prev!=NULL){
      printarray(headcopy,ischar);
      headcopy = headcopy->prev;
    }
    printarray(headcopy,ischar);
  }
}

//TODO: this is slightly broken, addition and all others too return diffrent depending on order
// ie (1 2 3)(2)+ returns (3 4 5), while (2)(1 2 3)+ returns (3). 
void dubop(char ch){
  if(stackhead==NULL || stackhead->prev==NULL){
    puts("NOT ENOUGH ELEMENTS ON STACK");
    return;
  }
  Token* tk1 = popped();
  Token* tk2 = stackhead;
  if(tk1->len==1){
    for(int i=0;i<tk2->len;i++){
      double r;
      switch(ch){
      case '+' :
         r = tk2->matrix.d[i]+tk1->matrix.d[0];
         break;
      case '-' :
         r = tk2->matrix.d[i]-tk1->matrix.d[0];
         break;
      case '/' :
          r = tk2->matrix.d[i]/tk1->matrix.d[0];
          break;
      case '*' :
          r = tk2->matrix.d[i]*tk1->matrix.d[0];
          break;
      case '%' :
          r = (int)tk2->matrix.d[i]%(int)tk1->matrix.d[0];
          break;
      case '>' :
          r = tk2->matrix.d[i]>tk1->matrix.d[0]?1:0;
          break;
      case '<' :
          r = tk2->matrix.d[i]<tk1->matrix.d[0]?1:0;
          break;
      case '=' :
          r = tk2->matrix.d[i]==tk1->matrix.d[0];
          break;
      }
      tk2->matrix.d[i]=r;
    }
  }
  else if(tk1->len>1){
    int iter = MIN(tk1->len,tk2->len);
    for(int i=0;i<iter;i++){
      double r;
      switch(ch){
        case '+' :
          r = tk2->matrix.d[i]+tk1->matrix.d[i];
          break;
        case '-' :
          r = tk2->matrix.d[i]-tk1->matrix.d[i];
        case '/' :
          r = tk2->matrix.d[i]/tk1->matrix.d[i];
          break;
        case '*' :
          r = tk2->matrix.d[i]*tk1->matrix.d[i];
          break;
        case '%' :
          r = (int)tk2->matrix.d[i]%(int)tk1->matrix.d[i];
          break;
        case '>' :
          r = tk2->matrix.d[i]>tk1->matrix.d[i]?1:0;
          break;
        case '<' :
          r = tk2->matrix.d[i]<tk1->matrix.d[i]?1:0;
          break;
        case '=' :
          r = tk2->matrix.d[i]==tk1->matrix.d[i]?1:0;
          break;
        
      }
      tk2->matrix.d[i]=r;
    }
  }
  else{
    puts("ERROR, 0 length array");
  }
  free(tk1->matrix.d);
  free(tk1);
}

//primitive implementations

void add(){
  dubop('+');
}

void sub(){
  dubop('-');
}

void divd(){
  dubop('/');
}

void mult(){
  dubop('*');
}

void mod(){
  dubop('%');
}

void gt(){
  dubop('>');
}

void lt(){
  dubop('<');
}

void eq(){
  dubop('=');
}

void sum(){
  double* arr = stackhead->matrix.d;
  double sumd = 0;
  for(int i=0;i<stackhead->len;i++){
    sumd+=arr[i];
  }
  
  stackhead->matrix.d=malloc(sizeof(double)*10);
  stackhead->size=10;
  stackhead->len=0;
  darrayappend(stackhead,sumd);
  
  free(arr);
}


void pop(){
  Token* tk = popped();
  if(tk==NULL)
    return;
  free(tk->matrix.d);
  free(tk);
}


void dprintstack(){
  printstack(0);
}

void sprintstack(){
  printstack(1);
}

//primitave "dict"
char prims[] = "+-pP./*%><=s";
void (*prim_func[128])();

void init_prims(){
  prim_func[(int)'+']=add;
  prim_func[(int)'-']=sub;
  prim_func[(int)'p']=dprintstack;
  prim_func[(int)'P']=sprintstack;
  prim_func[(int)'.']=pop;
  prim_func[(int)'/']=divd;
  prim_func[(int)'*']=mult;
  prim_func[(int)'%']=mod;
  prim_func[(int)'>']=gt;
  prim_func[(int)'<']=lt;
  prim_func[(int)'=']=eq;
  prim_func[(int)'s']=sum;
}

//defined function "dict"
char* def_funcs[128];
//defined variables "dict"
Token* def_vars[128];
//execute a chunk of code MAKE SURE ITS NULL TERMINATED
void execline(char* line){
  int i = strlen(line);
  
  for(int j=0;j<i;j++){
    int oldjtmp=j;
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
      
      Token* var = def_vars[(int)ch];
      Token* tk = malloc(sizeof(Token));
      *tk=*var;
      tk->matrix.d=malloc(sizeof(double)*tk->size);
      memcpy(tk->matrix.d,var->matrix.d,sizeof(double)*tk->size);
      
      tk->prev=stackhead;
      stackhead = tk;
      
      
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
          free(def_funcs[(int)name]);
          def_funcs[(int)name]=stri;
        }
     
    //define variable 
    else if(ch=='$'){
      free(def_vars[(int)ch]);
      
      ch=line[++j];
      Token* tk = malloc(sizeof(Token));
      *tk=*stackhead;

      tk->matrix.d=malloc(sizeof(double)*tk->size);
      memcpy(tk->matrix.d,stackhead->matrix.d,sizeof(double)*tk->size);

      def_vars[(int)ch]=tk;
      }
    
    else{
      fprintf(stderr, "%c is not a valid character!\n",ch);
      return;
    }
  
    int oldj = oldjtmp;
  }
}




