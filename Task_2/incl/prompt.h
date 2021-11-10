#ifndef PROMPT_H
#define PROMPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char prompt[128];

void generatePrompt() {
  char hostname[128];
  gethostname(hostname, 128);
  char current_dir[128];
  getcwd(current_dir, 128);

  prompt[0] = '[';
  strcpy(prompt + 1, getlogin());
  size_t length = strlen(prompt);
  prompt[length++] = '@';
  strcpy(prompt + length, hostname);
  length = strlen(prompt);
  prompt[length++] = ']';
  strcpy(prompt + length, "￥");
}

#endif