#ifndef __SSPAPVDEC__
#define __SSPAPVDEC__
#include <stdint.h>

/* 2: EVENT HEADER */
typedef struct
{
  uint32_t trigger_number:27;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} sspApv_event_header;

typedef union
{
  uint32_t raw;
  sspApv_event_header bf;
} sspApv_event_header_t;

/* 3: TRIGGER TIME */
typedef struct
{
  uint32_t T_F:8;
  uint32_t T_E:8;
  uint32_t T_D:8;
  uint32_t T_C:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} sspApv_trigger_time_1;

typedef union
{
  uint32_t raw;
  sspApv_trigger_time_1 bf;
} sspApv_trigger_time_1_t;

typedef struct
{
  uint32_t T_C:8;
  uint32_t T_B:8;
  uint32_t T_A:8;
  uint32_t undef:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} sspApv_trigger_time_2;

typedef union
{
  uint32_t raw;
  sspApv_trigger_time_2 bf;
} sspApv_trigger_time_2_t;

/* 4: WINDOW RAW DATA */
typedef struct
{
  uint32_t PTW:9;
  uint32_t undef:14;
  uint32_t channel_number:4;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} sspApv_window_raw_data_1;

typedef union
{
  uint32_t raw;
  sspApv_window_raw_data_1 bf;
} sspApv_window_raw_data_1_t;

typedef struct
{
  uint32_t adc_sample_2:13;
  uint32_t invalid_2:1;
  uint32_t undef2:2;
  uint32_t adc_sample_1:13;
  uint32_t invalid_1:1;
  uint32_t undef1:1;
  uint32_t data_type_defining:1;
} sspApv_window_raw_data_n;

typedef union
{
  uint32_t raw;
  sspApv_window_raw_data_n bf;
} sspApv_window_raw_data_n_t;



void  sspApvDataDecode(uint32_t data);


#endif /* __SSPAPVDEC__ */
