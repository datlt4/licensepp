cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(APP licensepp-generator-server)
project(${APP})

set(CMAKE_CXX_STANDARD 20)

set(SRC main.cpp base64.cpp)

find_package(Boost REQUIRED COMPONENTS system thread)
set(LIB ${LIB} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})

find_package(OpenSSL REQUIRED)
set(OPENSSL_USE_STATIC_LIBS TRUE)
set (LIB ${LIB} OpenSSL::Crypto)

set (LIB ${LIB} liblicensepp.so.1.0.6)

include_directories(${INC})
add_executable(${APP} ${SRC})
target_link_libraries(${APP} ${LIB})
