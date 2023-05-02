cmake_minimum_required(VERSION 3.11)

project(EDM4hepSource)

find_package(ROOT REQUIRED COMPONENTS RIO ROOTDataFrame)
find_package(podio REQUIRED)
find_package(EDM4HEP REQUIRED)

add_executable(e4hsource_test EDM4hepSource.cxx)
target_link_libraries(e4hsource_test ROOT::RIO
                                     ROOT::ROOTDataFrame
                                     podio::podioRootIO
                                     EDM4HEP::edm4hep
                                     EDM4HEP::edm4hepDict)
