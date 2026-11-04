#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "orbvi_4p_bridge__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__orbvi_4p_bridge__msg__FrameMetadata() -> *const std::ffi::c_void;
}

#[link(name = "orbvi_4p_bridge__rosidl_generator_c")]
extern "C" {
    fn orbvi_4p_bridge__msg__FrameMetadata__init(msg: *mut FrameMetadata) -> bool;
    fn orbvi_4p_bridge__msg__FrameMetadata__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<FrameMetadata>, size: usize) -> bool;
    fn orbvi_4p_bridge__msg__FrameMetadata__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<FrameMetadata>);
    fn orbvi_4p_bridge__msg__FrameMetadata__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<FrameMetadata>, out_seq: *mut rosidl_runtime_rs::Sequence<FrameMetadata>) -> bool;
}

// Corresponds to orbvi_4p_bridge__msg__FrameMetadata
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct FrameMetadata {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub sequence: u64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub sensor_timestamp_ns: i64,

}



impl Default for FrameMetadata {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !orbvi_4p_bridge__msg__FrameMetadata__init(&mut msg as *mut _) {
        panic!("Call to orbvi_4p_bridge__msg__FrameMetadata__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for FrameMetadata {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { orbvi_4p_bridge__msg__FrameMetadata__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { orbvi_4p_bridge__msg__FrameMetadata__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { orbvi_4p_bridge__msg__FrameMetadata__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for FrameMetadata {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for FrameMetadata where Self: Sized {
  const TYPE_NAME: &'static str = "orbvi_4p_bridge/msg/FrameMetadata";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__orbvi_4p_bridge__msg__FrameMetadata() }
  }
}


#[link(name = "orbvi_4p_bridge__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__orbvi_4p_bridge__msg__ImuMetadata() -> *const std::ffi::c_void;
}

#[link(name = "orbvi_4p_bridge__rosidl_generator_c")]
extern "C" {
    fn orbvi_4p_bridge__msg__ImuMetadata__init(msg: *mut ImuMetadata) -> bool;
    fn orbvi_4p_bridge__msg__ImuMetadata__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<ImuMetadata>, size: usize) -> bool;
    fn orbvi_4p_bridge__msg__ImuMetadata__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<ImuMetadata>);
    fn orbvi_4p_bridge__msg__ImuMetadata__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<ImuMetadata>, out_seq: *mut rosidl_runtime_rs::Sequence<ImuMetadata>) -> bool;
}

// Corresponds to orbvi_4p_bridge__msg__ImuMetadata
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ImuMetadata {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub sequence: u64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub accel_timestamp_ns: i64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub gyro_timestamp_ns: i64,

}



impl Default for ImuMetadata {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !orbvi_4p_bridge__msg__ImuMetadata__init(&mut msg as *mut _) {
        panic!("Call to orbvi_4p_bridge__msg__ImuMetadata__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for ImuMetadata {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { orbvi_4p_bridge__msg__ImuMetadata__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { orbvi_4p_bridge__msg__ImuMetadata__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { orbvi_4p_bridge__msg__ImuMetadata__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for ImuMetadata {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for ImuMetadata where Self: Sized {
  const TYPE_NAME: &'static str = "orbvi_4p_bridge/msg/ImuMetadata";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__orbvi_4p_bridge__msg__ImuMetadata() }
  }
}


