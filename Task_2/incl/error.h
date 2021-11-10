#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <errno.h>

int printError(char *command, char *argv, int errortype) {
  if (argv != NULL) fprintf(stderr, "%s: %s: ", command, argv);
  else fprintf(stderr, "%s: ", command);
  switch (errortype) {
    case EPERM:
      fprintf(stderr, "%s\n", "Operation not permitted.");
      break;
    case ENOENT:
      fprintf(stderr, "%s\n", "No such file or directory.");
      break;
    case ESRCH:
      fprintf(stderr, "%s\n", "No such process.");
      break;
    case EINTR:
      fprintf(stderr, "%s\n", "Interrupted system call.");
      break;
    case EIO:
      fprintf(stderr, "%s\n", "I/O error.");
      break;
    case ENXIO:
      fprintf(stderr, "%s\n", "No such device or address.");
      break;
    case E2BIG:
      fprintf(stderr, "%s\n", "Argument list too long.");
      break;
    case ENOEXEC:
      fprintf(stderr, "%s\n", "Exec format error.");
      break;
    case EBADF:
      fprintf(stderr, "%s\n", "Bad file number.");
      break;
    case ECHILD:
      fprintf(stderr, "%s\n", "No child processes.");
      break;
    case EAGAIN:
      fprintf(stderr, "%s\n", "Try again.");
      break;
    case ENOMEM:
      fprintf(stderr, "%s\n", "Out of memory.");
      break;
    case EACCES:
      fprintf(stderr, "%s\n", "Permission denied.");
      break;
    case EFAULT:
      fprintf(stderr, "%s\n", "Bad address.");
      break;
    case ENOTBLK:
      fprintf(stderr, "%s\n", "Block device required.");
      break;
    case EBUSY:
      fprintf(stderr, "%s\n", "Device or resource busy.");
      break;
    case EEXIST:
      fprintf(stderr, "%s\n", "File exists.");
      break;
    case EXDEV:
      fprintf(stderr, "%s\n", "Cross-device link.");
      break;
    case ENODEV:
      fprintf(stderr, "%s\n", "No such device.");
      break;
    case ENOTDIR:
      fprintf(stderr, "%s\n", "Not a directory.");
      break;
    case EISDIR:
      fprintf(stderr, "%s\n", "Is a directory.");
      break;
    case EINVAL:
      fprintf(stderr, "%s\n", "Invalid argument.");
      break;
    case ENFILE:
      fprintf(stderr, "%s\n", "File table overflow.");
      break;
    case EMFILE:
      fprintf(stderr, "%s\n", "Too many open files.");
      break;
    case ENOTTY:
      fprintf(stderr, "%s\n", "Not a typewriter.");
      break;
    case ETXTBSY:
      fprintf(stderr, "%s\n", "Text file busy.");
      break;
    case EFBIG:
      fprintf(stderr, "%s\n", "File too large.");
      break;
    case ENOSPC:
      fprintf(stderr, "%s\n", "No space left on device.");
      break;
    case ESPIPE:
      fprintf(stderr, "%s\n", "Illegal seek.");
      break;
    case EROFS:
      fprintf(stderr, "%s\n", "Read-only file system.");
      break;
    case EMLINK:
      fprintf(stderr, "%s\n", "Too many links.");
      break;
    case EPIPE:
      fprintf(stderr, "%s\n", "Broken pipe.");
      break;
    case EDOM:
      fprintf(stderr, "%s\n", "Math argument out of domain of func.");
      break;
    case ERANGE:
      fprintf(stderr, "%s\n", "Math result not representable.");
      break;
    default:
      break;
  }
  return -1;
}

#endif