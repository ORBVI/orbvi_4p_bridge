cmake_minimum_required(VERSION 3.16)

project(orbvi_4p_bridge)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ROS build tools require their Python dependencies (notably catkin_pkg).
# Prefer the system interpreter when the interactive PATH points at an
# application virtualenv that does not have those packages.
if(EXISTS "/usr/bin/python3")
  set(PYTHON_EXECUTABLE "/usr/bin/python3" CACHE FILEPATH
    "Python interpreter used by ROS build tools" FORCE)
  set(Python3_EXECUTABLE "/usr/bin/python3" CACHE FILEPATH
    "Python interpreter used by ROS build tools" FORCE)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(yaml-cpp REQUIRED)

# The SDK is intentionally injected either through an installed CMake package
# or through an explicit source tree. The latter is useful while the SDK is
# still being stabilized and the install prefix may be stale.
set(ORBVI_SDK_SOURCE_DIR "" CACHE PATH
  "Explicit path to the orbvi-4p-sdk source tree. If set, the SDK is built as a subdirectory.")

if(ORBVI_SDK_SOURCE_DIR)
  if(NOT EXISTS "${ORBVI_SDK_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
      "ORBVI_SDK_SOURCE_DIR does not contain CMakeLists.txt: ${ORBVI_SDK_SOURCE_DIR}")
  endif()

  # Keep the bridge build small and free of SDK host tests/examples and any
  # legacy hardware backend selection. The Fake runtime is selected only by
  # the node's explicit runtime_mode parameter.
  set(ORBVI_BUILD_TESTS OFF CACHE BOOL "Build SDK host tests" FORCE)
  set(ORBVI_BUILD_EXAMPLES OFF CACHE BOOL "Build SDK examples" FORCE)
  set(ORBVI_BUILD_STATIC OFF CACHE BOOL "Build SDK static library" FORCE)

  add_subdirectory("${ORBVI_SDK_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/orbvi-4p-sdk")
  if(NOT TARGET orbvi_sdk::shared)
    message(FATAL_ERROR "The SDK source tree did not provide the orbvi_sdk::shared target")
  endif()
  set(ORBVI_SDK_TARGET orbvi_sdk::shared)
else()
  find_package(orbvi_sdk CONFIG REQUIRED)
  if(TARGET orbvi_sdk::shared)
    set(ORBVI_SDK_TARGET orbvi_sdk::shared)
  elseif(TARGET orbvi_sdk::orbvi_sdk)
    set(ORBVI_SDK_TARGET orbvi_sdk::orbvi_sdk)
  else()
    message(FATAL_ERROR
      "orbvi_sdk was found but neither orbvi_sdk::shared nor orbvi_sdk::orbvi_sdk is available")
  endif()
endif()

add_executable(orbvi_bridge_node src/bridge_node.cpp)
target_link_libraries(orbvi_bridge_node ${ORBVI_SDK_TARGET})
ament_target_dependencies(orbvi_bridge_node rclcpp sensor_msgs)

add_executable(orbvi_image_stream_node src/image_stream_node.cpp)
target_link_libraries(orbvi_image_stream_node ${ORBVI_SDK_TARGET} yaml-cpp)
ament_target_dependencies(orbvi_image_stream_node rclcpp sensor_msgs)

add_executable(orbvi_temperature_node src/temperature_node.cpp)
target_link_libraries(orbvi_temperature_node ${ORBVI_SDK_TARGET})
ament_target_dependencies(orbvi_temperature_node rclcpp sensor_msgs)

# In the source-tree SDK build, both the SDK shared library and Bridge nodes
# install into the same prefix. Let installed nodes find the SDK without a
# manually configured LD_LIBRARY_PATH (also needed by launch_testing).
if(ORBVI_SDK_SOURCE_DIR)
  set_target_properties(orbvi_bridge_node orbvi_image_stream_node
    orbvi_temperature_node PROPERTIES INSTALL_RPATH "$ORIGIN/..")
endif()

install(TARGETS
  orbvi_bridge_node
  orbvi_image_stream_node
  orbvi_temperature_node
  DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(launch_testing_ament_cmake REQUIRED)
  find_package(Python3 REQUIRED COMPONENTS Interpreter)
  add_launch_test(test/test_fake_bridge_launch.py TIMEOUT 30)
  add_launch_test(test/test_imu_bridge_launch.py TIMEOUT 30)

  add_executable(test_frame_image test/test_frame_image.cpp)
  target_include_directories(test_frame_image PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
  target_link_libraries(test_frame_image ${ORBVI_SDK_TARGET})
  ament_target_dependencies(test_frame_image sensor_msgs)
  add_test(NAME frame_image COMMAND test_frame_image)

  add_test(NAME image_fail_closed_runtime
    COMMAND $<TARGET_FILE:orbvi_image_stream_node> --ros-args
      -p runtime_mode:=unsupported)
  add_test(NAME image_rejects_invalid_mask
    COMMAND $<TARGET_FILE:orbvi_image_stream_node> --ros-args
      -p runtime_mode:=jpeg -p host:=127.0.0.1 -p camera_mask:=16)
  add_test(NAME image_rejects_invalid_drain
    COMMAND $<TARGET_FILE:orbvi_image_stream_node> --ros-args
      -p runtime_mode:=nv12 -p host:=127.0.0.1
      -p drain_min_sec:=4.0 -p drain_timeout_sec:=2.0)
  add_test(NAME image_rejects_invalid_queue_depth
    COMMAND $<TARGET_FILE:orbvi_image_stream_node> --ros-args
      -p runtime_mode:=nv12 -p host:=127.0.0.1
      -p publish_queue_depth:=0)
  add_test(NAME temperature_rejects_zero_port
    COMMAND $<TARGET_FILE:orbvi_temperature_node> --ros-args
      -p port:=0)
  add_test(NAME temperature_rejects_bad_bind
    COMMAND $<TARGET_FILE:orbvi_temperature_node> --ros-args
      -p bind_address:=not-an-ip -p samples:=1)
  set_tests_properties(
    image_fail_closed_runtime image_rejects_invalid_mask
    image_rejects_invalid_drain image_rejects_invalid_queue_depth
    temperature_rejects_zero_port temperature_rejects_bad_bind
    PROPERTIES WILL_FAIL TRUE TIMEOUT 10)

  add_test(NAME image_stream_jpeg_one_shot
    COMMAND ${Python3_EXECUTABLE}
      ${CMAKE_CURRENT_SOURCE_DIR}/test/test_image_stream_node.py
      $<TARGET_FILE:orbvi_image_stream_node>)
  add_test(NAME image_stream_jpeg_four
    COMMAND ${Python3_EXECUTABLE}
      ${CMAKE_CURRENT_SOURCE_DIR}/test/test_image_jpeg_four.py
      $<TARGET_FILE:orbvi_image_stream_node>)
  add_test(NAME image_stream_nv12_four
    COMMAND ${Python3_EXECUTABLE}
      ${CMAKE_CURRENT_SOURCE_DIR}/test/test_image_nv12_four.py
      $<TARGET_FILE:orbvi_image_stream_node>)
  add_test(NAME image_stream_nv12_drain
    COMMAND ${Python3_EXECUTABLE}
      ${CMAKE_CURRENT_SOURCE_DIR}/test/test_image_nv12_drain.py
      $<TARGET_FILE:orbvi_image_stream_node>)
  add_test(NAME temperature_ocpu_end_to_end
    COMMAND ${Python3_EXECUTABLE}
      ${CMAKE_CURRENT_SOURCE_DIR}/test/test_temperature_node.py
      $<TARGET_FILE:orbvi_temperature_node>)
  set_tests_properties(
    image_stream_jpeg_one_shot image_stream_jpeg_four
    image_stream_nv12_four temperature_ocpu_end_to_end
    PROPERTIES TIMEOUT 45 RUN_SERIAL TRUE ENVIRONMENT "ROS_DOMAIN_ID=97")
  set_tests_properties(image_stream_nv12_drain PROPERTIES
    TIMEOUT 65 RUN_SERIAL TRUE ENVIRONMENT "ROS_DOMAIN_ID=97")

  add_test(NAME fail_closed_runtime
    COMMAND $<TARGET_FILE:orbvi_bridge_node> --ros-args -p runtime_mode:=unsupported)
  add_test(NAME rtsp_requires_url
    COMMAND $<TARGET_FILE:orbvi_bridge_node> --ros-args -p runtime_mode:=rtsp -p camera_count:=1)
  set_tests_properties(rtsp_requires_url PROPERTIES
    WILL_FAIL TRUE
    TIMEOUT 10)

  add_test(NAME rtsp_four_requires_all_urls
    COMMAND $<TARGET_FILE:orbvi_bridge_node> --ros-args
      -p runtime_mode:=rtsp -p camera_count:=4
      -p rtsp_url:=rtsp://127.0.0.1:554/live/PRR)
  add_test(NAME rtsp_rejects_extra_url
    COMMAND $<TARGET_FILE:orbvi_bridge_node> --ros-args
      -p runtime_mode:=rtsp -p camera_count:=1
      -p rtsp_url:=rtsp://127.0.0.1:554/live/PRR
      -p rtsp_url_1:=rtsp://127.0.0.1:555/live/PRR)
  add_test(NAME rtsp_rejects_invalid_image_format
    COMMAND $<TARGET_FILE:orbvi_bridge_node> --ros-args
      -p runtime_mode:=rtsp -p camera_count:=1
      -p rtsp_url:=rtsp://127.0.0.1:554/live/PRR -p image_format:=rgb8)
  set_tests_properties(rtsp_four_requires_all_urls rtsp_rejects_extra_url
    rtsp_rejects_invalid_image_format PROPERTIES
    WILL_FAIL TRUE TIMEOUT 10)

  add_test(NAME imu_requires_opt_in
    COMMAND $<TARGET_FILE:orbvi_bridge_node> --ros-args -p runtime_mode:=udp_imu)
  add_test(NAME imu_rejects_zero_port
    COMMAND $<TARGET_FILE:orbvi_bridge_node> --ros-args -p runtime_mode:=udp_imu
      -p imu_enabled:=true -p imu_port:=0)
  add_test(NAME fake_rejects_real_imu
    COMMAND $<TARGET_FILE:orbvi_bridge_node> --ros-args -p runtime_mode:=fake
      -p imu_enabled:=true)
  set_tests_properties(imu_requires_opt_in imu_rejects_zero_port
    fake_rejects_real_imu PROPERTIES WILL_FAIL TRUE TIMEOUT 10)

  set_tests_properties(fail_closed_runtime PROPERTIES
    WILL_FAIL TRUE
    TIMEOUT 10)
endif()

ament_export_dependencies(rclcpp sensor_msgs)
ament_package()
