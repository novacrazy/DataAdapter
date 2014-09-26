cmake_minimum_required (VERSION 2.6)
project (DataAdapter)
set(CMAKE_BUILD_TYPE Release)

set(DATA_ADAPTER_VERSION_MAJOR 0)
set(DATA_ADAPTER_VERSION_MINOR 2)
set(DATA_ADAPTER_VERSION ${DATA_ADAPTER_VERSION_MAJOR}.$(DATA_ADAPTER_VERSION_MINOR))

set(CMAKE_CXX_FLAGS "-g -Wall")

enable_testing()
find_package(GTest REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests/include)
include_directories(${GTEST_INCLUDE_DIRS})


set(SRC_LIST
	include/adapters/array.hpp
	include/data_adapter.hpp
	include/data_adapter_all.hpp
	tests/include/array/fixtures.hpp
	tests/include/array/tests.hpp
	tests/include/tests.h
	tests/include/tools.hpp
	tests/src/test_main.cpp
	)

# Since DataAdapter is header only, this builds the test suites
add_executable(DataAdapter ${CMAKE_CURRENT_SOURCE_DIR}/tests/src/test_main.cpp)
target_link_libraries(DataAdapter ${GTEST_LIBRARIES})

add_test(DataAdapter_Tests DataAdapter)