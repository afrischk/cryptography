#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "io.h"

data_t* read(){
  size_t nread;
  size_t buf_size = 1024;
  data_t* data = malloc(sizeof(data_t));
  data->size = 0;
  data->buf = malloc(sizeof(char) * buf_size);
  char* buf = malloc(sizeof(char) * buf_size);
  freopen(NULL, "rb", stdin);
  // TODO DAP DEBUG with stdin and file does not work
  //FILE *file = fopen("../../files/bible.b64", "rb");
  while((nread = fread(buf, 1, buf_size, stdin)) > 0)
  //while((nread = fread(buf, 1, buf_size, file)) > 0)
  {
    if(data->size == 0){
      memcpy(data->buf, buf, nread);
    }
    else
    {
      data->buf = realloc(data->buf, data->size + nread);
      if(data->buf == NULL){
        printf("Failed to allocate memory!\n");
      }

      memcpy(data->buf + data->size, buf, nread);
    }

    data->size += nread;
  }
  free(buf);
  return data;
}
