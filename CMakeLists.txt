cmake_minimum_required(VERSION 3.13)
SET(CMAKE_C_COMPILER "/usr/bin/gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")
project(DataCollect)
option(USE_DEBUG "use debug" ON)

find_package(Threads)
find_library(MicroDDS_Agent microxrcedds_agent PATHS ~/hux/xiaomi/6.4/demo/micro-xrce-dds-agent/build)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(/usr/local/include)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(SRCFILES ${CMAKE_CURRENT_SOURCE_DIR}/src/InfoStudent.c ${CMAKE_CURRENT_SOURCE_DIR}/src/Iclient.cpp)

if(USE_DEBUG)
    message(STATUS "Use debug")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize-recover=address -fno-stack-protector -fno-omit-frame-pointer")
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    set(CMAKE_BUILD_TYPE Debug)

endif(USE_DEBUG)

#add_executable(server      ${CMAKE_CURRENT_SOURCE_DIR}/src/dataCenter/server.cpp ${SRCFILES})
add_executable(clientData1 ${CMAKE_CURRENT_SOURCE_DIR}/src/clients/clientData1.cpp ${SRCFILES})
add_executable(clientData2 ${CMAKE_CURRENT_SOURCE_DIR}/src/clients/clientData2.cpp ${SRCFILES})
add_executable(clientData3 ${CMAKE_CURRENT_SOURCE_DIR}/src/clients/clientData3.cpp ${SRCFILES})
#add_executable(agent       ${CMAKE_CURRENT_SOURCE_DIR}/include/InfoStudent.c ${CMAKE_CURRENT_SOURCE_DIR}/src/dataCenter/agent.cpp)
add_executable(center      ${CMAKE_CURRENT_SOURCE_DIR}/src/dataCenter/center.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/dataCenter/agent.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/dataCenter/server.cpp ${SRCFILES})
#[[
install(
        TARGETS
            agent
        EXPORT
            MicroXRCEAgentTargets
        RUNTIME
            DESTINATION ${BIN_INSTALL_DIR}
        LIBRARY
            DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE
            DESTINATION ${LIB_INSTALL_DIR}
        COMPONENT
            executables
        )
]]
target_link_libraries(clientData1 
            microxrcedds_client 
            $<$<C_COMPILER_ID:GNU>:-Wl,--gc-section,--no-export-dynamic> 
            microcdr)
#[[
target_link_libraries(agent 
            $<$<C_COMPILER_ID:GNU>:-Wl,--gc-section,--no-export-dynamic>
            $<$<BOOL:$<PLATFORM_ID:Linux>>:rt>
            $<$<BOOL:$<PLATFORM_ID:Linux>>:dl>
            ${MicroDDS_Agent}
            microxrcedds_client
            spdlog
            microcdr
            fastrtps
        )]]
target_link_libraries(center 
            $<$<C_COMPILER_ID:GNU>:-Wl,--gc-section,--no-export-dynamic>
            $<$<BOOL:$<PLATFORM_ID:Linux>>:rt>
            $<$<BOOL:$<PLATFORM_ID:Linux>>:dl>
            ${MicroDDS_Agent}
            microxrcedds_client
            spdlog
            microcdr
            fastrtps
        )
#[[
target_link_libraries(server 
            asan
            $<$<C_COMPILER_ID:GNU>:-Wl,--gc-section,--no-export-dynamic>
            $<$<BOOL:$<PLATFORM_ID:Linux>>:rt>
            $<$<BOOL:$<PLATFORM_ID:Linux>>:dl>
            ${MicroDDS_Agent}
            microxrcedds_client
            spdlog
            microcdr
            fastrtps
        )]]
target_link_libraries(clientData1 
            $<$<C_COMPILER_ID:GNU>:-Wl,--gc-section,--no-export-dynamic>
            $<$<BOOL:$<PLATFORM_ID:Linux>>:rt>
            $<$<BOOL:$<PLATFORM_ID:Linux>>:dl>
            ${MicroDDS_Agent}
            microxrcedds_client
            spdlog
            microcdr
            fastrtps
        )
target_link_libraries(clientData2 
            $<$<C_COMPILER_ID:GNU>:-Wl,--gc-section,--no-export-dynamic>
            $<$<BOOL:$<PLATFORM_ID:Linux>>:rt>
            $<$<BOOL:$<PLATFORM_ID:Linux>>:dl>
            ${MicroDDS_Agent}
            microxrcedds_client
            spdlog
            microcdr
            fastrtps
        )
target_link_libraries(clientData3 
            $<$<C_COMPILER_ID:GNU>:-Wl,--gc-section,--no-export-dynamic>
            $<$<BOOL:$<PLATFORM_ID:Linux>>:rt>
            $<$<BOOL:$<PLATFORM_ID:Linux>>:dl>
            ${MicroDDS_Agent}
            microxrcedds_client
            spdlog
            microcdr
            fastrtps
        )
#target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(center ${CMAKE_THREAD_LIBS_INIT})


