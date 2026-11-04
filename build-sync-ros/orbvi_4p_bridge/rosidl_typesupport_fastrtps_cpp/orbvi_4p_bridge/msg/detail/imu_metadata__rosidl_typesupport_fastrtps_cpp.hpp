// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from orbvi_4p_bridge:msg/ImuMetadata.idl
// generated code does not contain a copyright notice

#ifndef ORBVI_4P_BRIDGE__MSG__DETAIL__IMU_METADATA__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define ORBVI_4P_BRIDGE__MSG__DETAIL__IMU_METADATA__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "orbvi_4p_bridge/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "orbvi_4p_bridge/msg/detail/imu_metadata__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace orbvi_4p_bridge
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_orbvi_4p_bridge
cdr_serialize(
  const orbvi_4p_bridge::msg::ImuMetadata & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_orbvi_4p_bridge
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  orbvi_4p_bridge::msg::ImuMetadata & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_orbvi_4p_bridge
get_serialized_size(
  const orbvi_4p_bridge::msg::ImuMetadata & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_orbvi_4p_bridge
max_serialized_size_ImuMetadata(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace orbvi_4p_bridge

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_orbvi_4p_bridge
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, orbvi_4p_bridge, msg, ImuMetadata)();

#ifdef __cplusplus
}
#endif

#endif  // ORBVI_4P_BRIDGE__MSG__DETAIL__IMU_METADATA__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
