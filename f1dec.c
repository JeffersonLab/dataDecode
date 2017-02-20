#include <stdio.h>
#include <stdint.h>
#include "fa125dec.h"

void 
f1DataDecode(int id, uint32_t data)
{
  static uint32_t type_last = 15;	/* initialize to type FILLER WORD */
	
  uint32_t data_type, slot_id_hd, slot_id_tr, blk_evts, blk_num, blk_words;
  uint32_t new_type, evt_num, time_1, time_2, mod_id;
  int rev=0;
  int mode=0, factor=0;

  /* Need to replace this */
  /* rev = (f1Rev[id] & F1_VERSION_BOARDREV_MASK)>>8; */

  if(rev==2)
    mode = 1;

  factor = 2 - mode;
	
  if( data & 0x80000000 )		/* data type defining word */
    {
      new_type = 1;
      data_type = (data & 0x78000000) >> 27;
    }
  else
    {
      new_type = 0;
      data_type = type_last;
    }
        
  switch( data_type )
    {
    case 0:		/* BLOCK HEADER */
      slot_id_hd = (data & 0x7C00000) >> 22;
      mod_id = (data & 0x003C0000) >> 18;
      blk_num = (data & 0x3FF00) >> 8;
      blk_evts = (data & 0xFF);
      printf("      %08X - BLOCK HEADER  - slot = %u  id = %u  blk_evts = %u   n_blk = %u\n",
	     data, slot_id_hd, mod_id, blk_evts, blk_num);
      break;

    case 1:		/* BLOCK TRAILER */
      slot_id_tr = (data & 0x7C00000) >> 22;
      blk_words = (data & 0x3FFFFF);
      printf("      %08X - BLOCK TRAILER - slot = %u   n_words = %u\n",
	     data, slot_id_tr, blk_words);
      break;

    case 2:		/* EVENT HEADER */
      evt_num = (data & 0x3FFFFF);
      printf("      %08X - EVENT HEADER - evt_num = %u\n", data, evt_num);
      break;

    case 3:		/* TRIGGER TIME */
      if( new_type )
	{
	  time_1 = (data & 0xFFFFFF);
	  printf("      %08X - TRIGGER TIME 1 - time = %u\n", data, time_1);
	  type_last = 3;
	}    
      else
	{
	  if( type_last == 3 )
	    {
	      time_2 = (data & 0xFFFF);
	      printf("      %08X - TRIGGER TIME 2 - time = %u\n", data, time_2);
	    }    
	  else
	    printf("      %08X - TRIGGER TIME - (ERROR)\n", data);	                
	}    
      break;

    case 7:		/* EVENT DATA */
      printf("TDC = %08X   ED   ERR=%X  fake = %u  chip=%u  chan=%u  t = %u ", 
	     data, 
	     ((data >> 24) & 0x7), // ERR
	     ((data >> 22) & 1),   // Fake data flag
	     ((data >> 19) & 0x7), // chip
	     ((data >> 16) & 0x7), // chan
	     (data & 0xFFFF)); // t
      printf("(%u ps)\n", 
	     ( (data & 0xFFFF) * 56 * factor ));
      break;

    case 8:		/* CHIP HEADER */
      /* need 2 prints - maximum # of variables is 7 in VxWorks printf (?) */
      printf("TDC = %08X --CH-- (%X,%u)  chip=%u  chan=%u  trig = %u  t = %3u ", 
	     data, 
	     ((data >> 24) & 0x7), 
	     ((data >> 6) & 0x1), 
	     ((data >> 3) & 0x7), // chip
	     (data & 0x7),  //chan
	     ((data >> 16) & 0x3F),  // trig
	     ((data >> 7) & 0x1FF)); // t
      printf("(%u ps) (P=%u)\n", 
	     ( ( (data >> 7) & 0x1FF) * 56 * factor * 128 ),
	     ((data >> 6) & 0x1));
      break;

    case 14:		/* DATA NOT VALID (no data available) */
      printf("      %08X - DATA NOT VALID = %u\n", data, data_type);
      break;
    case 15:		/* FILLER WORD */
      printf("      %08X - FILLER WORD = %u\n", data, data_type);
      break;
       	        
    case 4:		/* UNDEFINED TYPE */
    case 5:		/* UNDEFINED TYPE */
    case 6:		/* UNDEFINED TYPE */
    case 9:		/* UNDEFINED TYPE */
    case 10:		/* UNDEFINED TYPE */
    case 11:		/* UNDEFINED TYPE */
    case 12:		/* UNDEFINED TYPE */
    case 13:		/* UNDEFINED TYPE */
    default:
      printf("      %08X - UNDEFINED TYPE = %u\n", data, data_type);
      break;
    }
	        
}        
