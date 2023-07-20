cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(EDM4hepSource)

find_package(ROOT REQUIRED COMPONENTS RIO ROOTDataFrame)
find_package(podio REQUIRED)
find_package(EDM4HEP REQUIRED)
find_package(FCCAnalyses REQUIRED)

add_library(e4hsource SHARED ${CMAKE_SOURCE_DIR}/src/EDM4hepSource.cxx)
target_include_directories(e4hsource PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(e4hsource PUBLIC ROOT::RIO
                                       ROOT::ROOTDataFrame
                                       podio::podioRootIO
                                       EDM4HEP::edm4hep
                                       EDM4HEP::edm4hepDict)

add_executable(test_source ${CMAKE_SOURCE_DIR}/test/test_source.cxx)
target_include_directories(test_source PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_source e4hsource)

add_executable(test_source_associations
               ${CMAKE_SOURCE_DIR}/test/test_source_associations.cxx)
target_include_directories(test_source_associations PRIVATE
                           ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_source_associations e4hsource
                                               FCCAnalyses::FCCAnalyses
)

add_executable(test_low_level ${CMAKE_SOURCE_DIR}/test/test_low_level.cxx)
target_link_libraries(test_low_level ROOT::ROOTDataFrame
                                     podio::podioRootIO
                                     EDM4HEP::edm4hep
                                     EDM4HEP::edm4hepDict
)

add_executable(test_low_level_associations
               ${CMAKE_SOURCE_DIR}/test/test_low_level_associations.cxx)
target_link_libraries(test_low_level_associations ROOT::ROOTDataFrame
                                                  podio::podioRootIO
                                                  EDM4HEP::edm4hep
                                                  EDM4HEP::edm4hepDict
                                                  FCCAnalyses::FCCAnalyses
)
