#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "../incl/error.h"

int main(int argc, char *argv[]) {
  char *buffer = (char *)malloc(114514);
  int length;
  if (argc == 1) {
    while ((length = read(STDIN_FILENO, buffer, 114514)) > 0)
      write(STDOUT_FILENO, buffer, length);
    return 0;
  }
  int return_value = 0;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-") == 0) {
      while ((length = read(STDIN_FILENO, buffer, 114514)) > 0)
        write(STDOUT_FILENO, buffer, length);
    } else {
      int fp = open(argv[i], O_RDONLY);
      if (fp >= 0) {
        while ((length = read(fp, buffer, 114514)) > 0)
          write(STDOUT_FILENO, buffer, length);
        close(fp);
      } else {
        printError("cat", argv[i], errno);
        return_value = -1;
      }
    }
  }
  free(buffer);
  return return_value;
}
