#ifndef ENV_VAR_H
#define ENV_VAR_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char env[1024];

void initEnv() {
  FILE* fp = fopen(".muttrc", "r");
  if (fp == NULL) return;
  clearenv();
  while (fgets(env, 1024, fp) != NULL) putenv(env);
  fclose(fp);
}

void muttExport(char *argv[]) {
  if (argv[1] == NULL) return;
  strcpy(env, argv[1]);
  putenv(env);
}

#endif