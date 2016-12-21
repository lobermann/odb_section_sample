cmake_minimum_required(VERSION 3.6)
project(odbSectionsSample)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /Users/Shared/xbmc-depends/macosx10.12_x86_64-target)
set(CMAKE_LIBRARY_PATH  ${CMAKE_LIBRARY_PATH} /Users/Shared/xbmc-depends/macosx10.12_x86_64-target/lib)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/modules)
include(modules/UseODB.cmake)

find_package(ODB REQUIRED COMPONENTS sqlite mysql)

include_directories(/Users/Shared/xbmc-depends/macosx10.12_x86_64-target/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(OdbCMake_ODB_HEADERS
        ODBFile.h
        ODBData.h)

odb_compile(OdbCMake_SOURCES FILES ${OdbCMake_ODB_HEADERS}
        MULTI_DATABASE dynamic
        DB sqlite mysql
        GENERATE_QUERY
        GENERATE_SESSION
        GENERATE_SCHEMA
        GENERATE_PREPARED
        SCHEMA_FORMAT embedded
        CPP11)

set(SOURCES ${OdbCMake_SOURCES})
set(HEADERS ${OdbCMake_ODB_HEADERS})

foreach(header ${OdbCMake_ODB_HEADERS})
    configure_file(${header} ${ODB_COMPILE_OUTPUT_DIR}/${header} COPYONLY)
endforeach()

add_library(odb_binding STATIC ${SOURCES} ${HEADERS})
set_target_properties(odb_binding PROPERTIES LINKER_LANGUAGE CXX)

set(SOURCE_FILES main.cpp)
add_executable(odbSectionsSample ${SOURCE_FILES})
target_link_libraries(odbSectionsSample
        odb_binding
        ${CMAKE_LIBRARY_PATH}/libodb.a
        ${CMAKE_LIBRARY_PATH}/libodb-mysql.a
        ${CMAKE_LIBRARY_PATH}/libodb-sqlite.a
        ${CMAKE_LIBRARY_PATH}/mysql/libmysqlclient.a
        ${CMAKE_LIBRARY_PATH}/libsqlite3.a
        ${CMAKE_LIBRARY_PATH}/libssl.a
        ${CMAKE_LIBRARY_PATH}/libcurl.a
        ${CMAKE_LIBRARY_PATH}/libcrypto.a)

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( odbSectionsSample ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )