// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from orbvi_4p_bridge:msg/FrameMetadata.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "orbvi_4p_bridge/msg/detail/frame_metadata__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace orbvi_4p_bridge
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void FrameMetadata_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) orbvi_4p_bridge::msg::FrameMetadata(_init);
}

void FrameMetadata_fini_function(void * message_memory)
{
  auto typed_message = static_cast<orbvi_4p_bridge::msg::FrameMetadata *>(message_memory);
  typed_message->~FrameMetadata();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember FrameMetadata_message_member_array[3] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(orbvi_4p_bridge::msg::FrameMetadata, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "sequence",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT64,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(orbvi_4p_bridge::msg::FrameMetadata, sequence),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "sensor_timestamp_ns",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT64,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(orbvi_4p_bridge::msg::FrameMetadata, sensor_timestamp_ns),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers FrameMetadata_message_members = {
  "orbvi_4p_bridge::msg",  // message namespace
  "FrameMetadata",  // message name
  3,  // number of fields
  sizeof(orbvi_4p_bridge::msg::FrameMetadata),
  FrameMetadata_message_member_array,  // message members
  FrameMetadata_init_function,  // function to initialize message memory (memory has to be allocated)
  FrameMetadata_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t FrameMetadata_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &FrameMetadata_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace orbvi_4p_bridge


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<orbvi_4p_bridge::msg::FrameMetadata>()
{
  return &::orbvi_4p_bridge::msg::rosidl_typesupport_introspection_cpp::FrameMetadata_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, orbvi_4p_bridge, msg, FrameMetadata)() {
  return &::orbvi_4p_bridge::msg::rosidl_typesupport_introspection_cpp::FrameMetadata_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
