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
  uint32_t trigger_time_l:24;
  uint32_t undef:3;
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
  uint32_t trigger_time_h:24;
  uint32_t undef:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} sspApv_trigger_time_2;

typedef union
{
  uint32_t raw;
  sspApv_trigger_time_2 bf;
} sspApv_trigger_time_2_t;

/* 5: MPD Frame */
typedef struct
{
  uint32_t mpd_id:5;
  uint32_t undef:11;
  uint32_t fiber:5;
  uint32_t _undef:6;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} sspApv_mpd_frame_1;

typedef union
{
  uint32_t raw;
  sspApv_mpd_frame_1 bf;
} sspApv_mpd_frame_1_t;

/* 5: APV Data */
typedef struct
{
  uint32_t apv_sample0:13;
  uint32_t apv_sample1:13;
  uint32_t apv_channel_num_40:5;
  uint32_t data_type_defining:1;
} sspApv_apv_data_1;

typedef union
{
  uint32_t raw;
  sspApv_apv_data_1 bf;
} sspApv_apv_data_1_t;

typedef struct
{
  uint32_t apv_sample2:13;
  uint32_t apv_sample3:13;
  uint32_t apv_channel_num_65:5;
  uint32_t data_type_defining:1;
} sspApv_apv_data_2;

typedef union
{
  uint32_t raw;
  sspApv_apv_data_2 bf;
} sspApv_apv_data_2_t;

typedef struct
{
  uint32_t apv_sample4:13;
  uint32_t apv_sample5:13;
  uint32_t apv_id:5;
  uint32_t data_type_defining:1;
} sspApv_apv_data_3;

typedef union
{
  uint32_t raw;
  sspApv_apv_data_3 bf;
} sspApv_apv_data_3_t;



void  sspApvDataDecode(uint32_t data);


#endif /* __SSPAPVDEC__ */
