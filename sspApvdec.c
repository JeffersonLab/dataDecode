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
  static int apv_data_word = 0;
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
		   d.bf.trigger_time_l);

	    time_last = 1;
	  }
	else
	  {
	    sspApv_trigger_time_2_t d; d.raw = data;
	    if( time_last == 1 )
	      {
		printf("%8X - TRIGGER TIME 2 - time = %08x\n",
		       d.raw,
		       d.bf.trigger_time_h);
	      }
	    else
	      printf("%8X - TRIGGER TIME - (ERROR)\n", data);

	    time_last = 0;
	  }
	break;
      }

    case 5:		/* MPD Frame */
      {
	if( new_type )
	  {
	    sspApv_mpd_frame_1_t d; d.raw = data;

	    printf("%8X - FIBER = %2d  MPD_ID = %2d\n",
		   d.raw,
		   d.bf.fiber,
		   d.bf.mpd_id);

	    apv_data_word = 1;
	  }
	else
	  {
	    switch(apv_data_word)
	      {
	      case 1:
		{
		  sspApv_apv_data_1_t d; d.raw = data;
		  printf("%8X - APV DATA 1 - CHANNEL_NUM(4:0) = %2d S1 = %4x  S0 = %4x\n",
			 d.raw,
			 d.bf.apv_channel_num_40,
			 d.bf.apv_sample1,
			 d.bf.apv_sample0);

		  apv_data_word++;
		  break;
		}
	      case 2:
		{
		  sspApv_apv_data_2_t d; d.raw = data;
		  printf("%8X - APV DATA 2 - CHANNEL_NUM(6:5) = %d S3 = %4x  S2 = %4x\n",
			 d.raw,
			 d.bf.apv_channel_num_65,
			 d.bf.apv_sample3,
			 d.bf.apv_sample2);

		  apv_data_word++;
		  break;
		}
	      case 3:
		{
		  sspApv_apv_data_3_t d; d.raw = data;
		  printf("%8X - APV DATA 3 - APV_ID = %2d S5 = %4x  S4 = %4x\n",
			 d.raw,
			 d.bf.apv_id,
			 d.bf.apv_sample5,
			 d.bf.apv_sample4);

		  apv_data_word=1;
		  break;
		}
	      default:
		break;
	      }

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
