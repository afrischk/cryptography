#ifndef IO_H
#define IO_H
#include <stdlib.h>

typedef struct data{
  char* buf;
  size_t size;
} data_t;

data_t* read();

#endif /*IO_H*/
