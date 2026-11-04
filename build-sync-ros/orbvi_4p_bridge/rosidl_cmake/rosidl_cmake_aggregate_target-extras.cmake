# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target orbvi_4p_bridge::orbvi_4p_bridge
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${orbvi_4p_bridge_TARGETS}.
if(orbvi_4p_bridge_TARGETS AND NOT TARGET orbvi_4p_bridge::orbvi_4p_bridge)
  add_library(orbvi_4p_bridge::orbvi_4p_bridge INTERFACE IMPORTED)
  set_target_properties(orbvi_4p_bridge::orbvi_4p_bridge PROPERTIES
    INTERFACE_LINK_LIBRARIES "${orbvi_4p_bridge_TARGETS}")
endif()
