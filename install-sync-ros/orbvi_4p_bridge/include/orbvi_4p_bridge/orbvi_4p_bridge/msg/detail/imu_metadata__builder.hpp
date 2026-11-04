// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from orbvi_4p_bridge:msg/ImuMetadata.idl
// generated code does not contain a copyright notice

#ifndef ORBVI_4P_BRIDGE__MSG__DETAIL__IMU_METADATA__BUILDER_HPP_
#define ORBVI_4P_BRIDGE__MSG__DETAIL__IMU_METADATA__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "orbvi_4p_bridge/msg/detail/imu_metadata__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace orbvi_4p_bridge
{

namespace msg
{

namespace builder
{

class Init_ImuMetadata_gyro_timestamp_ns
{
public:
  explicit Init_ImuMetadata_gyro_timestamp_ns(::orbvi_4p_bridge::msg::ImuMetadata & msg)
  : msg_(msg)
  {}
  ::orbvi_4p_bridge::msg::ImuMetadata gyro_timestamp_ns(::orbvi_4p_bridge::msg::ImuMetadata::_gyro_timestamp_ns_type arg)
  {
    msg_.gyro_timestamp_ns = std::move(arg);
    return std::move(msg_);
  }

private:
  ::orbvi_4p_bridge::msg::ImuMetadata msg_;
};

class Init_ImuMetadata_accel_timestamp_ns
{
public:
  explicit Init_ImuMetadata_accel_timestamp_ns(::orbvi_4p_bridge::msg::ImuMetadata & msg)
  : msg_(msg)
  {}
  Init_ImuMetadata_gyro_timestamp_ns accel_timestamp_ns(::orbvi_4p_bridge::msg::ImuMetadata::_accel_timestamp_ns_type arg)
  {
    msg_.accel_timestamp_ns = std::move(arg);
    return Init_ImuMetadata_gyro_timestamp_ns(msg_);
  }

private:
  ::orbvi_4p_bridge::msg::ImuMetadata msg_;
};

class Init_ImuMetadata_sequence
{
public:
  explicit Init_ImuMetadata_sequence(::orbvi_4p_bridge::msg::ImuMetadata & msg)
  : msg_(msg)
  {}
  Init_ImuMetadata_accel_timestamp_ns sequence(::orbvi_4p_bridge::msg::ImuMetadata::_sequence_type arg)
  {
    msg_.sequence = std::move(arg);
    return Init_ImuMetadata_accel_timestamp_ns(msg_);
  }

private:
  ::orbvi_4p_bridge::msg::ImuMetadata msg_;
};

class Init_ImuMetadata_header
{
public:
  Init_ImuMetadata_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ImuMetadata_sequence header(::orbvi_4p_bridge::msg::ImuMetadata::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_ImuMetadata_sequence(msg_);
  }

private:
  ::orbvi_4p_bridge::msg::ImuMetadata msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::orbvi_4p_bridge::msg::ImuMetadata>()
{
  return orbvi_4p_bridge::msg::builder::Init_ImuMetadata_header();
}

}  // namespace orbvi_4p_bridge

#endif  // ORBVI_4P_BRIDGE__MSG__DETAIL__IMU_METADATA__BUILDER_HPP_
