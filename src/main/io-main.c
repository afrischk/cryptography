#include <stdio.h>
#include "io.h"

int main(){
  data_t* data = read();
  printf("Read %zu bytes of data\n", data->size);
  free(data);
}
