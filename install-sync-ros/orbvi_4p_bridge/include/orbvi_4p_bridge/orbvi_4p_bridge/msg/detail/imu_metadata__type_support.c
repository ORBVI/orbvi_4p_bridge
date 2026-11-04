// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from orbvi_4p_bridge:msg/ImuMetadata.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "orbvi_4p_bridge/msg/detail/imu_metadata__rosidl_typesupport_introspection_c.h"
#include "orbvi_4p_bridge/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "orbvi_4p_bridge/msg/detail/imu_metadata__functions.h"
#include "orbvi_4p_bridge/msg/detail/imu_metadata__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  orbvi_4p_bridge__msg__ImuMetadata__init(message_memory);
}

void orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_fini_function(void * message_memory)
{
  orbvi_4p_bridge__msg__ImuMetadata__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_message_member_array[4] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(orbvi_4p_bridge__msg__ImuMetadata, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "sequence",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT64,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(orbvi_4p_bridge__msg__ImuMetadata, sequence),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "accel_timestamp_ns",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT64,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(orbvi_4p_bridge__msg__ImuMetadata, accel_timestamp_ns),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "gyro_timestamp_ns",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT64,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(orbvi_4p_bridge__msg__ImuMetadata, gyro_timestamp_ns),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_message_members = {
  "orbvi_4p_bridge__msg",  // message namespace
  "ImuMetadata",  // message name
  4,  // number of fields
  sizeof(orbvi_4p_bridge__msg__ImuMetadata),
  orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_message_member_array,  // message members
  orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_init_function,  // function to initialize message memory (memory has to be allocated)
  orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_message_type_support_handle = {
  0,
  &orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_orbvi_4p_bridge
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, orbvi_4p_bridge, msg, ImuMetadata)() {
  orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_message_type_support_handle.typesupport_identifier) {
    orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &orbvi_4p_bridge__msg__ImuMetadata__rosidl_typesupport_introspection_c__ImuMetadata_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
