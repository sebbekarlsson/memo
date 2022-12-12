cmake_minimum_required(VERSION 3.20)

project(memo)

file(GLOB PUBLIC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
file(GLOB memo_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

message(${memo_files})

add_executable(memo_e ${memo_files})
target_compile_options(memo_e PRIVATE -g -Wall)


list(FILTER memo_files EXCLUDE REGEX "main.c")

add_library(memo SHARED ${memo_files})
add_library(memo_static STATIC ${memo_files})

target_compile_options(memo PUBLIC -fPIC)
target_compile_options(memo_static PUBLIC -fPIC)


# Debug
target_compile_options(memo PUBLIC -fPIC -g -Wall)
target_compile_options(memo_static PUBLIC -fPIC -g -Wall)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(
  memo
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_include_directories(
  memo_static
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_include_directories(
  memo_e
    PUBLIC
      $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      ${CMAKE_CURRENT_BINARY_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/include)


include(FetchContent)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)



set(LIBRARIES m pthread)

target_link_libraries(memo_e PRIVATE ${LIBRARIES})
target_link_libraries(memo PRIVATE ${LIBRARIES})
target_link_libraries(memo_static PRIVATE ${LIBRARIES})
