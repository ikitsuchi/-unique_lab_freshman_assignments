#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>

int flag_a = 0;
int flag_l = 0;
int dir_num = 0;

int compare(const void *x_, const void *y_) {
  char *x = (char *) x_, *y = (char *) y_;
  int len_x = strlen(x), len_y = strlen(y);
  for (int i = 0; i < len_x && i < len_y; ++i) {
    if (isalpha(x[i]) && isalpha(y[i])) {
      char a = tolower(x[i]), b = tolower(y[i]);
      if (a < b)
        return -1;
      else if (a > b)
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
  DIR *d;
  if (dir == NULL) d = opendir(".");
  else d = opendir(dir);
  if (d == NULL) {
    fprintf(stderr, "ls: Error.\n");
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

  if (!flag_l) {
    if (dir_num > 1) fprintf(stdout, "%s:\n", dir);
    for (int i = 0; i < count; ++i) fprintf(stdout, "%s\t", file_names[i]);
    fprintf(stdout, "\n");
  } else {
    struct stat file_status;
    printf("%d\n", count);
    for (int i = 0; i < count; ++i) {
      printf("%s\n", file_names[i]);
      stat(file_names[i], &file_status);

      char mode[10];
      if (S_ISDIR(file_status.st_mode)) mode[0] = 'd';
      else if (S_ISREG(file_status.st_mode)) mode[0] = '-';
      else if (S_ISLNK(file_status.st_mode)) mode[0] = 'l';
      else if (S_ISCHR(file_status.st_mode)) mode[0] = 'c';
      else if (S_ISSOCK(file_status.st_mode)) mode[0] = 's';

      int mode_owner = file_status.st_mode / 64 % 8;
      int mode_group = file_status.st_mode / 8 % 8;
      int mode_other = file_status.st_mode % 8;
      mode[1] = mode_owner & 4 ? 'r' : '-';
      mode[2] = mode_owner & 2 ? 'w' : '-';
      mode[3] = mode_owner & 1 ? 'x' : '-';
      mode[4] = mode_group & 4 ? 'r' : '-';
      mode[5] = mode_group & 2 ? 'w' : '-';
      mode[6] = mode_group & 1 ? 'x' : '-';
      mode[7] = mode_other & 4 ? 'r' : '-';
      mode[8] = mode_other & 2 ? 'w' : '-';
      mode[9] = mode_other & 1 ? 'x' : '-';
      fprintf(stdout, "%s ", mode);

      fprintf(stdout, "%d ", file_status.st_nlink);

      fprintf(stdout, "%s ", getpwuid(file_status.st_uid)->pw_name);
      fprintf(stdout, "%s ", getgrgid(file_status.st_gid)->gr_name);
      fprintf(stdout, "%d ", file_status.st_size);
      fprintf(stdout, "%s ", file_status.st_mtime);
      fprintf(stdout, "%s\n", file_names[i]);
    }
  }
  free(file_names);
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
  if (dir_num == 0) {
    listDirectory(NULL);
  } else {
    for (int i = 1; i < argc; ++i) {
      if (argv[i][0] == '-') continue;
      listDirectory(argv[i]);
    }
  }
  return 0;
}