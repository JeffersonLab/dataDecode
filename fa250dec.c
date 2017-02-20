#include <stdio.h>
#include <stdint.h>
#include "jlabdec.h"
#include "fa250dec.h"

struct fadc_data_struct fadc_data;

void 
faDataDecode(uint32_t data)
{
  int i_print = 1;
  static uint32_t type_last = 15;	/* initialize to type FILLER WORD */
  static uint32_t time_last = 0;
  int idata=0;

  if( data & 0x80000000 )		/* data type defining word */
    {
      fadc_data.new_type = 1;
      fadc_data.type = (data & 0x78000000) >> 27;
    }
  else
    {
      fadc_data.new_type = 0;
      fadc_data.type = type_last;
    }
        
  switch( fadc_data.type )
    {
    case 0:		/* BLOCK HEADER */
      if( fadc_data.new_type )
	{
	  fadc_data.slot_id_hd = ((data) & 0x7C00000) >> 22;
	  fadc_data.modID      = (data & 0x3C0000)>>18;
	  fadc_data.blk_num    = (data & 0x3FF00) >> 8;
	  fadc_data.n_evts     = (data & 0xFF);
	  if( i_print ) 
	    printf("%8X - BLOCK HEADER - slot = %d  modID = %d   n_evts = %d   n_blk = %d\n",
		   data, fadc_data.slot_id_hd, 
		   fadc_data.modID, fadc_data.n_evts, fadc_data.blk_num);
	}
      else
	{
	  fadc_data.PL  = (data & 0x1FFC0000) >> 18;
	  fadc_data.NSB = (data & 0x0003FE00) >> 9;
	  fadc_data.NSA = (data & 0x000001FF) >> 0;

	  printf("%8X - BLOCK HEADER 2 - PL = %d  NSB = %d  NSA = %d\n",
		 data, 
		 fadc_data.PL,
		 fadc_data.NSB,
		 fadc_data.NSA);
	}
      break;

    case 1:		/* BLOCK TRAILER */
      fadc_data.slot_id_tr = (data & 0x7C00000) >> 22;
      fadc_data.n_words = (data & 0xFFF);
      if( i_print ) 
	printf("%8X - BLOCK TRAILER - slot = %d   n_words = %d\n",
	       data, fadc_data.slot_id_tr, fadc_data.n_words);
      break;

    case 2:		/* EVENT HEADER */
      fadc_data.time_low_10 = (data & 0x003FF000) >> 12;
      fadc_data.evt_num_1 = (data & 0x3FFFFF);
      if( i_print ) 
	printf("%8X - EVENT HEADER 1 - trig time = %d   trig num = %d\n", data, 
	       fadc_data.time_low_10, fadc_data.evt_num_1);
      break;

    case 3:		/* TRIGGER TIME */
      if( fadc_data.new_type )
	{
	  fadc_data.time_1 = (data & 0x07FFFFFF);
	  if( i_print ) 
	    printf("%8X - TRIGGER TIME 1 - time = %08x\n", data, fadc_data.time_1);
	  fadc_data.time_now = 1;
	  time_last = 1;
	}    
      else
	{
	  if( time_last == 1 )
	    {
	      fadc_data.time_2 = (data & 0xFFFFFF);
	      if( i_print ) 
		printf("%8X - TRIGGER TIME 2 - time = %08x\n", data, fadc_data.time_2);
	      fadc_data.time_now = 2;
	    }    
	  else if( time_last == 2 )
	    {
	      fadc_data.time_3 = (data & 0xFFFFFF);
	      if( i_print ) 
		printf("%8X - TRIGGER TIME 3 - time = %08x\n", data, fadc_data.time_3);
	      fadc_data.time_now = 3;
	    }    
	  else if( time_last == 3 )
	    {
	      fadc_data.time_4 = (data & 0xFFFFFF);
	      if( i_print ) 
		printf("%8X - TRIGGER TIME 4 - time = %08x\n", data, fadc_data.time_4);
	      fadc_data.time_now = 4;
	    }    
	  else
	    if( i_print ) 
	      printf("%8X - TRIGGER TIME - (ERROR)\n", data);
	                
	  time_last = fadc_data.time_now;
	}    
      break;

    case 4:		/* WINDOW RAW DATA */
      if( fadc_data.new_type )
	{
	  fadc_data.chan = (data & 0x7800000) >> 23;
	  fadc_data.width = (data & 0xFFF);
	  if( i_print ) 
	    printf("%8X - WINDOW RAW DATA - chan = %d   nsamples = %d\n", 
		   data, fadc_data.chan, fadc_data.width);
	}    
      else
	{
	  fadc_data.valid_1 = 1;
	  fadc_data.valid_2 = 1;
	  fadc_data.adc_1 = (data & 0x1FFF0000) >> 16;
	  if( data & 0x20000000 )
	    fadc_data.valid_1 = 0;
	  fadc_data.adc_2 = (data & 0x1FFF);
	  if( data & 0x2000 )
	    fadc_data.valid_2 = 0;
	  if( i_print ) 
	    printf("%8X - RAW SAMPLES - valid = %d  adc = %4d   valid = %d  adc = %4d\n", 
		   data, fadc_data.valid_1, fadc_data.adc_1, 
		   fadc_data.valid_2, fadc_data.adc_2);
	}    
      break;
 
    case 5:		/* UNDEFINED TYPE */
      if( i_print ) 
	printf("%8X - UNDEFINED TYPE = %d\n", data, fadc_data.type);
      break;

    case 6:		/* PULSE RAW DATA */
      if( fadc_data.new_type )
	{
	  fadc_data.chan = (data & 0x7800000) >> 23;
	  fadc_data.pulse_num = (data & 0x600000) >> 21;
	  fadc_data.thres_bin = (data & 0x3FF);
	  if( i_print ) 
	    printf("%8X - PULSE RAW DATA - chan = %d   pulse # = %d   threshold bin = %d\n", 
		   data, fadc_data.chan, fadc_data.pulse_num, fadc_data.thres_bin);
	}    
      else
	{
	  fadc_data.valid_1 = 1;
	  fadc_data.valid_2 = 1;
	  fadc_data.adc_1 = (data & 0x1FFF0000) >> 16;
	  if( data & 0x20000000 )
	    fadc_data.valid_1 = 0;
	  fadc_data.adc_2 = (data & 0x1FFF);
	  if( data & 0x2000 )
	    fadc_data.valid_2 = 0;
	  if( i_print ) 
	    printf("%8X - PULSE RAW SAMPLES - valid = %d  adc = %d   valid = %d  adc = %d\n", 
		   data, fadc_data.valid_1, fadc_data.adc_1, 
		   fadc_data.valid_2, fadc_data.adc_2);
	}    
      break;

    case 7:		/* PULSE INTEGRAL */
      fadc_data.chan = (data & 0x7800000) >> 23;
      fadc_data.pulse_num = (data & 0x600000) >> 21;
      fadc_data.quality = (data & 0x180000) >> 19;
      fadc_data.integral = (data & 0x7FFFF);
      if( i_print ) 
	printf("%8X - PULSE INTEGRAL - chan = %d   pulse # = %d   quality = %d   integral = %d\n", 
	       data, fadc_data.chan, fadc_data.pulse_num, 
	       fadc_data.quality, fadc_data.integral);
      break;
 
    case 8:		/* PULSE TIME */
      fadc_data.chan = (data & 0x7800000) >> 23;
      fadc_data.pulse_num = (data & 0x600000) >> 21;
      fadc_data.quality = (data & 0x180000) >> 19;
      fadc_data.time = (data & 0xFFFF);
      if( i_print ) 
	printf("%8X - PULSE TIME - chan = %d   pulse # = %d   quality = %d   time = %d\n", 
	       data, fadc_data.chan, fadc_data.pulse_num, 
	       fadc_data.quality, fadc_data.time);
      break;

    case 9:		/* PULSE PARAMETERS */
      if( fadc_data.new_type )
	{ /* Channel ID and Pedestal Info */
	  fadc_data.pulse_num  = 0; /* Initialize */
	  fadc_data.evt_of_blk = (data & 0x07f80000)>>19;
	  fadc_data.chan       = (data & 0x00078000)>>15;
	  fadc_data.quality    = (data & (1<<14))>>14;
	  fadc_data.ped_sum    = (data & 0x00003fff);

	      printf("%8X - PULSEPARAM 1 - evt = %d   chan = %d   quality = %d   pedsum = %d\n", 
		 data, 
		 fadc_data.evt_of_blk, 
		 fadc_data.chan, 
		 fadc_data.quality, 
		 fadc_data.ped_sum);
	}
      else
	{
	  if(data & (1<<30))
	    { /* Word 1: Integral of n-th pulse in window */
	      fadc_data.pulse_num++;
	      fadc_data.adc_sum = (data & 0x3ffff000)>>12;
	      fadc_data.nsa_ext = (data & (1<<11))>>11;
	      fadc_data.over    = (data & (1<<10))>>10;
	      fadc_data.under   = (data & (1<<9))>>9;
	      fadc_data.samp_ov_thres = (data & 0x000001ff);

	      printf("%8X - PULSEPARAM 2 - P# = %d  Sum = %d  NSA+ = %d  Ov/Un = %d/%d  #OT = %d\n", 
		     data, 
		     fadc_data.pulse_num, 
		     fadc_data.adc_sum, 
		     fadc_data.nsa_ext, 
		     fadc_data.over, 
		     fadc_data.under, 
		     fadc_data.samp_ov_thres);
	    }
	  else
	    { /* Word 2: Time of n-th pulse in window */
	      fadc_data.time_coarse = (data & 0x3fe00000)>>21;
	      fadc_data.time_fine   = (data & 0x001f8000)>>15;
	      fadc_data.vpeak       = (data & 0x00007ff8)>>3;
	      fadc_data.quality     = (data & 0x2)>>1;
	      fadc_data.quality2    = (data & 0x1);

	      printf("%8X - PULSEPARAM 3 - CTime = %d  FTime = %d  Peak = %d  NoVp = %d  Q = %d\n", 
		     data, 
		     fadc_data.time_coarse, 
		     fadc_data.time_fine, 
		     fadc_data.vpeak, 
		     fadc_data.quality, 
		     fadc_data.quality2);
	    }
	}

      break;

    case 10:		/* UNDEFINED TYPE */
      if( i_print ) 
	printf("%8X - UNDEFINED TYPE = %d\n", data, fadc_data.type);
      break;

    case 11:		/* UNDEFINED TYPE */
      if( i_print ) 
	printf("%8X - UNDEFINED TYPE = %d\n", data, fadc_data.type);
      break;

    case 12:		/* SCALER HEADER */
      if( fadc_data.new_type )
	{
	  fadc_data.scaler_data_words = (data & 0x3F);
	  if( i_print ) 
	    printf("%8X - SCALER HEADER - data words = %d\n", data, fadc_data.scaler_data_words);
	}
      else
	{
	  for(idata=0; idata<fadc_data.scaler_data_words; idata++)
	    {
	      if( i_print ) 
		printf("%8X - SCALER DATA - word = %2d  counter = %d\n", 
		       data, idata, data);
	    }
	}
      break;
 
    case 13:		/* END OF EVENT */
      if( i_print ) 
	printf("%8X - END OF EVENT = %d\n", data, fadc_data.type);
      break;

    case 14:		/* DATA NOT VALID (no data available) */
      if( i_print ) 
	printf("%8X - DATA NOT VALID = %d\n", data, fadc_data.type);
      break;

    case 15:		/* FILLER WORD */
      if( i_print ) 
	printf("%8X - FILLER WORD = %d\n", data, fadc_data.type);
      break;
    }
	
  type_last = fadc_data.type;	/* save type of current data word */
		   
}        
