#ifndef __JLABDEC__
#define __JLABDEC__
#include <stdint.h>

typedef struct
{
  uint32_t undef:27;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} generic_data_word;

/* 0: BLOCK HEADER */
typedef struct
{
  uint32_t number_of_events_in_block:8;
  uint32_t event_block_number:10;
  uint32_t module_ID:4;
  uint32_t slot_number:5;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;  
} block_header;

/* 1: BLOCK TRAILER */
typedef struct
{
  uint32_t words_in_block:27;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;  
} block_trailer;

/* 2: EVENT HEADER */
typedef struct
{
  uint32_t event_number:22;
  uint32_t slot_number:5;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;  
} event_header;

/* 13: EVENT TRAILER */
typedef struct
{
  uint32_t undef:22;
  uint32_t slot_number:5;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;  
} event_trailer;

/* 14: DATA NOT VALID */
typedef struct
{
  uint32_t undef:22;
  uint32_t slot_number:5;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;  
} data_not_valid;

/* 15: FILLER */
typedef struct
{
  uint32_t undef:22;
  uint32_t slot_number:5;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;  
} filler_word;

#endif /* __JLABDEC__ */
