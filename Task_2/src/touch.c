#include <errno.h>
#include <stdio.h>
#include <utime.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  // No argument.
  if (argc == 1) {
    fprintf(stderr, "mkdir: Missing argv.");
    return -1;
  }
  for (int i = 1; i < argc; ++i) {
    // Create the file if it doesn't exist.
    int value = open(argv[i], O_CREAT | O_EXCL, (mode_t) 0644);
    // The file doesn't exist, update the timestamp.
    if (errno == 17) {
      utime(argv[i], NULL);
    }
  }
  return 0;
}