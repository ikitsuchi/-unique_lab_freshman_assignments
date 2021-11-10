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
    fprintf(stderr, "cp: Missing argv.\n");
    return -1;
  } else if (argc == 3) {
    if (isDirectory(argv[1])) return printError("cp", argv[1], EISDIR);

    FILE *source = fopen(argv[1], "r");
    if (source == NULL) return printError("cp", argv[1], ENOENT);

    char *dest_path = NULL;
    if (isDirectory(argv[2])) {
      int length_1 = strlen(argv[1]);
      int length_2 = strlen(argv[2]);
      dest_path = (char *) malloc(sizeof(char) * (length_1 + length_2 + 1));
      strcpy(dest_path, argv[2]);
      dest_path[length_2] = '/';
      strcpy(dest_path + length_2 + 1, argv[1]);
    } else {
      dest_path = (char *) malloc(sizeof(char) * (strlen(argv[2])));
      strcpy(dest_path, argv[2]);
    }
    FILE *dest = fopen(dest_path, "w");
    char *buffer = (char *) malloc(sizeof(char) * 114514);
    int num;
    while (num = read(source->_fileno, buffer, 114514)) 
      write(dest->_fileno, buffer, num);

    free(dest_path);
    free(buffer);
    fclose(source);
    fclose(dest);
  } else if (argc > 3) {
    if (!isDirectory(argv[argc - 1])) return printError("cp", argv[argc - 1], ENOTDIR);
    int length_dir = strlen(argv[argc - 1]);

    for (int i = 1; i < argc - 1; ++i) {
      if (isDirectory(argv[i])) return printError("cp", argv[i], EISDIR);

      FILE *source = fopen(argv[i], "r");
      if (source == NULL) return printError("cp", argv[i], ENOENT);

      char *dest_path = (char *) malloc(sizeof(char) * (length_dir + strlen(argv[i]) + 1));
      strcpy(dest_path, argv[argc - 1]);
      dest_path[length_dir] = '/';
      strcpy(dest_path + length_dir + 1, argv[i]);
      FILE *dest = fopen(dest_path, "w");
      char *buffer = (char *) malloc(sizeof(char) * 114514);
      int num;
      while (num = read(source->_fileno, buffer, 114514)) 
        write(dest->_fileno, buffer, num);

      free(dest_path);
      free(buffer);
      fclose(source);
      fclose(dest);
    }
  }
  return 0;
}