#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
#  if defined(ORBVI_SDK_BUILDING_LIBRARY)
#    define ORBVI_SDK_API __declspec(dllexport)
#  else
#    define ORBVI_SDK_API __declspec(dllimport)
#  endif
#else
#  if defined(__GNUC__) || defined(__clang__)
#    define ORBVI_SDK_API __attribute__((visibility("default")))
#  else
#    define ORBVI_SDK_API
#  endif
#endif

#if defined(__GNUC__) || defined(__clang__)
#  define ORBVI_SDK_DEPRECATED(message) __attribute__((deprecated(message)))
#else
#  define ORBVI_SDK_DEPRECATED(message)
#endif