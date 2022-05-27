#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const int STR_MAX_SIZE = 1024;

void get_logfile(char *buf, const int buf_size, const char *ext) {
  char exe[STR_MAX_SIZE];
  int ret = readlink("/proc/self/exe", exe, sizeof(exe) - 1);
  if (ret == -1) {
    fprintf(stderr, "Error: Cannot find /proc/self/exe\n");
    exit(1);
  }
  exe[ret] = 0;

  int len = strlen(exe);
  strncpy(buf, "/tmp/", 6);
  strncat(buf, exe, len);
  buf[len] = 0;
  for (int i = 0; i <= strlen(ext); ++i) {
    buf[buf_size-(i+1)] = 0;
  }
  strncat(buf, ext, strlen(ext));
}

void __sanitize__(int divisor, int line, int col) {
  if (divisor == 0) {
    printf("Divide-by-zero detected at line %d and col %d\n", line, col);
    exit(1);
  }
}

void __coverage__(int line, int col) {
  char logfile[STR_MAX_SIZE];
  get_logfile(logfile, sizeof(logfile), ".cov");
  FILE *f = fopen(logfile, "a");
  fprintf(f, "%d, %d\n", line, col);
  fclose(f);
}
