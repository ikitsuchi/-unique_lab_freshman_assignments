#ifndef BUILTIN_H
#define BUILTIN_H

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MUTT_MAX 114514

char *builtin[] = {"echo", "exit", "cd", "pwd"};

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
  if (argv[2] != NULL)
    fprintf(stderr, "cd: Too many arguments.\n");
  else if (chdir(argv[1]) != 0)
    fprintf(stderr, "cd: Error.\n");
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

int execBuiltin(char command[], char *argv[]) {
  if (strcmp(command, "echo") == 0) {
    muttEcho(argv);
  } else if (strcmp(command, "exit") == 0) {
    muttExit();
  } else if (strcmp(command, "cd") == 0) {
    muttCd(argv);
  } else if (strcmp(command, "pwd") == 0) {
    muttPwd(argv);
  }
  return 0;
}

#endif