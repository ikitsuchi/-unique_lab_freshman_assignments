#ifndef BUILTIN_H
#define BUILTIN_H

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "../incl/error.h"
#include "../incl/env_var.h"

#define MUTT_MAX 114514

char *builtin[] = {"echo", "exit", "cd", "pwd", "export", "clear"};

int isBuiltin(char argv[]) {
  int return_value = 0;
  for (int i = 0; builtin[i] != NULL; ++i)
    if (strcmp(builtin[i], argv) == 0) return_value = 1;
  return return_value;
}

void muttEcho(char *argv[]) {
  for (int i = 1; argv[i] != NULL; ++i) {
    fprintf(stdout, "%s ", argv[i]);
  }
  fprintf(stdout, "\n");
}

void muttExit() { exit(0); }

void muttCd(char *argv[]) {
  if (argv[2] != NULL) {
    fprintf(stderr, "cd: Too many arguments.\n");
  }/* else if (argv[1] == NULL) {
    printf("%s\n", getenv("HOME"));
    if (chdir(getenv("HOME")) != 0)
      printError("cd", argv[1], errno);
  }*/ else if (chdir(argv[1]) != 0) {
    printError("cd", argv[1], errno);
  }
}

void muttPwd() {
  char *buffer = (char *)malloc(MUTT_MAX);
  if (getcwd(buffer, MUTT_MAX) != NULL) {
    fprintf(stdout, "%s\n", buffer);
  } else {
    fprintf(stderr, "pwd: Error.\n");
  }
  free(buffer);
}

void muttClear() {
  fprintf(stdout, "\x1b[H\x1b[J");
}

#endif