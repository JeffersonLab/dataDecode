#include <stdio.h>
#include <stdint.h>


void
tiDataDecode(uint32_t data)
{
  static uint8_t blocklevel = 0;
  static uint16_t event_length = 0;
  static uint32_t ievent = 0;
  generic_data_word_t gword;
  ti_event_header_t ehead;
  trigger_bank_t tbank;
  
  gword.raw = data;
  /* Check if the word is a block header */
  if(gword.bf.data_type_defining && (gword.bf.data_type_tag == 0))
    {
      block_header_t d; d.raw = data;
      
      printf("%8X - BLOCK HEADER - slot = %d  modID = %d   n_evts = %d   n_blk = %d\n",
	     d.raw,
	     d.bf.slot_number,
	     d.bf.module_ID,
	     d.bf.number_of_events_in_block,
	     d.bf.event_block_number);

      blocklevel = d.bf.number_of_events_in_block;
      ievent = 0;
      event_length = 0;
      return;
    }

  tbank.raw = data;
  /* Check if the word is a trigger bank header */
  if((tbank.bf.tag & 0xff10) == 0xff10)
    {
      printf("%8X - TRIGGER BANK HEADER - type = %d  blocklevel = %d\n",
	     d.raw,
	     d.bf.tag & 0xf,
	     d.bf.number_of_events);

      blocklevel = d.bf.number_of_events;
      ievent = 0;
      event_length = 0;
      return;
    }

  ehead.raw = data;
  /* Check if the word is an event header */
  if(ehead.type == 0x01)
    {
      printf("%8x - EVENT HEADER - trigtype = %d  len = %d\n",
	     d.raw,
	     d.bf.trigger_type,
	     d.bf.length);
      
      ievent = 0;
      event_length = d.bf.length;
      return;
    }
}

int
tiCheckTriggerBlock(volatile unsigned int *data)
{
  unsigned int blen=0, blevel=0, evlen=0;
  int iword=0, iev=0, ievword=0;
  int rval=0;

  printf("--------------------------------------------------------------------------------\n");
  /* First word should be the trigger bank length */
  blen = data[iword];
  printf("%4d: %08X - TRIGGER BANK LENGTH - len = %d\n",iword, data[iword], blen);
  iword++;

  /* Trigger Bank Header */
  if( ((data[iword] & 0xFF100000)>>16 != 0xFF10) ||
      ((data[iword] & 0x0000FF00)>>8 != 0x20) )
    {
      rval = -1;
      printf("%4d: %08X - **** INVALID TRIGGER BANK HEADER ****\n",
	     iword, 
	     data[iword]);
      iword++;
      while(iword<blen+1)
	{
	  if(iword>blen)
	    {
	      rval = -1;
	      printf("----: **** -1: Data continues past Trigger Bank Length (%d) ****\n",blen);
	    }
	  printf("%4d: %08X - **** REST OF DATA ****\n",
		 iword,
		 data[iword]);
	  iword++;
	}
    }
  else
    {
      if(iword>blen)
	{
	  rval = -1;
	  printf("----: **** -1: Data continues past Trigger Bank Length (%d) ****\n",blen);
	}
      blevel = data[iword] & 0xFF;
      printf("%4d: %08X - TRIGGER BANK HEADER - type = %d  blocklevel = %d\n",
	     iword, 
	     data[iword],
	     (data[iword] & 0x000F0000)>>16, 
	     blevel);
      iword++;

      for(iev=0; iev<blevel; iev++)
	{
	  if(iword>blen)
	    {
	      rval = -1;
	      printf("----: **** -1: Data continues past Trigger Bank Length (%d) ****\n",blen);
	    }
	  
	  if((data[iword] & 0x00FF0000)>>16!=0x01)
	    {
	      rval = -1;
	      printf("%4d: %08x - **** INVALID EVENT HEADER ****\n",
		     iword, data[iword]);
	      iword++;
	      while(iword<blen+1)
		{
		  printf("%4d: %08X - **** REST OF DATA ****\n",
			 iword,
			 data[iword]);
		  iword++;
		}
	      break;
	    }
	  else
	    {
	      if(iword>blen)
		{
		  rval = -1;
		  printf("----: **** -1: Data continues past Trigger Bank Length (%d) ****\n",blen);
		}
	      
	      evlen = data[iword] & 0x0000FFFF;
	      printf("%4d: %08x - EVENT HEADER - trigtype = %d  len = %d\n",
		     iword,
		     data[iword],
		     (data[iword] & 0xFF000000)>>24,
		     evlen);
	      iword++;

	      if(iword>blen)
		{
		  rval = -1;
		  printf("----: **** -1: Data continues past Trigger Bank Length (%d) ****\n",blen);
		}

	      printf("%4d: %08x - EVENT NUMBER - evnum = %d\n",
		     iword,
		     data[iword],
		     data[iword]);
	      iword++;
	      for(ievword=1; ievword<evlen; ievword++)
		{
		  if(iword>blen)
		    {
		      rval = -1;
		      printf("----: **** -1: Data continues past Trigger Bank Length (%d) ****\n",blen);
		    }
		  printf("%4d: %08X - EVENT DATA\n",
			 iword,
			 data[iword]);
		  iword++;
		}
	    }
	}
    }

  printf("--------------------------------------------------------------------------------\n");
  return rval;
}
