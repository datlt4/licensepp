# CMakeLists.txt
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set(APP license-device)
project(${APP})

set(SRC ${SRC} license_device.cpp)
find_package(OpenSSL REQUIRED)
set(OPENSSL_USE_STATIC_LIBS TRUE)
set (LIB ${LIB} OpenSSL::Crypto)

set (LIB ${LIB} liblicensepp.so.1.0.6)
add_executable(${APP} ${SRC})
target_link_libraries(${APP} ${LIB})

set_property(TARGET ${APP} PROPERTY CXX_STANDARD 20)
