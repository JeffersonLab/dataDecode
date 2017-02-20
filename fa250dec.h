#ifndef __FA250DEC__
#define __FA250DEC__
#include <stdint.h>

typedef struct
{
  uint32_t trigger_number:12;
  uint32_t trigger_time:10
  uint32_t slot_number:5;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;  
} fa250_event_header;

typedef struct
{
  uint32_t T_F:8;
  uint32_t T_E:8;
  uint32_t T_D:8;
  uint32_t T_C:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_trigger_time_1;

typedef struct
{
  uint32_t T_C:8;
  uint32_t T_B:8;
  uint32_t T_A:8;
  uint32_t undef:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_trigger_time_2;

typedef struct
{
  uint32_t PTW:9;
  uint32_t undef:14;
  uint32_t channel_number:4;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_window_raw_data_1;

typedef struct
{
  uint32_t adc_sample_2:13;
  uint32_t invalid_2:1;
  uint32_t undef2:2;
  uint32_t adc_sample_1:13;
  uint32_t invalid_1:1;
  uint32_t undef1:1;
  uint32_t data_type_defining:1;
} fa250_window_raw_data_n;

typedef struct
{
  uint32_t pedestal_sum:14;
  uint32_t pedestal_quality:1;
  uint32_t channel_number:4;
  uint32_t event_number_within_block:8;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_pulse_parameters_1;

typedef struct
{
  uint32_t pulse_samples_over_threshold:9;
  uint32_t underflow:1;
  uint32_t overflow:1;
  uint32_t NSA_beyond_PTW:1;
  uint32_t sample_sum:18;
  uint32_t parameter_type_tag:1;
  uint32_t data_type_defining:1;
} fa250_pulse_parameters_2;

typedef struct
{
  uint32_t samples_over_threshold:1;
  uint32_t vpeak_unfound:1;
  uint32_t vpeak_beyond:1;
  uint32_t pulse_peak:12;
  uint32_t fine_time:6;
  uint32_t coarse_time:9;
  uint32_t parameter_type_tag:1;
  uint32_t data_type_defining:1;
} fa250_pulse_parameters_3;

struct 
fadc_data_struct 
{
  uint32_t new_type;	
  uint32_t type;	
  uint32_t slot_id_hd;
  uint32_t slot_id_tr;
  uint32_t slot_id_evh;
  uint32_t n_evts;
  uint32_t blk_num;
  uint32_t modID;
  uint32_t PL;
  uint32_t NSB;
  uint32_t NSA;
  uint32_t n_words;
  uint32_t evt_num_1;
  uint32_t evt_num_2;
  uint32_t evt_of_blk;
  uint32_t time_now;
  uint32_t time_low_10;
  uint32_t time_1;
  uint32_t time_2;
  uint32_t time_3;
  uint32_t time_4;
  uint32_t chan;
  uint32_t width;
  uint32_t valid_1;
  uint32_t adc_1;
  uint32_t valid_2;
  uint32_t adc_2;
  uint32_t over;
  uint32_t under;
  uint32_t nsa_ext;
  uint32_t samp_ov_thres;
  uint32_t adc_sum;
  uint32_t ped_sum;
  uint32_t pulse_num;
  uint32_t thres_bin;
  uint32_t quality;
  uint32_t quality2;
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
  uint32_t trig_type_int;
  uint32_t trig_state_int;
  uint32_t evt_num_int;
  uint32_t err_status_int;
  uint32_t scaler_data_words;
};

void  faDataDecode(uint32_t data);


#endif /* __FA250DEC__ */
