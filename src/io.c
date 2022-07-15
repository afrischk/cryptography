#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024

struct io_data *read(const char *file) {
  size_t nread;
  struct io_data *data = malloc(sizeof(struct io_data));
  data->size = 0;
  data->buf = malloc(sizeof(char) * BUFFER_SIZE);
  char *buf = malloc(sizeof(char) * BUFFER_SIZE);
  FILE *input = NULL;
  if (file[0] == '-') {
    input = stdin;
    freopen(NULL, "rb", input);
  } else {
    input = fopen(file, "rb");
      if (input == NULL) {
        printf("Failed to read file!\n");
        exit(1);
      }
  }
  while ((nread = fread(buf, 1, BUFFER_SIZE, input)) > 0) {
    if (data->size == 0) {
      memcpy(data->buf, buf, nread);
    } else {
      data->buf = realloc(data->buf, data->size + nread);
      if (data->buf == NULL) {
        printf("Failed to allocate memory!\n");
        exit(1);
      }
      memcpy(data->buf + data->size, buf, nread);
    }
    data->size += nread;
  }
  fclose(input);
  free(buf);
  buf = NULL;
  return data;
}
