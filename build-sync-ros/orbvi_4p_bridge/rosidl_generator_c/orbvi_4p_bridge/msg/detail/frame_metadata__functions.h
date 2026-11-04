// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from orbvi_4p_bridge:msg/FrameMetadata.idl
// generated code does not contain a copyright notice

#ifndef ORBVI_4P_BRIDGE__MSG__DETAIL__FRAME_METADATA__FUNCTIONS_H_
#define ORBVI_4P_BRIDGE__MSG__DETAIL__FRAME_METADATA__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "orbvi_4p_bridge/msg/rosidl_generator_c__visibility_control.h"

#include "orbvi_4p_bridge/msg/detail/frame_metadata__struct.h"

/// Initialize msg/FrameMetadata message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * orbvi_4p_bridge__msg__FrameMetadata
 * )) before or use
 * orbvi_4p_bridge__msg__FrameMetadata__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_orbvi_4p_bridge
bool
orbvi_4p_bridge__msg__FrameMetadata__init(orbvi_4p_bridge__msg__FrameMetadata * msg);

/// Finalize msg/FrameMetadata message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_orbvi_4p_bridge
void
orbvi_4p_bridge__msg__FrameMetadata__fini(orbvi_4p_bridge__msg__FrameMetadata * msg);

/// Create msg/FrameMetadata message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * orbvi_4p_bridge__msg__FrameMetadata__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_orbvi_4p_bridge
orbvi_4p_bridge__msg__FrameMetadata *
orbvi_4p_bridge__msg__FrameMetadata__create();

/// Destroy msg/FrameMetadata message.
/**
 * It calls
 * orbvi_4p_bridge__msg__FrameMetadata__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_orbvi_4p_bridge
void
orbvi_4p_bridge__msg__FrameMetadata__destroy(orbvi_4p_bridge__msg__FrameMetadata * msg);

/// Check for msg/FrameMetadata message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_orbvi_4p_bridge
bool
orbvi_4p_bridge__msg__FrameMetadata__are_equal(const orbvi_4p_bridge__msg__FrameMetadata * lhs, const orbvi_4p_bridge__msg__FrameMetadata * rhs);

/// Copy a msg/FrameMetadata message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_orbvi_4p_bridge
bool
orbvi_4p_bridge__msg__FrameMetadata__copy(
  const orbvi_4p_bridge__msg__FrameMetadata * input,
  orbvi_4p_bridge__msg__FrameMetadata * output);

/// Initialize array of msg/FrameMetadata messages.
/**
 * It allocates the memory for the number of elements and calls
 * orbvi_4p_bridge__msg__FrameMetadata__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_orbvi_4p_bridge
bool
orbvi_4p_bridge__msg__FrameMetadata__Sequence__init(orbvi_4p_bridge__msg__FrameMetadata__Sequence * array, size_t size);

/// Finalize array of msg/FrameMetadata messages.
/**
 * It calls
 * orbvi_4p_bridge__msg__FrameMetadata__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_orbvi_4p_bridge
void
orbvi_4p_bridge__msg__FrameMetadata__Sequence__fini(orbvi_4p_bridge__msg__FrameMetadata__Sequence * array);

/// Create array of msg/FrameMetadata messages.
/**
 * It allocates the memory for the array and calls
 * orbvi_4p_bridge__msg__FrameMetadata__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_orbvi_4p_bridge
orbvi_4p_bridge__msg__FrameMetadata__Sequence *
orbvi_4p_bridge__msg__FrameMetadata__Sequence__create(size_t size);

/// Destroy array of msg/FrameMetadata messages.
/**
 * It calls
 * orbvi_4p_bridge__msg__FrameMetadata__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_orbvi_4p_bridge
void
orbvi_4p_bridge__msg__FrameMetadata__Sequence__destroy(orbvi_4p_bridge__msg__FrameMetadata__Sequence * array);

/// Check for msg/FrameMetadata message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_orbvi_4p_bridge
bool
orbvi_4p_bridge__msg__FrameMetadata__Sequence__are_equal(const orbvi_4p_bridge__msg__FrameMetadata__Sequence * lhs, const orbvi_4p_bridge__msg__FrameMetadata__Sequence * rhs);

/// Copy an array of msg/FrameMetadata messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_orbvi_4p_bridge
bool
orbvi_4p_bridge__msg__FrameMetadata__Sequence__copy(
  const orbvi_4p_bridge__msg__FrameMetadata__Sequence * input,
  orbvi_4p_bridge__msg__FrameMetadata__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // ORBVI_4P_BRIDGE__MSG__DETAIL__FRAME_METADATA__FUNCTIONS_H_
