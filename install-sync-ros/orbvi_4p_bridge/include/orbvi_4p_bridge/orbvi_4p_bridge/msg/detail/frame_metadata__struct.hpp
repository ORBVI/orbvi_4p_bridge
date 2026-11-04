// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from orbvi_4p_bridge:msg/FrameMetadata.idl
// generated code does not contain a copyright notice

#ifndef ORBVI_4P_BRIDGE__MSG__DETAIL__FRAME_METADATA__STRUCT_HPP_
#define ORBVI_4P_BRIDGE__MSG__DETAIL__FRAME_METADATA__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__orbvi_4p_bridge__msg__FrameMetadata __attribute__((deprecated))
#else
# define DEPRECATED__orbvi_4p_bridge__msg__FrameMetadata __declspec(deprecated)
#endif

namespace orbvi_4p_bridge
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct FrameMetadata_
{
  using Type = FrameMetadata_<ContainerAllocator>;

  explicit FrameMetadata_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->sequence = 0ull;
      this->sensor_timestamp_ns = 0ll;
    }
  }

  explicit FrameMetadata_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->sequence = 0ull;
      this->sensor_timestamp_ns = 0ll;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _sequence_type =
    uint64_t;
  _sequence_type sequence;
  using _sensor_timestamp_ns_type =
    int64_t;
  _sensor_timestamp_ns_type sensor_timestamp_ns;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__sequence(
    const uint64_t & _arg)
  {
    this->sequence = _arg;
    return *this;
  }
  Type & set__sensor_timestamp_ns(
    const int64_t & _arg)
  {
    this->sensor_timestamp_ns = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    orbvi_4p_bridge::msg::FrameMetadata_<ContainerAllocator> *;
  using ConstRawPtr =
    const orbvi_4p_bridge::msg::FrameMetadata_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<orbvi_4p_bridge::msg::FrameMetadata_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<orbvi_4p_bridge::msg::FrameMetadata_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      orbvi_4p_bridge::msg::FrameMetadata_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<orbvi_4p_bridge::msg::FrameMetadata_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      orbvi_4p_bridge::msg::FrameMetadata_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<orbvi_4p_bridge::msg::FrameMetadata_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<orbvi_4p_bridge::msg::FrameMetadata_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<orbvi_4p_bridge::msg::FrameMetadata_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__orbvi_4p_bridge__msg__FrameMetadata
    std::shared_ptr<orbvi_4p_bridge::msg::FrameMetadata_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__orbvi_4p_bridge__msg__FrameMetadata
    std::shared_ptr<orbvi_4p_bridge::msg::FrameMetadata_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const FrameMetadata_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->sequence != other.sequence) {
      return false;
    }
    if (this->sensor_timestamp_ns != other.sensor_timestamp_ns) {
      return false;
    }
    return true;
  }
  bool operator!=(const FrameMetadata_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct FrameMetadata_

// alias to use template instance with default allocator
using FrameMetadata =
  orbvi_4p_bridge::msg::FrameMetadata_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace orbvi_4p_bridge

#endif  // ORBVI_4P_BRIDGE__MSG__DETAIL__FRAME_METADATA__STRUCT_HPP_
