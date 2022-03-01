#pragma once
#include <stdint.h>

/* 2: EVENT HEADER */
typedef struct
{
  uint32_t trigger_number:12;
  uint32_t trigger_time:10;
  uint32_t slot_number:5;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} hd_event_header;

typedef union
{
  uint32_t raw;
  hd_event_header bf;
} hd_event_header_t;

/* 3: TRIGGER TIME */
typedef struct
{
  uint32_t T_F:8;
  uint32_t T_E:8;
  uint32_t T_D:8;
  uint32_t T_C:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} hd_trigger_time_1;

typedef union
{
  uint32_t raw;
  hd_trigger_time_1 bf;
} hd_trigger_time_1_t;

typedef struct
{
  uint32_t T_C:8;
  uint32_t T_B:8;
  uint32_t T_A:8;
  uint32_t undef:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} hd_trigger_time_2;

typedef union
{
  uint32_t raw;
  hd_trigger_time_2 bf;
} hd_trigger_time_2_t;

/* 8: Decoder Data */
typedef struct
{
  uint32_t nwords:6;
  uint32_t undef:21;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} hd_decoder_data;

typedef union
{
  uint32_t raw;
  hd_decoder_data bf;
} hd_decoder_data_t;

void  hdDataDecode(uint32_t data);
