#include <stdio.h>
#include <errno.h>
#include "../incl/error.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "rm: Missing argv.\n");
    return -1;
  }
  char buffer[114514];
  int return_value = 0;
  for (int i = 1; i < argc; ++i) {
    int ret = remove(argv[i]);
    return_value |= ret;
    if (ret == -1) printError("rm", argv[i], errno);
  }
  return return_value;
}