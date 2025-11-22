#------------------------------------------------------------------------------
#
# \brief   CMakeLists.txt file for "libphpony"
#
#------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.11)

project ( phonybl CXX )

option( HOST "Compile host tools and tests." False )

if ( NOT MCU_PLATFORM )
   set ( MCU_PLATFORM "linux" )
endif( NOT MCU_PLATFORM)

if ( "${MCU_PLATFORM}" MATCHES "linux" )
    set( CMAKE_AUTOUIC ON )
    set( CMAKE_AUTOMOC ON )
    set( CMAKE_AUTORCC ON )
    set( CMAKE_AUTOUIC_SEARCH_PATHS "ui" )

    # Check either Qt6 or Qt5
    find_package( QT NAMES Qt5 Qt6 COMPONENTS Core Widgets Gui Network SerialPort REQUIRED )
    find_package( Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui Network SerialPort )

endif( "${MCU_PLATFORM}" MATCHES "linux" )

enable_testing()

if( HOST )
   set( PLATFORM_HEADER include/phonybl/phonybl_host.hpp )
   set( PLATFORM_LIBS
      Qt${QT_VERSION_MAJOR}::Core
      Qt${QT_VERSION_MAJOR}::SerialPort
   )
elseif( USE_BIWAK )
   set( PLATFORM_HEADER include/phonybl/phonybl_biwak.hpp )
   set( PLATFORM_LIBS
      biwak
   )
else()
   message( FATAL_ERROR "Neither biwak nor host are enabled." )
endif( HOST )

add_definitions(
   -DLEPTO_LOG_USE_PRETTY_PRINT
   -DLEPTO_CONFIGURED
)

add_library(
   phonybl
      src/phonyblCore.cpp
      include/phonybl/phonybl.hpp
      ${PLATFORM_HEADER}
)

target_include_directories(
   phonybl
      PRIVATE
         include
      INTERFACE
         include
)

target_link_libraries(
   phonybl
      lepto
      ${PLATFORM_LIBS}
)

if( "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" OR HOST )
   
   add_executable(
      phony
         src/main.cpp
   )
   
   target_link_libraries(
      phony
         phonybl
         Qt${QT_VERSION_MAJOR}::Core
   )
   
   add_subdirectory( tests )
endif()


#---fin.-----------------------------------------------------------------------
