//typedefs and function prototypes

typedef enum {
  INTEGER,
  CHARACTER,
  VERB,
  ADVERB,
  EOL
} Tokentype;

typedef struct{
  Tokentype type;
  union Value{
    long int a;
    double b;
    char c;
  }val;
} Token;

void init_prims();
void execline(char* line);
