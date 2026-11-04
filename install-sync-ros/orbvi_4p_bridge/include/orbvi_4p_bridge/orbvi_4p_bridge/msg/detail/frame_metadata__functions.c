// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from orbvi_4p_bridge:msg/FrameMetadata.idl
// generated code does not contain a copyright notice
#include "orbvi_4p_bridge/msg/detail/frame_metadata__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"

bool
orbvi_4p_bridge__msg__FrameMetadata__init(orbvi_4p_bridge__msg__FrameMetadata * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    orbvi_4p_bridge__msg__FrameMetadata__fini(msg);
    return false;
  }
  // sequence
  // sensor_timestamp_ns
  return true;
}

void
orbvi_4p_bridge__msg__FrameMetadata__fini(orbvi_4p_bridge__msg__FrameMetadata * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // sequence
  // sensor_timestamp_ns
}

bool
orbvi_4p_bridge__msg__FrameMetadata__are_equal(const orbvi_4p_bridge__msg__FrameMetadata * lhs, const orbvi_4p_bridge__msg__FrameMetadata * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // sequence
  if (lhs->sequence != rhs->sequence) {
    return false;
  }
  // sensor_timestamp_ns
  if (lhs->sensor_timestamp_ns != rhs->sensor_timestamp_ns) {
    return false;
  }
  return true;
}

bool
orbvi_4p_bridge__msg__FrameMetadata__copy(
  const orbvi_4p_bridge__msg__FrameMetadata * input,
  orbvi_4p_bridge__msg__FrameMetadata * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // sequence
  output->sequence = input->sequence;
  // sensor_timestamp_ns
  output->sensor_timestamp_ns = input->sensor_timestamp_ns;
  return true;
}

orbvi_4p_bridge__msg__FrameMetadata *
orbvi_4p_bridge__msg__FrameMetadata__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  orbvi_4p_bridge__msg__FrameMetadata * msg = (orbvi_4p_bridge__msg__FrameMetadata *)allocator.allocate(sizeof(orbvi_4p_bridge__msg__FrameMetadata), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(orbvi_4p_bridge__msg__FrameMetadata));
  bool success = orbvi_4p_bridge__msg__FrameMetadata__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
orbvi_4p_bridge__msg__FrameMetadata__destroy(orbvi_4p_bridge__msg__FrameMetadata * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    orbvi_4p_bridge__msg__FrameMetadata__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
orbvi_4p_bridge__msg__FrameMetadata__Sequence__init(orbvi_4p_bridge__msg__FrameMetadata__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  orbvi_4p_bridge__msg__FrameMetadata * data = NULL;

  if (size) {
    data = (orbvi_4p_bridge__msg__FrameMetadata *)allocator.zero_allocate(size, sizeof(orbvi_4p_bridge__msg__FrameMetadata), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = orbvi_4p_bridge__msg__FrameMetadata__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        orbvi_4p_bridge__msg__FrameMetadata__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
orbvi_4p_bridge__msg__FrameMetadata__Sequence__fini(orbvi_4p_bridge__msg__FrameMetadata__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      orbvi_4p_bridge__msg__FrameMetadata__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

orbvi_4p_bridge__msg__FrameMetadata__Sequence *
orbvi_4p_bridge__msg__FrameMetadata__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  orbvi_4p_bridge__msg__FrameMetadata__Sequence * array = (orbvi_4p_bridge__msg__FrameMetadata__Sequence *)allocator.allocate(sizeof(orbvi_4p_bridge__msg__FrameMetadata__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = orbvi_4p_bridge__msg__FrameMetadata__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
orbvi_4p_bridge__msg__FrameMetadata__Sequence__destroy(orbvi_4p_bridge__msg__FrameMetadata__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    orbvi_4p_bridge__msg__FrameMetadata__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
orbvi_4p_bridge__msg__FrameMetadata__Sequence__are_equal(const orbvi_4p_bridge__msg__FrameMetadata__Sequence * lhs, const orbvi_4p_bridge__msg__FrameMetadata__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!orbvi_4p_bridge__msg__FrameMetadata__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
orbvi_4p_bridge__msg__FrameMetadata__Sequence__copy(
  const orbvi_4p_bridge__msg__FrameMetadata__Sequence * input,
  orbvi_4p_bridge__msg__FrameMetadata__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(orbvi_4p_bridge__msg__FrameMetadata);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    orbvi_4p_bridge__msg__FrameMetadata * data =
      (orbvi_4p_bridge__msg__FrameMetadata *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!orbvi_4p_bridge__msg__FrameMetadata__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          orbvi_4p_bridge__msg__FrameMetadata__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!orbvi_4p_bridge__msg__FrameMetadata__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
