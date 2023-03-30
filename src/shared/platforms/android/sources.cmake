# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

get_filename_component(MZ_SHARED_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/shared ABSOLUTE)

target_sources(shared-sources INTERFACE
     ${MZ_SHARED_SOURCE_DIR}/platforms/wasm/wasmcryptosettings.cpp
     ${MZ_SHARED_SOURCE_DIR}/platforms/android/androidcommons.cpp
     ${MZ_SHARED_SOURCE_DIR}/platforms/android/androidcommons.h
)

if(ADJUST_TOKEN)
    message(Adjust SDK enabled)
    # SDK Token present, let's enable that.
    target_compile_definitions(shared-sources INTERFACE MZ_ADJUST)

    target_sources(shared-sources INTERFACE
        ${MZ_SHARED_SOURCE_DIR}/adjust/adjustfiltering.cpp
        ${MZ_SHARED_SOURCE_DIR}/adjust/adjustfiltering.h
        ${MZ_SHARED_SOURCE_DIR}/adjust/adjusthandler.cpp
        ${MZ_SHARED_SOURCE_DIR}/adjust/adjusthandler.h
        ${MZ_SHARED_SOURCE_DIR}/adjust/adjustproxy.cpp
        ${MZ_SHARED_SOURCE_DIR}/adjust/adjustproxy.h
        ${MZ_SHARED_SOURCE_DIR}/adjust/adjustproxyconnection.cpp
        ${MZ_SHARED_SOURCE_DIR}/adjust/adjustproxyconnection.h
        ${MZ_SHARED_SOURCE_DIR}/adjust/adjustproxypackagehandler.cpp
        ${MZ_SHARED_SOURCE_DIR}/adjust/adjustproxypackagehandler.h
        ${MZ_SHARED_SOURCE_DIR}/adjust/adjusttasksubmission.cpp
        ${MZ_SHARED_SOURCE_DIR}/adjust/adjusttasksubmission.h
    )
else()
    if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        message(${CMAKE_BUILD_TYPE})
        message( FATAL_ERROR "Adjust token cannot be empty for release builds")
    endif()
endif()

add_dependencies(shared-sources ndk_openssl_merged)

get_property(crypto_module GLOBAL PROPERTY OPENSSL_CRYPTO_MODULE)
get_property(ssl_module GLOBAL PROPERTY OPENSSL_SSL_MODULE)

target_include_directories(shared-sources INTERFACE ${ssl_module}/include)

get_property(openssl_libs GLOBAL PROPERTY OPENSSL_LIBS)
set_property(TARGET shared-sources PROPERTY QT_ANDROID_EXTRA_LIBS
    ${openssl_libs}/libcrypto_1_1.so
    ${openssl_libs}/libssl_1_1.so)

target_link_directories(shared-sources INTERFACE ${openssl_libs})
target_link_libraries(shared-sources INTERFACE libcrypto.so)
target_link_libraries(shared-sources INTERFACE libssl.so)
target_link_libraries(shared-sources INTERFACE -ljnigraphics)
