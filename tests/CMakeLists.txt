#------------------------------------------------------------------------------
#
# \brief	CMakeLists.txt file for "libbiwak"
#
#------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.11)
project( phonybl_tests )

# Catch2 v3?
find_package(Catch2 3 QUIET)

if (Catch2_FOUND)
    message(STATUS "Using Catch2 v3")
    add_definitions( "-DCATCH_V3" )
    set(CATCH2_TARGET Catch2::Catch2WithMain)
else()
    # Catch2 v2?
    find_package(Catch2 2 REQUIRED)
    message(STATUS "Using Catch2 v2")
    add_definitions( "-DCATCH_V2" )
    set(CATCH2_TARGET Catch2::Catch2)
endif()

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
