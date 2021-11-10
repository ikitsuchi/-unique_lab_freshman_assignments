#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "../incl/error.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "ln: Missing argv.\n");
    return -1;
  } else if (argc > 3) {
    fprintf(stderr, "ln: Too many argv.\n");
    return -1;
  } else {
    if (symlink(argv[1], argv[2]) == -1) printError("ln", NULL, errno);
  }
  return 0;
}