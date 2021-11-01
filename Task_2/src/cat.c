#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *buffer = (char *)malloc(114514);
  if (argc == 1) {
    while (read(STDIN_FILENO, buffer, 114514) > 0)
      fwrite(buffer, 114514, 1, stdout);
    return 0;
  }
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-") == 0) {
      while (read(STDIN_FILENO, buffer, 114514) > 0)
        fwrite(buffer, 114514, 1, stdout);
    } else {
      FILE *fp = fopen(argv[i], "r");
      if (fp != NULL) {
        fread(buffer, 114514, 1, fp);
        fwrite(buffer, 114514, 1, stdout);
        fclose(fp);
      } else {
        fprintf(stderr, "cat: %s: That file or directory doesn't exist.\n", argv[i]);
      }
    }
  }
  free(buffer);
  return 0;
}