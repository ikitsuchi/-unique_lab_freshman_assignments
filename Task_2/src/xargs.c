#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "../incl/builtin.h"
#include "../incl/exec_command.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "xargs: Error.\n");
    return 0;
  }
  char *buffer = (char *)malloc(sizeof(char) * 114514);
  int stdin_length = read(STDIN_FILENO, buffer, 114514);

  char *exec_argv[4096];
  int exec_argc = 1; 
  exec_argv[0] = (char *) malloc(sizeof(char) * (strlen(argv[1])));
  strcpy(exec_argv[0], argv[1]);
  for (int i = 0, j; i < stdin_length; ++i) {
    if (buffer[i] == ' ' || !isprint(buffer[i])) continue;
    for (j = i; buffer[j] != ' ' && isprint(buffer[j]) && j < stdin_length; ++j);
    exec_argv[exec_argc] = (char *) malloc(sizeof(char) * (j - i + 1));
    strncpy(exec_argv[exec_argc], buffer + i, j - i);
    ++exec_argc;
    i = j;
  }
  for (int i = 2; i < argc; ++i) {
    exec_argv[exec_argc + i - 2] = (char *) malloc(sizeof(char) * strlen(argv[i]));
    strcpy(exec_argv[exec_argc + i - 2], argv[i]);
    ++exec_argc;
  }

  exec(exec_argv);
  free(buffer);
  return 0;
}