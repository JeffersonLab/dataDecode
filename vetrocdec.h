#ifndef __VETROCDEC__
#define __VETROCDEC__
#include <stdint.h>

/* 2: EVENT HEADER */
typedef struct
{
  uint32_t trigger_number:27;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} vetroc_event_header;

typedef union
{
  uint32_t raw;
  vetroc_event_header bf;
} vetroc_event_header_t;

/* 3: TRIGGER TIME */
typedef struct
{
  uint32_t T_F:8;
  uint32_t T_E:8;
  uint32_t T_D:8;
  uint32_t undef:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} vetroc_trigger_time_1;

typedef union
{
  uint32_t raw;
  vetroc_trigger_time_1 bf;
} vetroc_trigger_time_1_t;

typedef struct
{
  uint32_t T_C:8;
  uint32_t T_B:8;
  uint32_t T_A:8;
  uint32_t undef:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} vetroc_trigger_time_2;

typedef union
{
  uint32_t raw;
  vetroc_trigger_time_2 bf;
} vetroc_trigger_time_2_t;

/* 8: PULSE TIME */
typedef struct
{
  uint32_t time:16;
  uint32_t channel:8;
  uint32_t undef:2;
  uint32_t edge:1;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} vetroc_pulse_time;

typedef union
{
  uint32_t raw;
  vetroc_pulse_time bf;
} vetroc_pulse_time_t;

void  vetrocDataDecode(uint32_t data);


#endif /* __VETROCDEC__ */
