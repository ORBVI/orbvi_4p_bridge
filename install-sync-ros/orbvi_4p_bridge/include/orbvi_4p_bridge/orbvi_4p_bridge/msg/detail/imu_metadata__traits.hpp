// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from orbvi_4p_bridge:msg/ImuMetadata.idl
// generated code does not contain a copyright notice

#ifndef ORBVI_4P_BRIDGE__MSG__DETAIL__IMU_METADATA__TRAITS_HPP_
#define ORBVI_4P_BRIDGE__MSG__DETAIL__IMU_METADATA__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "orbvi_4p_bridge/msg/detail/imu_metadata__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace orbvi_4p_bridge
{

namespace msg
{

inline void to_flow_style_yaml(
  const ImuMetadata & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: sequence
  {
    out << "sequence: ";
    rosidl_generator_traits::value_to_yaml(msg.sequence, out);
    out << ", ";
  }

  // member: accel_timestamp_ns
  {
    out << "accel_timestamp_ns: ";
    rosidl_generator_traits::value_to_yaml(msg.accel_timestamp_ns, out);
    out << ", ";
  }

  // member: gyro_timestamp_ns
  {
    out << "gyro_timestamp_ns: ";
    rosidl_generator_traits::value_to_yaml(msg.gyro_timestamp_ns, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const ImuMetadata & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: sequence
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "sequence: ";
    rosidl_generator_traits::value_to_yaml(msg.sequence, out);
    out << "\n";
  }

  // member: accel_timestamp_ns
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "accel_timestamp_ns: ";
    rosidl_generator_traits::value_to_yaml(msg.accel_timestamp_ns, out);
    out << "\n";
  }

  // member: gyro_timestamp_ns
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "gyro_timestamp_ns: ";
    rosidl_generator_traits::value_to_yaml(msg.gyro_timestamp_ns, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const ImuMetadata & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace orbvi_4p_bridge

namespace rosidl_generator_traits
{

[[deprecated("use orbvi_4p_bridge::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const orbvi_4p_bridge::msg::ImuMetadata & msg,
  std::ostream & out, size_t indentation = 0)
{
  orbvi_4p_bridge::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use orbvi_4p_bridge::msg::to_yaml() instead")]]
inline std::string to_yaml(const orbvi_4p_bridge::msg::ImuMetadata & msg)
{
  return orbvi_4p_bridge::msg::to_yaml(msg);
}

template<>
inline const char * data_type<orbvi_4p_bridge::msg::ImuMetadata>()
{
  return "orbvi_4p_bridge::msg::ImuMetadata";
}

template<>
inline const char * name<orbvi_4p_bridge::msg::ImuMetadata>()
{
  return "orbvi_4p_bridge/msg/ImuMetadata";
}

template<>
struct has_fixed_size<orbvi_4p_bridge::msg::ImuMetadata>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<orbvi_4p_bridge::msg::ImuMetadata>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<orbvi_4p_bridge::msg::ImuMetadata>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ORBVI_4P_BRIDGE__MSG__DETAIL__IMU_METADATA__TRAITS_HPP_
