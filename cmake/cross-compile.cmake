list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/platforms-toolchain/")

# ------ CROSS-COMPILE CONFIG ------
if (EXISTS "${CMAKE_CXX_COMPILER}")
     execute_process(COMMAND ${CMAKE_CXX_COMPILER} --version
                     OUTPUT_VARIABLE EMCC_VERSION
                     ERROR_VARIABLE EMCC_VERSION)
     if (EMCC_VERSION MATCHES "Emscripten")
              message(STATUS "Ready for Wasm...")
              set(WASM TRUE)
              set(PLATFORM_OS "Web")
              set(OS_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR})
            include(wasm-toolchain)
            if(wasm-toolchain)
               return()
            endif()
            set(wasm-toolchain ON)
     endif()
endif()

#LINUX
if(LINUX AND NOT ANDROID AND NOT APPLE)
  message(STATUS "Ready for LINUX.")
  set(LINUX TRUE)
  set(PLATFORM_OS "Linux")
  set(OS_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR})
include(linux-toolchain)
set(linux-toolchain ON)
if(linux-toolchain)
    return()
endif()
endif()

#ANDROID
if(ANDROID AND NOT APPLE)
    message(STATUS "Ready for Android.")
    set(ANDROID TRUE)
    set(PLATFORM_OS "Android")
    set(OS_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR})
include(android-toolchain)
if(android-toolchain)
  return()
endif()
set(android-toolchain ON)
endif()

#APPLE SILICON
if(UNIX AND NOT ANDROID AND NOT LINUX AND APPLE)
    message(STATUS "Ready for Apple Silicon.")
    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        message(STATUS "Ready for macOS.")
        set(MACOSX TRUE)
        set(PLATFORM_OS "macOS")
        set(OS_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR})
    include(macos-toolchain)
    if(macos-toolchain)
      return()
    endif()
    set(macos-toolchain ON)
    endif()
endif()

#iOS
if(UNIX AND NOT ANDROID AND NOT LINUX AND APPLE)
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
         message(STATUS "Ready for iOS.")
         set(PLATFORM_OS "iOS")
         set(OS_ARCHITECTURES ${CMAKE_OSX_ARCHITECTURES})
    include(ios-toolchain)
    if(ios-toolchain)
        return()
    endif()
    set(ios-toolchain ON)
    endif()
endif()

#FREEBSD
if(UNIX AND NOT ANDROID AND NOT LINUX AND NOT APPLE AND NOT WASM)
    message(STATUS "Ready for Unix, BSDs...")
    set(UNIX TRUE)
    set(PLATFORM_OS "Unix")
    set(OS_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR})
  include(unix-toolchain)
  if(unix-toolchain)
      return()
  endif()
  set(unix-toolchain ON)
endif()

#Windows
if(WIN32)
    set(WINDOWS TRUE)
    message(STATUS "Ready for Windows.")
    include(windows-toolchain)
    if(windows-toolchain)
      return()
    endif()
    set(windows-toolchain ON)
endif()
