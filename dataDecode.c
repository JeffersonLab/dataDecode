#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <byteswap.h>
#include "tidec.h"

int
main(int argc, char *argv[])
{
  FILE *f;
  uint32_t data = 0;
  
  f = fopen(argv[1], "r");
  if(f)
    {
      while(fscanf(f, "%x", &data) > 0)
	{
	  data = bswap_32(data);
	  tiDataDecode(data);
	}
    }
  else
    perror(argv[1]);
  
  return 0;
}
