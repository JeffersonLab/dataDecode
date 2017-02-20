#include <stdio.h>
#include <stdint.h>
#include "fa125dec.h"

struct data_struct fadc_data;

void 
fa125DecodeData(uint32_t data)
{
  /* for new data format - 10/23/13 - EJ */

  static uint32_t type_last = 15;/* initialize to type FILLER WORD */
  static uint32_t time_last = 0;
  static uint32_t scaler_index = 0;
  static uint32_t num_scalers = 1;
  
  static uint32_t slot_id_ev_hd = 0;
  static uint32_t slot_id_dnv = 0;
  static uint32_t slot_id_fill = 0;

  static int nsamples=0;
  static int ipk=0;
/*   static int goto_raw=0; */

  int i_print =1;
  
  if( scaler_index )/* scaler data word */
    {
      fadc_data.type = 16;/* scaler data words as type 16 */
      fadc_data.new_type = 0;
      if( scaler_index < num_scalers )
	{ 
	  fadc_data.scaler[scaler_index - 1] = data;
	  if( i_print ) 
	    printf("%8X - SCALER(%d) = %d\n", data, (scaler_index - 1), data);
	  scaler_index++;
	}
      else/* last scaler word */
	{ 
	  fadc_data.scaler[scaler_index - 1] = data;
	  if( i_print ) 
	    printf("%8X - SCALER(%d) = %d\n", data, (scaler_index - 1), data);
	  scaler_index = 0;
	  num_scalers = 1;
	} 
    }
  else/* non-scaler word */
    {
      if( data & 0x80000000 )/* data type defining word */
	{
	  fadc_data.new_type = 1;
	  fadc_data.type = (data & 0x78000000) >> 27;
	}
      else/* data type continuation word */
	{
	  fadc_data.new_type = 0;
	  fadc_data.type = type_last;
	}
        
      switch( fadc_data.type )
	{
	case 0:/* BLOCK HEADER */
	  fadc_data.slot_id_hd = (data & 0x7C00000) >> 22;
	  fadc_data.mod_id_hd =  (data &  0x3C0000) >> 18;
	  fadc_data.n_evts =  (data & 0x000FF);
	  fadc_data.blk_num = (data & 0x7F00) >> 8;
	  if( i_print ) 
	    printf("%8X - BLOCK HEADER - slot = %d  id = %d  n_evts = %d  n_blk = %d\n",
		   data, fadc_data.slot_id_hd, fadc_data.mod_id_hd, fadc_data.n_evts, fadc_data.blk_num);
	  break;

	case 1:/* BLOCK TRAILER */
	  fadc_data.slot_id_tr = (data & 0x7C00000) >> 22;
	  fadc_data.n_words = (data & 0x3FFFFF);
	  if( i_print ) 
	    printf("%8X - BLOCK TRAILER - slot = %d   n_words = %d\n",
		   data, fadc_data.slot_id_tr, fadc_data.n_words);
	  break;

	case 2:/* EVENT HEADER */
	  if( fadc_data.new_type )
	    {
	      slot_id_ev_hd        = (data & 0x7C00000) >> 22;
	      fadc_data.evt_num_1 =  (data & 0x03FFFFF);
	      if( i_print ) 
		printf("%8X - EVENT HEADER - slot = %d  evt_num = %d\n", 
		       data, slot_id_ev_hd, fadc_data.evt_num_1);
	    }    
	  break;

	case 3:/* TRIGGER TIME */
	  if( fadc_data.new_type )
	    {
	      fadc_data.time_1 = (data & 0xFFFFFF);
	      if( i_print ) 
		printf("%8X - TRIGGER TIME 1 - time = 0x%08x\n", data, fadc_data.time_1);
	      fadc_data.time_now = 1;
	      time_last = 1;
	    }    
	  else
	    {
	      if( time_last == 1 )
		{
		  fadc_data.time_2 = (data & 0xFFFFFF);
		  if( i_print ) 
		    printf("%8X - TRIGGER TIME 2 - time = 0x%08x\n", data, fadc_data.time_2);
		  fadc_data.time_now = 2;
		}    
	      else
		if( i_print ) 
		  printf("%8X - TRIGGER TIME - (ERROR)\n", data);
	                      
	      time_last = fadc_data.time_now;
	    }    
	  break;

	case 4:/* WINDOW RAW DATA */
          if( fadc_data.new_type )
            {
              fadc_data.chan = (data & 0x7F00000) >> 20;
	      fadc_data.width = (data & 0xFFF);
              if( i_print ) 
		printf("%8X - WINDOW RAW DATA - chan = %2d   width = %d\n", 
		       data, fadc_data.chan, fadc_data.width);
              nsamples=0;
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
		printf("%8X - RAW SAMPLES (%3d) - valid = %d  adc = %4d (%03X)  valid = %d  adc = %4d (%03X)\n", 
		       data, nsamples,fadc_data.valid_1, fadc_data.adc_1, fadc_data.adc_1, 
		       fadc_data.valid_2, fadc_data.adc_2, fadc_data.adc_2);
              nsamples += 2;
            }    
          break;

	case 5:/* PULSE DATA, CDC */
	  if( fadc_data.new_type )
	    {
	      fadc_data.chan = (data & 0x7F00000) >> 20;
	      fadc_data.npk  = (data & 0xF8000)>>15;
	      fadc_data.le_time = (data & 0x7FF0)>>4;
	      fadc_data.time_quality = (data & (1<<3))>>3;
	      fadc_data.overflow_cnt = (data & 0x7);
	      if( i_print ) 
		printf("%8X - PULSE DATA (CDC IT) - chan = %2d  LE time = %d  Q = %d  OVF = %d\n", 
		       data, fadc_data.chan, fadc_data.le_time,
		       fadc_data.time_quality, fadc_data.overflow_cnt);
	    }    
	  else
	    {
	      fadc_data.pedestal = (data & 0x7F800000)>>23;
	      fadc_data.integral = (data & 0x007FFE00)>>9;
	      fadc_data.fm_amplitude = (data & 0x000001FF);
	      if( i_print ) 
		printf("%8X - PULSE DATA (CDC IT) - ped = %d  integral = %d  firstmax ampl = %d\n", 
		       data, fadc_data.pedestal, fadc_data.integral, fadc_data.fm_amplitude);
	    }    
	  break;

	case 6:/* PULSE DATA, FDC - Integral and Time */
	  if( fadc_data.new_type )
	    {
	      fadc_data.chan = (data & 0x7F00000) >> 20;
	      fadc_data.npk  = (data & 0xF8000)>>15;
	      fadc_data.le_time = (data & 0x7FF0)>>4;
	      fadc_data.time_quality = (data & (1<<3))>>3;
	      fadc_data.overflow_cnt = (data & 0x7);
	      ipk = 0;
	      if( i_print ) 
		printf("%8X - PULSE DATA (FDC IT) - chan = %2d  NPK = %d  LE time = %d  Q = %d  OVF = %d\n", 
		       data, fadc_data.chan, fadc_data.npk, fadc_data.le_time,
		       fadc_data.time_quality, fadc_data.overflow_cnt);
	    }    
	  else
	    {
	      ipk++;
	      fadc_data.pedestal = (data & 0x7F800000)>>23;
	      fadc_data.integral = (data & 0x007FFE00)>>9;
	      fadc_data.fm_amplitude = (data & 0x000001FF);
	      if( i_print ) 
		printf("%8X - PULSE DATA (FDC IT) %d - ped = %d  integral = %d  firstmax ampl = %d\n", 
		       data, ipk, fadc_data.pedestal, fadc_data.integral, fadc_data.fm_amplitude);
	    }    
	  break;

	case 9:/* PULSE DATA, FDC - Peak Ampl and Time */
	  if( fadc_data.new_type )
	    {
	      fadc_data.chan = (data & 0x7F00000) >> 20;
	      fadc_data.le_time = (data & 0x7FF0)>>4;
	      fadc_data.time_quality = (data & (1<<3))>>3;
	      fadc_data.overflow_cnt = (data & 0x7);
	      ipk = 0;
	      if( i_print ) 
		printf("%8X - PULSE DATA (FDC AT) - chan = %2d  NPK = %d  LE time = %d  Q = %d  OVF = %d\n", 
		       data, fadc_data.chan, fadc_data.npk, fadc_data.le_time,
		       fadc_data.time_quality, fadc_data.overflow_cnt);
	    }    
	  else
	    {
	      ipk++;
	      fadc_data.peak_amplitude = (data & 0x7ff80000)>>19;
	      fadc_data.peak_time = (data & 0x0007f800)>>11;
	      fadc_data.pedestal = (data & 0x000007ff);
	      if( i_print ) 
		printf("%8X - PULSE DATA (FDC AT) %d - Ampl = %d  Time = %d  Pedestal = %d\n", 
		       data, ipk, fadc_data.peak_amplitude, fadc_data.peak_time, fadc_data.pedestal);
	    }    
	  break;

	case 7:
	case 8:
	case 10:
	case 11:
	case 12:/* UNDEFINED TYPE */
	  if( i_print ) 
	    printf("%8X - UNDEFINED TYPE = %d\n", data, fadc_data.type);
	  break;
	              
	case 13:/* END OF EVENT */
	  if( i_print ) 
	    printf("%8X - END OF EVENT = %d\n", data, fadc_data.type);
	  break;

	case 14:/* DATA NOT VALID (no data available) */
	  slot_id_dnv = (data & 0x7C00000) >> 22; 
	  if( i_print )
	    printf("%8X - DATA NOT VALID = %d  slot = %d\n", data, fadc_data.type, slot_id_dnv);
	  break;

	case 15:/* FILLER WORD */
	  slot_id_fill = (data & 0x7C00000) >> 22; 
	  if( i_print ) 
	    printf("%8X - FILLER WORD = %d  slot = %d\n", data, fadc_data.type, slot_id_fill);
	  break;
	}
      
      type_last = fadc_data.type;    /* save type of current data word */
      
    }

}
