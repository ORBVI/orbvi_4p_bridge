#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to orbvi_4p_bridge__msg__FrameMetadata

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct FrameMetadata {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub sequence: u64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub sensor_timestamp_ns: i64,

}



impl Default for FrameMetadata {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::FrameMetadata::default())
  }
}

impl rosidl_runtime_rs::Message for FrameMetadata {
  type RmwMsg = super::msg::rmw::FrameMetadata;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        sequence: msg.sequence,
        sensor_timestamp_ns: msg.sensor_timestamp_ns,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
      sequence: msg.sequence,
      sensor_timestamp_ns: msg.sensor_timestamp_ns,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      sequence: msg.sequence,
      sensor_timestamp_ns: msg.sensor_timestamp_ns,
    }
  }
}


// Corresponds to orbvi_4p_bridge__msg__ImuMetadata

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct ImuMetadata {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,


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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::ImuMetadata::default())
  }
}

impl rosidl_runtime_rs::Message for ImuMetadata {
  type RmwMsg = super::msg::rmw::ImuMetadata;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        sequence: msg.sequence,
        accel_timestamp_ns: msg.accel_timestamp_ns,
        gyro_timestamp_ns: msg.gyro_timestamp_ns,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
      sequence: msg.sequence,
      accel_timestamp_ns: msg.accel_timestamp_ns,
      gyro_timestamp_ns: msg.gyro_timestamp_ns,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      sequence: msg.sequence,
      accel_timestamp_ns: msg.accel_timestamp_ns,
      gyro_timestamp_ns: msg.gyro_timestamp_ns,
    }
  }
}


