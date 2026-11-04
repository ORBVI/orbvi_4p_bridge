// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from orbvi_4p_bridge:msg/FrameMetadata.idl
// generated code does not contain a copyright notice

#ifndef ORBVI_4P_BRIDGE__MSG__DETAIL__FRAME_METADATA__BUILDER_HPP_
#define ORBVI_4P_BRIDGE__MSG__DETAIL__FRAME_METADATA__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "orbvi_4p_bridge/msg/detail/frame_metadata__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace orbvi_4p_bridge
{

namespace msg
{

namespace builder
{

class Init_FrameMetadata_sensor_timestamp_ns
{
public:
  explicit Init_FrameMetadata_sensor_timestamp_ns(::orbvi_4p_bridge::msg::FrameMetadata & msg)
  : msg_(msg)
  {}
  ::orbvi_4p_bridge::msg::FrameMetadata sensor_timestamp_ns(::orbvi_4p_bridge::msg::FrameMetadata::_sensor_timestamp_ns_type arg)
  {
    msg_.sensor_timestamp_ns = std::move(arg);
    return std::move(msg_);
  }

private:
  ::orbvi_4p_bridge::msg::FrameMetadata msg_;
};

class Init_FrameMetadata_sequence
{
public:
  explicit Init_FrameMetadata_sequence(::orbvi_4p_bridge::msg::FrameMetadata & msg)
  : msg_(msg)
  {}
  Init_FrameMetadata_sensor_timestamp_ns sequence(::orbvi_4p_bridge::msg::FrameMetadata::_sequence_type arg)
  {
    msg_.sequence = std::move(arg);
    return Init_FrameMetadata_sensor_timestamp_ns(msg_);
  }

private:
  ::orbvi_4p_bridge::msg::FrameMetadata msg_;
};

class Init_FrameMetadata_header
{
public:
  Init_FrameMetadata_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_FrameMetadata_sequence header(::orbvi_4p_bridge::msg::FrameMetadata::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_FrameMetadata_sequence(msg_);
  }

private:
  ::orbvi_4p_bridge::msg::FrameMetadata msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::orbvi_4p_bridge::msg::FrameMetadata>()
{
  return orbvi_4p_bridge::msg::builder::Init_FrameMetadata_header();
}

}  // namespace orbvi_4p_bridge

#endif  // ORBVI_4P_BRIDGE__MSG__DETAIL__FRAME_METADATA__BUILDER_HPP_
