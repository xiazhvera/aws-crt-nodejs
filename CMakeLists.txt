cmake_minimum_required(VERSION 3.1)
project(aws-crt-nodejs C)
option(BUILD_DEPS "Builds aws common runtime dependencies as part of build, only do this if you don't want to control your dependency chain." ON)

option(CMAKE_JS_PLATFORM "Target platform. Should match node's os.platform()")
if (NOT CMAKE_JS_PLATFORM)
    message(FATAL_ERROR "CMAKE_JS_PLATFORM must be set")
endif()

include(CTest)

# ensure that the release build has symbols
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

if (UNIX AND NOT APPLE)
    include(GNUInstallDirs)
elseif(NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif()

if (${CMAKE_INSTALL_LIBDIR} STREQUAL "lib64")
    set(FIND_LIBRARY_USE_LIB64_PATHS true)
endif()

# This is required in order to append /lib/cmake to each element in CMAKE_PREFIX_PATH
set(AWS_MODULE_DIR "/${CMAKE_INSTALL_LIBDIR}/cmake")
string(REPLACE ";" "${AWS_MODULE_DIR};" AWS_MODULE_PATH "${CMAKE_PREFIX_PATH}${AWS_MODULE_DIR}")
# Append that generated list to the module search path
list(APPEND CMAKE_MODULE_PATH ${AWS_MODULE_PATH})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/crt/aws-c-common/cmake")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_PREFIX_PATH}/${CMAKE_INSTALL_LIBDIR}/cmake")

include(AwsCFlags)
include(AwsSharedLibSetup)
include(AwsSanitizers)
include(AwsCRuntime)

aws_determine_local_c_runtime(AWS_C_RUNTIME)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/dist")


if (POLICY CMP0069)
    cmake_policy(SET CMP0069 NEW) # Enable LTO/IPO if available in the compiler, see AwsCFlags
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_PREFIX_PATH}/${CMAKE_INSTALL_LIBDIR}/cmake")

file(GLOB AWS_CRT_SRC
       "source/*.c"
)

add_library(${PROJECT_NAME} SHARED ${AWS_CRT_SRC})


# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(${PROJECT_NAME} PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${CMAKE_JS_INC})

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_JS_LIB} ${DEP_AWS_LIBS})

set(destination bin/${CMAKE_JS_PLATFORM}-${NODE_ARCH}-${AWS_C_RUNTIME})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/aws-crt-nodejs.node"
    DESTINATION ${destination})
