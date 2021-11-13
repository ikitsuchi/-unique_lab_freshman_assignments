#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "../incl/builtin.h"
#include "../incl/error.h"
#include "../incl/prompt.h"
#include "../incl/env_var.h"
#include "../incl/exec_command.h"

int argc, cmdc;
char *argv[128];
char *command[128];
char *instruction = NULL;
int child_pid;
int background = 0;

int origin_stdin, origin_stdout;
int input_fd, output_fd;
int input_flag, output_flag;
char *input_filename, *output_filename;
FILE *input_file, *output_file;

int isValid(char c) { return c != ' ' && isprint(c); }
void splitArguments(char *command);
void splitCommands(char *instruction);

void ioRecovery();
int ioRedirect();
int iteratePipe(int id, int prev_pipe_fd);
void exec(char *argv[]);

void sigintHandler(int sig);
void sigchldHandler(int sig);
void initEnv();

int main() {
  signal(SIGINT, sigintHandler);
  signal(SIGCHLD, sigchldHandler);
  initEnv();
  generatePrompt();

  while (1) {
    instruction = readline(prompt);
    add_history(instruction);
    if (instruction == NULL) {
      fprintf(stdout, "\n");
      free(instruction);
      break;
    }
    splitCommands(instruction);
    free(instruction);

    origin_stdin = dup(0);
    origin_stdout = dup(1);
    iteratePipe(0, 0);
    background = 0;
    for (int i = 0; i < cmdc; ++i) {
      free(command[i]);
      command[i] = NULL;
    }
  }
  return 0;
}

int ioRedirect() {
  input_flag = input_filename != NULL;
  output_flag = output_filename != NULL;
  int return_value = 0;
  if (input_flag) {
    input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
      fprintf(stderr, "muttshell: %s: No such file or directory.\n",
              input_filename);
      free(input_filename);
      input_filename = NULL;
      input_flag = 0;
      return_value = -1;
    } else {
      input_fd = input_file->_fileno;
    }
  }
  if (output_flag) {
    output_file = fopen(output_filename, "w");
    output_fd = output_file->_fileno;
  }
  return return_value;
}

void ioRecovery() {
  if (input_flag) {
    fclose(input_file);
    free(input_filename);
    input_filename = NULL;
  }
  if (output_flag) {
    fclose(output_file);
    free(output_filename);
    output_filename = NULL;
  }
  input_flag = 0;
  output_flag = 0;
  child_pid = 0;
}

void sigintHandler(int sig) {
  if (child_pid != 0) {
    kill(child_pid, SIGINT);
    puts("");
  }
}

void sigchldHandler(int sig) { wait(NULL); }

void splitCommands(char *instruction) {
  cmdc = 0;
  int length = strlen(instruction);
  for (int i = 0; i < length; ++i) {
    if (instruction[i] == '|') continue;
    command[cmdc] = (char *)calloc(128, sizeof(char));
    for (int j = 0; instruction[i] != '|' && i < length; ++i, ++j) {
      if (instruction[i] == '\\') {
        if (isValid(instruction[++i])) {
          command[cmdc][j] = instruction[i];
        } else {
          --i;
          command[cmdc][j] = '\\';
        }
        continue;
      } else if (instruction[i] == '&') {
        background = 1;
        break;
      } else {
        command[cmdc][j] = instruction[i];
      }
    }
    ++cmdc;
  }
}

void splitArguments(char *command) {
  argc = 0;
  int length = strlen(command);
  for (int i = 0; i < length; ++i) {
    if (!isValid(command[i])) continue;
    argv[argc] = (char *)calloc(128, sizeof(char));
    if (command[i] == '<' || command[i] == '>') {
      argv[argc++][0] = command[i];
      continue;
    }
    for (int j = 0; isValid(command[i]) && i < length; ++i, ++j) {
      if (command[i] == '<' || command[i] == '>') {
        argv[++argc] = (char *)calloc(1, sizeof(char));
        argv[argc][0] = command[i];
        break;
      }
      argv[argc][j] = command[i];
    }
    if (argc > 0 && strcmp(argv[argc - 1], "<") == 0) {
      if (input_filename == NULL)
        input_filename = (char *)calloc(128, sizeof(char));
      strcpy(input_filename, argv[argc]);
      free(argv[argc]);
      argv[argc] = NULL;
      free(argv[argc - 1]);
      argv[argc - 1] = NULL;
      argc -= 2;
    }
    if (argc > 0 && strcmp(argv[argc - 1], ">") == 0) {
      if (output_filename == NULL)
        output_filename = (char *)calloc(128, sizeof(char));
      strcpy(output_filename, argv[argc]);
      free(argv[argc]);
      argv[argc] = NULL;
      free(argv[argc - 1]);
      argv[argc - 1] = NULL;
      argc -= 2;
    }
    ++argc;
  }
}

int iteratePipe(int id, int prev_pipe_fd) {
  if (id == cmdc) return 0;
  splitArguments(command[id]);
  int io_return = ioRedirect(id);

  if (id < cmdc - 1) {
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
      return printError("muttshell", "create pipe", errno);

    if (!isBuiltin(argv[0])) {
      child_pid = fork();
      if (child_pid == 0) {
        close(pipe_fd[0]);
        if (input_flag) {
          dup2(input_fd, 0);
          close(input_fd);
        } else if (io_return == -1) {
          exit(0);
        } else {
          if (id > 0) dup2(prev_pipe_fd, 0);
        }
        if (id > 0) close(prev_pipe_fd);
        if (output_flag) {
          dup2(output_fd, 1);
          close(output_fd);
        } else {
          if (id < cmdc - 1) dup2(pipe_fd[1], 1);
        }
        if (id < cmdc - 1) close(pipe_fd[1]);
        exec(argv);
        exit(0);
      } else {
        close(pipe_fd[1]);
        if (id > 0) close(prev_pipe_fd);
        ioRecovery();
        for (int j = 0; j < argc; ++j) {
          free(argv[j]);
          argv[j] = NULL;
        }
        return iteratePipe(id + 1, pipe_fd[0]);
      }
    } else {
      if (input_flag) {
        dup2(input_fd, 0);
        close(input_fd);
      } else if (io_return == -1) {
        exit(0);
      } else {
        if (id > 0) dup2(prev_pipe_fd, 0);
      }
      if (id > 0) close(prev_pipe_fd);
      if (output_flag) {
        dup2(output_fd, 1);
        close(output_fd);
      } else {
        if (id < cmdc - 1) dup2(pipe_fd[1], 1);
      }
      if (id < cmdc - 1) close(pipe_fd[1]);
      exec(argv);
      ioRecovery();
      for (int j = 0; j < argc; ++j) {
        free(argv[j]);
        argv[j] = NULL;
      }
      return iteratePipe(id + 1, pipe_fd[0]);
    }
  } else {
    if (!isBuiltin(argv[0])) {
      int child_pid = fork();
      if (child_pid == 0) {
        if (input_flag) {
          dup2(input_fd, 0);
          close(input_fd);
        } else if (io_return == -1) {
          if (id > 0) close(prev_pipe_fd);
          exit(0);
        } else {
          if (id > 0) dup2(prev_pipe_fd, 0);
        }
        if (id > 0) close(prev_pipe_fd);
        if (output_flag) {
          dup2(output_fd, 1);
          close(output_fd);
        } else {
          dup2(origin_stdout, 1);
          close(origin_stdout);
        }
        exec(argv);
        exit(0);
      } else {
        if (id > 0) close(prev_pipe_fd);
        if (!background) waitpid(child_pid, NULL, 0);
        ioRecovery();
        for (int j = 0; j < argc; ++j) {
          free(argv[j]);
          argv[j] = NULL;
        }
        dup2(origin_stdin, 0);
        close(origin_stdin);
        dup2(origin_stdout, 1);
        close(origin_stdout);
        return 0;
      }
    } else {
      if (input_flag) {
        dup2(input_fd, 0);
        close(input_fd);
      } else if (io_return == -1) {
        if (id > 0) close(prev_pipe_fd);
        exit(0);
      } else {
        if (id > 0) dup2(prev_pipe_fd, 0);
      }
      if (id > 0) close(prev_pipe_fd);
      if (output_flag) {
        dup2(output_fd, 1);
        close(output_fd);
      } else {
        dup2(origin_stdout, 1);
        close(origin_stdout);
      }
      exec(argv);
      return 0;
    }
  }
}