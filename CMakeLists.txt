cmake_minimum_required (VERSION 3.22)

project (Godwit)

macro(print_all_variables)
    message(STATUS "print_all_variables------------------------------------------{")
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
    message(STATUS "print_all_variables------------------------------------------}")
endmacro()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
set (source_dir "${PROJECT_SOURCE_DIR}/src")
set (external_libs "${PROJECT_SOURCE_DIR}/libs")

set (rapidjson_lib "${external_libs}/rapidjson")
set (tuntap_dir ${source_dir}/tuntap)
set (tcp_dir ${source_dir}/tcp)
set (utils_dir ${source_dir}/gwutils)

include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${rapidjson_lib}/include 
                    ${tuntap_dir} 
                    ${tcp_dir}
                    ${utils_dir})

set (client_files ${source_dir}/client/gw_client.cpp)
set (relay_files ${source_dir}/relay/gw_relay.cpp)
set (trial_files ${source_dir}/trial.cpp)
set (tuntap_files ${tuntap_dir}/gwtuntap.cpp)
set (tcp_files ${tcp_dir}/gwtcp.cpp)
set (utils_file ${utils_dir}/gwutils.cpp)

add_executable (godwit-client ${client_files})
add_executable (godwit_relay ${relay_files})
add_executable (gw_trial ${trial_files} ${tuntap_files})
