#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include "../incl/error.h"

int isDirectory(char *pathname) {
  struct stat statbuf;
  if (stat(pathname, &statbuf) != 0) return 0;
  return S_ISDIR(statbuf.st_mode); 
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "mv: Missing argv.\n");
    return -1;
  } else if (argc == 3) {
    if (!isDirectory(argv[2])) {
      if (rename(argv[1], argv[2]) == -1) printError("mv", NULL, errno);
    } else {
      char *dest = (char *) malloc(sizeof(char) * (strlen(argv[2] + 1 + strlen(argv[1]))));
      strcpy(dest, argv[2]);
      dest[strlen(argv[2])] = '/';
      strcpy(dest + strlen(argv[2]) + 1, argv[1]);
      if (rename(argv[1], dest) == -1) printError("mv", argv[1], errno);
      free(dest);
    }
  } else if (argc > 3) {
    if (!isDirectory(argv[argc - 1])) return printError("mv", argv[argc - 1], ENOTDIR);
    int length_dir = strlen(argv[argc - 1]);
    for (int i = 1; i < argc - 1; ++i) {
      char *dest = (char *) malloc(sizeof(char) * (length_dir + 1 + strlen(argv[i])));
      strcpy(dest, argv[argc - 1]);
      dest[argc] = '/';
      strcpy(dest + length_dir + 1, argv[i]);
      if (rename(argv[i], dest) == -1) printError("mv", argv[i], errno);
      free(dest);
    }
  }
  return 0;
}