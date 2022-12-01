#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "klack.h"

int main(int argc, char** argv) {

  printf("Klack REPL v0.1\n");

  init_prims();
  while (1) {

    char* input = readline("> ");

    add_history(input);

    execline(input);

    free(input);
  }

  return 0;
} 
