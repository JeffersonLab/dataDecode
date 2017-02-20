#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "fa250dec.h"

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
	  faDataDecode(data);
	}
    }
  else
    perror(argv[1]);
  
  return 0;
}
