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
  Scalar* scalhead;
  Scalar* scaltail;
  int size;
  struct TokenT* prev;
} Token;



void init_prims();
void execline(char* line);
