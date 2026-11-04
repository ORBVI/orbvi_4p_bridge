
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was orbvi_sdkConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)
find_dependency(Threads)
find_dependency(PkgConfig)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
  libavformat
  libavcodec
  libavutil
  libswscale)
include("${CMAKE_CURRENT_LIST_DIR}/orbvi_sdkTargets.cmake")

if(TARGET orbvi_sdk::shared AND NOT TARGET orbvi_sdk::orbvi_sdk)
  add_library(orbvi_sdk::orbvi_sdk INTERFACE IMPORTED)
  set_property(TARGET orbvi_sdk::orbvi_sdk PROPERTY
    INTERFACE_LINK_LIBRARIES orbvi_sdk::shared)
endif()

check_required_components(orbvi_sdk)
