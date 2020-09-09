#include <stdio.h>
#include <stdint.h>
#include "jlabdec.h"
#include "sspApvdec.h"

void
sspApvDataDecode(uint32_t data)
{
  static uint32_t type_last = 15;	/* initialize to type FILLER WORD */
  static uint32_t time_last = 0;
  static int new_type = 0;
  int type_current = 0;
  static int pulse_number = 0;
  static int isca = 0;
  generic_data_word_t gword;

  gword.raw = data;

  if(gword.bf.data_type_defining) /* data type defining word */
    {
      new_type = 1;
      type_current = gword.bf.data_type_tag;
    }
  else
    {
      new_type = 0;
      type_current = type_last;
    }

  switch( type_current )
    {
    case 0:		/* BLOCK HEADER */
      {
	block_header_t d; d.raw = data;

	printf("%8X - BLOCK HEADER - slot = %d  modID = %d   n_evts = %d   n_blk = %d\n",
	       d.raw,
	       d.bf.slot_number,
	       d.bf.module_ID,
	       d.bf.number_of_events_in_block,
	       d.bf.event_block_number);

	break;
      }

    case 1:		/* BLOCK TRAILER */
      {
	block_trailer_t d; d.raw = data;

	printf("%8X - BLOCK TRAILER - slot = %d   n_words = %d\n",
		     d.raw,
		     d.bf.slot_number,
		     d.bf.words_in_block);
	break;
      }

    case 2:		/* EVENT HEADER */
      {
	sspApv_event_header_t d; d.raw = data;

	printf("%8X - EVENT HEADER 1 - trig num = %d\n",
	       d.raw,
	       d.bf.trigger_number);
	break;
      }

    case 3:		/* TRIGGER TIME */
      {
	if( new_type )
	  {
	    sspApv_trigger_time_1_t d; d.raw = data;

	    printf("%8X - TRIGGER TIME 1 - time = %08x\n",
		   d.raw,
		   (d.bf.T_C<<24) | (d.bf.T_D<<16) | (d.bf.T_E<<8) | (d.bf.T_F) );

	    time_last = 1;
	  }
	else
	  {
	    sspApv_trigger_time_2_t d; d.raw = data;
	    if( time_last == 1 )
	      {
		printf("%8X - TRIGGER TIME 2 - time = %08x\n",
		       d.raw,
		       (d.bf.T_A<<16) | (d.bf.T_B<<8) | (d.bf.T_C) );
	      }
	    else
	      printf("%8X - TRIGGER TIME - (ERROR)\n", data);

	    time_last = 0;
	  }
	break;
      }


    case 14:		/* DATA NOT VALID (no data available) */
      {
	data_not_valid_t d; d.raw = data;

	printf("%8X - DATA NOT VALID = %d\n",
	       d.raw,
	       d.bf.data_type_tag);
	break;
      }

    case 15:		/* FILLER WORD */
      {
	filler_word_t d; d.raw = data;

	printf("%8X - FILLER WORD = %d\n",
	       d.raw,
	       d.bf.data_type_tag);
	break;
      }

    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    default:
      {
	printf("%8X - UNDEFINED TYPE = %d\n",
	       gword.raw,
	       gword.bf.data_type_tag);
	break;
      }

    }

  type_last = type_current;	/* save type of current data word */

}
