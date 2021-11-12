#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include "../incl/error.h"

int main(int argc, char *argv[]) {
  mode_t mode = (mode_t)0755;
  if (argc == 1) {
    fprintf(stderr, "mkdir: Missing argv.");
    return -1;
  }
  int return_value = 0;
  for (int i = 1; i < argc; ++i) {
    int ret = mkdir(argv[i], mode);
    if (ret != 0) printError("mkdir", argv[i], errno);
    return_value |= ret;
  }
  return return_value;
}