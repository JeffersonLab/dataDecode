#ifndef __FA125DEC__
#define __FA125DEC__
#include <stdint.h>

typedef struct
{
  uint32_t trigger_number:27;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;  
} fa125_event_header;

typedef struct
{
  uint32_t T_C:8;
  uint32_t T_B:8;
  uint32_t T_A:8;
  uint32_t undef:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa125_trigger_time_1;

typedef struct
{
  uint32_t T_F:8;
  uint32_t T_E:8;
  uint32_t T_D:8;
  uint32_t undef:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa125_trigger_time_2;

struct data_struct 
{
  uint32_t new_type;
  uint32_t type;
  uint32_t slot_id_hd;
  uint32_t mod_id_hd;
  uint32_t slot_id_tr;
  uint32_t n_evts;
  uint32_t blk_num;
  uint32_t n_words;
  uint32_t evt_num_1;
  uint32_t time_now;
  uint32_t time_1;
  uint32_t time_2;
  uint32_t chan;
  uint32_t width;
  uint32_t npk;
  uint32_t le_time;
  uint32_t time_quality;
  uint32_t overflow_cnt;
  uint32_t pedestal;
  uint32_t fm_amplitude;
  uint32_t peak_amplitude;
  uint32_t peak_time;
  uint32_t valid_1;
  uint32_t adc_1;
  uint32_t valid_2;
  uint32_t adc_2;
  uint32_t over;
  uint32_t adc_sum;
  uint32_t pulse_num;
  uint32_t thres_bin;
  uint32_t quality;
  uint32_t integral;
  uint32_t time;
  uint32_t chan_a;
  uint32_t source_a;
  uint32_t chan_b;
  uint32_t source_b;
  uint32_t group;
  uint32_t time_coarse;  
  uint32_t time_fine;
  uint32_t vmin;
  uint32_t vpeak;
  uint32_t scaler[18];/* data stream scalers */
};

void  fa125DecodeData(uint32_t data);


#endif /* __FA125DEC__ */
