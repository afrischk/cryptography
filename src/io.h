#ifndef IO_H
#define IO_H
#include <stdlib.h>

struct io_data{
  char* buf;
  size_t size;
};

struct io_data* read(void);

#endif /*IO_H*/
