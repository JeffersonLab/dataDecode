#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <byteswap.h>
#include "tidec.h"

#ifndef DATADECODE
#define DATADECODE fa250DataDecode
#endif

int
main(int argc, char *argv[])
{
  FILE *f;
  uint32_t data = 0;
  char *user_input = NULL;
  size_t input_size = 0;
  size_t line_len = 0;
  int bytes_read, bytes_skip = 0;

  
  if(argc == 2)
    {
      f = fopen(argv[1], "r");
      if(f)
	{
	  while(fscanf(f, "%x", &data) > 0)
	    {
	      /* data = bswap_32(data); */
	      DATADECODE(data);
	    }
	}
      else
	perror(argv[1]);
    }
  else if(argc == 1)
    {
      while(1)
	{
	  line_len = getline(&user_input, &input_size, stdin);
	  if(line_len > 1)
	    {
	      while(sscanf(user_input + bytes_skip, "%x%n", &data, &bytes_read) == 1)
		{
		  DATADECODE(data);
		  bytes_skip += bytes_read;
		}
	      bytes_skip = 0;
	    }
	  else
	    break;
	}
    }
  return 0;
}
