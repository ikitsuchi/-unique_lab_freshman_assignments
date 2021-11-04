#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int flag_a = 0;
int flag_l = 0;
int dir_num = 0;

int compare(void *x_, void *y_) {
  char *x = *(char **)x_, *y = *(char **)y_;
  int len_x = strlen(x), len_y = strlen(y);
  for (int i = 0; i < len_x && i < len_y; ++i) {
    if (isalpha(x[i]) && isalpha(y[i])) {
      x[i] = tolower(x[i]), y[i] = tolower(y[i]);
      if (x[i] < y[i])
        return -1;
      else if (x[i] > y[i])
        return 1;
    } else {
      if (x[i] != y[i]) return (x[i] < y[i]) ? -1 : 1;
    }
  }
  if (len_x == len_y)
    return 0;
  else
    return (len_x < len_y) ? -1 : 1;
}

void listDirectory(char dir[]) {
  int count = 0;
  DIR *d = opendir((dir == NULL) ? "." : dir);
  if (d == NULL) {
    fprintf(stderr, "ls: File or directory not found.\n");
    return;
  }
  struct dirent *dir_detail;
  char **file_names = NULL;
  while ((dir_detail = readdir(d)) != NULL) {
    if (dir_detail->d_name[0] == '.' && flag_a == 0) continue;
    file_names = (char **)realloc(file_names, sizeof(char *) * (count + 1));
    file_names[count++] = dir_detail->d_name;
  }

  qsort(file_names, count, sizeof(char *), compare);

  if (!flag_a && !flag_l) {
    if (dir_num > 1) fprintf(stdout, "%s:\n", dir);
    for (int i = 0; i < count; ++i) fprintf(stdout, "%s  ", file_names[i]);
    fprintf(stdout, "\n");
  }
  file_names = (char **)realloc(file_names, 0);
  closedir(d);
}

int main(int argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      for (int j = 1; argv[i][j]; ++j) {
        if (argv[i][j] == 'a') {
          flag_a = 1;
        } else if (argv[i][j] == 'l') {
          flag_l = 1;
        }
      }
    } else {
      ++dir_num;
    }
  }
  if (argc == 1) {
    listDirectory(NULL);
  } else {
    for (int i = 1; i < argc; ++i) {
      if (argv[i][0] == '-') continue;
      listDirectory(argv[i]);
    }
  }
  return 0;
}