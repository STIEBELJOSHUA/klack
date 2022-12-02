//typedefs and function prototypes

typedef enum {
  FLOAT,
  CHARACTER,
  STRING
} Tokentype;


typedef struct ScalarT{
Tokentype type;
  union Value{
    double d;
    char* c;
  }val;
  struct ScalarT* next;
}Scalar;

typedef struct TokenT{
  Tokentype type;
  // number of rows for matrix dimension
  int rows;
  //size is the actiual size of the array in c
  int size;
  //len is the number of elements
  int len;
  union Array{
    double* d;
    char* c;
    char** s;
  }matrix;
  struct TokenT* prev;
} Token;



void init_prims();
void execline(char* line);
