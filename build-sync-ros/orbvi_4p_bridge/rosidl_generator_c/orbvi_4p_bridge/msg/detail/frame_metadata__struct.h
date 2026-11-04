// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from orbvi_4p_bridge:msg/FrameMetadata.idl
// generated code does not contain a copyright notice

#ifndef ORBVI_4P_BRIDGE__MSG__DETAIL__FRAME_METADATA__STRUCT_H_
#define ORBVI_4P_BRIDGE__MSG__DETAIL__FRAME_METADATA__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"

/// Struct defined in msg/FrameMetadata in the package orbvi_4p_bridge.
typedef struct orbvi_4p_bridge__msg__FrameMetadata
{
  std_msgs__msg__Header header;
  uint64_t sequence;
  int64_t sensor_timestamp_ns;
} orbvi_4p_bridge__msg__FrameMetadata;

// Struct for a sequence of orbvi_4p_bridge__msg__FrameMetadata.
typedef struct orbvi_4p_bridge__msg__FrameMetadata__Sequence
{
  orbvi_4p_bridge__msg__FrameMetadata * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} orbvi_4p_bridge__msg__FrameMetadata__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ORBVI_4P_BRIDGE__MSG__DETAIL__FRAME_METADATA__STRUCT_H_
