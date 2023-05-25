cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(EDM4hepSource)

find_package(ROOT REQUIRED COMPONENTS RIO ROOTDataFrame)
find_package(podio REQUIRED)
find_package(EDM4HEP REQUIRED)

add_library(e4hsource SHARED ${CMAKE_SOURCE_DIR}/src/EDM4hepSource.cxx)
target_include_directories(e4hsource PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(e4hsource PUBLIC ROOT::RIO
                                       ROOT::ROOTDataFrame
                                       podio::podioRootIO
                                       EDM4HEP::edm4hep
                                       EDM4HEP::edm4hepDict)

add_executable(e4hsource_test ${CMAKE_SOURCE_DIR}/test/test.cxx)
target_include_directories(e4hsource_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(e4hsource_test e4hsource)
