#ifndef EXEC_COMMAND_H
#define EXEC_COMMAND_H

#include "builtin.h"

int execBuiltin(char command[], char *argv[]) {
  if (strcmp(command, "echo") == 0) {
    muttEcho(argv);
  } else if (strcmp(command, "exit") == 0) {
    muttExit();
  } else if (strcmp(command, "cd") == 0) {
    muttCd(argv);
  } else if (strcmp(command, "pwd") == 0) {
    muttPwd(argv);
  } else if (strcmp(command, "export") == 0) {
    muttExport(argv);
  } else if (strcmp(command, "clear") == 0) {
    muttClear();
  }
  return 0;
}

void exec(char *argv[]) {
  if (isBuiltin(argv[0])) {
    execBuiltin(argv[0], argv);
  } else {
    if (execvp(argv[0], argv) == -1)
      fprintf(stderr, "muttshell: %s: Command not found.\n", argv[0]);
  }
}

#endif