#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "orbvi_sdk::shared" for configuration ""
set_property(TARGET orbvi_sdk::shared APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(orbvi_sdk::shared PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/liborbvi_sdk.so.0.1.0"
  IMPORTED_SONAME_NOCONFIG "liborbvi_sdk.so.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS orbvi_sdk::shared )
list(APPEND _IMPORT_CHECK_FILES_FOR_orbvi_sdk::shared "${_IMPORT_PREFIX}/lib/liborbvi_sdk.so.0.1.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
