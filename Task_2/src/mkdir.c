#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  // 默认的新建文件夹权限 755
  mode_t mode = (mode_t)0755;
  // No argument.
  if (argc == 1) {
    fprintf(stderr, "mkdir: Missing argv.");
    return -1;
  }
  // 0 indicates successful completion, and -1 indicates failure.
  int return_value = 0;
  for (int i = 1; i < argc; ++i) {
    int value = mkdir(argv[i], mode);
    if (value != 0) {
      fprintf(stderr, "mkdir: Can't make directory \"%s\": ", argv[i]);
      switch (errno) {
        case 13:
          fprintf(
              stderr,
              "Write permission is denied for the parent directory in which "
              "the new directory is to be added.\n");
          break;

        case 17:
          fprintf(stderr, "A file named %s already exists.\n", argv[i]);
          break;

        case 31:
          fprintf(stderr,
                  "The parent directory has too many links (entries).\n");
          break;

        case 28:
          fprintf(stderr,
                  "The file system doesn’t have enough room to create the new "
                  "directory.\n");
          break;

        case 30:
          fprintf(stderr,
                  "The parent directory of the directory being created is on a "
                  "read-only file system and cannot be modified.\n");
          break;
      }
    }
    return_value |= value;
  }
  return return_value;
}