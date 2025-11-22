#------------------------------------------------------------------------------
#
# \brief	CMakeLists.txt file for "libbiwak"
#
#------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.11)
project( phonybl_tests )

find_package( Catch2 3 )
if( NOT Catch2_FOUND )
   find_package( Catch )
   if( NOT Catch_FOUND )
      # find_package does not work for Catch, at least on ubuntu 20.04
      if( EXISTS /usr/include/catch )
         add_definitions( "-DCATCH_V1" )
      elseif( EXISTS /usr/include/catch2 )
         if( EXISTS /usr/include/catch2/catch_test_macros.hpp )
            add_definitions( "-DCATCH_V3" )
         else()
            add_definitions( "-DCATCH_V2" )
         endif()
      else( EXISTS /usr/include/catch2 )
         message( FATAL_ERROR "Install eeither Catch or Catch2")
      endif( EXISTS /usr/include/catch )
   endif( NOT Catch_FOUND )
   add_definitions( "-DCATCH_V1" )
else( NOT Catch2_FOUND)
   add_definitions( "-DCATCH_V3" )
   set( CATCH_LIBRARY "Catch2::Catch2WithMain" )
endif( NOT Catch2_FOUND )

set(
   headers

)

set(
   sources
      test_phonybl.cpp
      test_main.cpp
)

add_executable(
   ${PROJECT_NAME}
      ${sources}
      ${headers}
)


target_link_libraries(
   ${PROJECT_NAME}
   PRIVATE
      lepto
      phonybl
      ${CATCH_LIBRARY}
)


add_test(
   NAME ${PROJECT_NAME}
   COMMAND ${PROJECT_NAME}
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


#------------------------------------------------------------------------------
