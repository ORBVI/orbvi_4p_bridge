# Install script for directory: /home/k8plus/src/orbvi-4p/orbvi_4p_bridge

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/install-sync-ros/orbvi_4p_bridge")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/rosidl_interfaces" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_index/share/ament_index/resource_index/rosidl_interfaces/orbvi_4p_bridge")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/orbvi_4p_bridge/orbvi_4p_bridge" TYPE DIRECTORY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_generator_c/orbvi_4p_bridge/" REGEX "/[^/]*\\.h$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/environment" TYPE FILE FILES "/opt/ros/humble/lib/python3.10/site-packages/ament_package/template/environment_hook/library_path.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/environment" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_environment_hooks/library_path.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_c.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/liborbvi_4p_bridge__rosidl_generator_c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_c.so"
         OLD_RPATH "/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_c.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/orbvi_4p_bridge/orbvi_4p_bridge" TYPE DIRECTORY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_typesupport_fastrtps_c/orbvi_4p_bridge/" REGEX "/[^/]*\\.cpp$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_c.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_c.so"
         OLD_RPATH "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_c.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/orbvi_4p_bridge/orbvi_4p_bridge" TYPE DIRECTORY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_generator_cpp/orbvi_4p_bridge/" REGEX "/[^/]*\\.hpp$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/orbvi_4p_bridge/orbvi_4p_bridge" TYPE DIRECTORY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_typesupport_fastrtps_cpp/orbvi_4p_bridge/" REGEX "/[^/]*\\.cpp$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_cpp.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_cpp.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_cpp.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_cpp.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_cpp.so"
         OLD_RPATH "/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_fastrtps_cpp.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/orbvi_4p_bridge/orbvi_4p_bridge" TYPE DIRECTORY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_typesupport_introspection_c/orbvi_4p_bridge/" REGEX "/[^/]*\\.h$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_c.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/liborbvi_4p_bridge__rosidl_typesupport_introspection_c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_c.so"
         OLD_RPATH "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_c.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_c.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_c.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/liborbvi_4p_bridge__rosidl_typesupport_c.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_c.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_c.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_c.so"
         OLD_RPATH "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_c.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/orbvi_4p_bridge/orbvi_4p_bridge" TYPE DIRECTORY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_typesupport_introspection_cpp/orbvi_4p_bridge/" REGEX "/[^/]*\\.hpp$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_cpp.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_cpp.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/liborbvi_4p_bridge__rosidl_typesupport_introspection_cpp.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_cpp.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_cpp.so"
         OLD_RPATH "/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_introspection_cpp.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_cpp.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_cpp.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/liborbvi_4p_bridge__rosidl_typesupport_cpp.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_cpp.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_cpp.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_cpp.so"
         OLD_RPATH "/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_typesupport_cpp.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/environment" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_environment_hooks/pythonpath.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/environment" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_environment_hooks/pythonpath.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge-0.1.0-py3.10.egg-info" TYPE DIRECTORY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_python/orbvi_4p_bridge/orbvi_4p_bridge.egg-info/")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge" TYPE DIRECTORY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_generator_py/orbvi_4p_bridge/" REGEX "/[^/]*\\.pyc$" EXCLUDE REGEX "/\\_\\_pycache\\_\\_$" EXCLUDE)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(
        COMMAND
        "/usr/bin/python3" "-m" "compileall"
        "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/install-sync-ros/orbvi_4p_bridge/local/lib/python3.10/dist-packages/orbvi_4p_bridge"
      )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/orbvi_4p_bridge__py/cmake_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_fastrtps_c.cpython-310-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_fastrtps_c.cpython-310-x86_64-linux-gnu.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_fastrtps_c.cpython-310-x86_64-linux-gnu.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge" TYPE SHARED_LIBRARY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_generator_py/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_fastrtps_c.cpython-310-x86_64-linux-gnu.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_fastrtps_c.cpython-310-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_fastrtps_c.cpython-310-x86_64-linux-gnu.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_fastrtps_c.cpython-310-x86_64-linux-gnu.so"
         OLD_RPATH "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_generator_py/orbvi_4p_bridge:/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_fastrtps_c.cpython-310-x86_64-linux-gnu.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_introspection_c.cpython-310-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_introspection_c.cpython-310-x86_64-linux-gnu.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_introspection_c.cpython-310-x86_64-linux-gnu.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge" TYPE SHARED_LIBRARY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_generator_py/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_introspection_c.cpython-310-x86_64-linux-gnu.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_introspection_c.cpython-310-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_introspection_c.cpython-310-x86_64-linux-gnu.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_introspection_c.cpython-310-x86_64-linux-gnu.so"
         OLD_RPATH "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_generator_py/orbvi_4p_bridge:/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_introspection_c.cpython-310-x86_64-linux-gnu.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_c.cpython-310-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_c.cpython-310-x86_64-linux-gnu.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_c.cpython-310-x86_64-linux-gnu.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge" TYPE SHARED_LIBRARY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_generator_py/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_c.cpython-310-x86_64-linux-gnu.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_c.cpython-310-x86_64-linux-gnu.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_c.cpython-310-x86_64-linux-gnu.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_c.cpython-310-x86_64-linux-gnu.so"
         OLD_RPATH "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_generator_py/orbvi_4p_bridge:/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/local/lib/python3.10/dist-packages/orbvi_4p_bridge/orbvi_4p_bridge_s__rosidl_typesupport_c.cpython-310-x86_64-linux-gnu.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_py.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_py.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_py.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_generator_py/orbvi_4p_bridge/liborbvi_4p_bridge__rosidl_generator_py.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_py.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_py.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_py.so"
         OLD_RPATH "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/liborbvi_4p_bridge__rosidl_generator_py.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/orbvi_4p_bridge__rs/cmake_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge" TYPE DIRECTORY FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_generator_rs/orbvi_4p_bridge/rust")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/msg" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_adapter/orbvi_4p_bridge/msg/FrameMetadata.idl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/msg" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_adapter/orbvi_4p_bridge/msg/ImuMetadata.idl")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/msg" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/msg/FrameMetadata.msg")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/msg" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/msg/ImuMetadata.msg")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/orbvi-4p-sdk/cmake_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_bridge_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_bridge_node")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_bridge_node"
         RPATH "\$ORIGIN/..")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge" TYPE EXECUTABLE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/orbvi_bridge_node")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_bridge_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_bridge_node")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_bridge_node"
         OLD_RPATH "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/orbvi-4p-sdk:/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge:/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN/..")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_bridge_node")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_capture_check" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_capture_check")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_capture_check"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge" TYPE EXECUTABLE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/orbvi_capture_check")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_capture_check" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_capture_check")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_capture_check"
         OLD_RPATH "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge:/opt/ros/humble/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_capture_check")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_image_stream_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_image_stream_node")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_image_stream_node"
         RPATH "\$ORIGIN/..")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge" TYPE EXECUTABLE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/orbvi_image_stream_node")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_image_stream_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_image_stream_node")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_image_stream_node"
         OLD_RPATH "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/orbvi-4p-sdk:/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN/..")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_image_stream_node")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_temperature_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_temperature_node")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_temperature_node"
         RPATH "\$ORIGIN/..")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge" TYPE EXECUTABLE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/orbvi_temperature_node")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_temperature_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_temperature_node")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_temperature_node"
         OLD_RPATH "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/orbvi-4p-sdk:/opt/ros/humble/lib:"
         NEW_RPATH "\$ORIGIN/..")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/orbvi_4p_bridge/orbvi_temperature_node")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge" TYPE DIRECTORY FILES
    "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/launch"
    "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/config"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/package_run_dependencies" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_index/share/ament_index/resource_index/package_run_dependencies/orbvi_4p_bridge")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/parent_prefix_path" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_index/share/ament_index/resource_index/parent_prefix_path/orbvi_4p_bridge")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/environment" TYPE FILE FILES "/opt/ros/humble/share/ament_cmake_core/cmake/environment_hooks/environment/ament_prefix_path.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/environment" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_environment_hooks/ament_prefix_path.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/environment" TYPE FILE FILES "/opt/ros/humble/share/ament_cmake_core/cmake/environment_hooks/environment/path.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/environment" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_environment_hooks/path.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_environment_hooks/local_setup.bash")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_environment_hooks/local_setup.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_environment_hooks/local_setup.zsh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_environment_hooks/local_setup.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_environment_hooks/package.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/packages" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_index/share/ament_index/resource_index/packages/orbvi_4p_bridge")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cExport.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cExport.cmake"
         "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cExport.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cExport-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cExport.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cExport.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cExport-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cExport.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cExport.cmake"
         "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cExport.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cExport-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cExport.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cExport.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cExport-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cppExport.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cppExport.cmake"
         "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cppExport.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cppExport-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cppExport.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_cppExport.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cppExport.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cppExport.cmake"
         "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cppExport.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cppExport-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cppExport.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cppExport.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_typesupport_fastrtps_cppExport-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cExport.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cExport.cmake"
         "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cExport.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cExport-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cExport.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cExport.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cExport-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cExport.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cExport.cmake"
         "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cExport.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cExport-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cExport.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cExport.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cExport-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cppExport.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cppExport.cmake"
         "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cppExport.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cppExport-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cppExport.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cppExport.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_introspection_cppExport-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cppExport.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cppExport.cmake"
         "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cppExport.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cppExport-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cppExport.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cppExport.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/orbvi_4p_bridge__rosidl_typesupport_cppExport-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_pyExport.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_pyExport.cmake"
         "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_pyExport.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_pyExport-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_pyExport.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_pyExport.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/CMakeFiles/Export/share/orbvi_4p_bridge/cmake/export_orbvi_4p_bridge__rosidl_generator_pyExport-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_cmake/rosidl_cmake-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_export_dependencies/ament_cmake_export_dependencies-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_export_include_directories/ament_cmake_export_include_directories-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_export_libraries/ament_cmake_export_libraries-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_export_targets/ament_cmake_export_targets-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_cmake/rosidl_cmake_export_typesupport_targets-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_cmake/rosidl_cmake_export_typesupport_libraries-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/rosidl_cmake/rosidl_cmake_aggregate_target-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge/cmake" TYPE FILE FILES
    "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_core/orbvi_4p_bridgeConfig.cmake"
    "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/ament_cmake_core/orbvi_4p_bridgeConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/orbvi_4p_bridge" TYPE FILE FILES "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/package.xml")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/k8plus/src/orbvi-4p/orbvi_4p_bridge/build-sync-ros/orbvi_4p_bridge/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
