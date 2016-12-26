cmake_minimum_required(VERSION 3.5)
project(odbSectionsSample)

set(CMAKE_CXX_STANDARD 11)

set(depdir /Users/lobermann/devel/dev_libs/cpp/installed/usr/local)
set(libdir ${depdir}/lib)

#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /Users/Shared/xbmc-depends/macosx10.12_x86_64-target)
#set(CMAKE_LIBRARY_PATH  ${CMAKE_LIBRARY_PATH} /Users/Shared/xbmc-depends/macosx10.12_x86_64-target/lib)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${depdir})
set(CMAKE_LIBRARY_PATH  ${CMAKE_LIBRARY_PATH} ${libdir})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/modules)
include(modules/UseODB.cmake)

#set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -no-cpp-precomp -arch x86_64 -stdlib=libc++ -g -O0 ")

set(ODB_COMPILE_DEBUG true)

find_package(ODB REQUIRED COMPONENTS
        sqlite
        mysql
        )

include_directories(${depdir}/include)
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
        CPP11
        )

set(SOURCES ${OdbCMake_SOURCES})
set(HEADERS ${OdbCMake_ODB_HEADERS})

foreach(header ${OdbCMake_ODB_HEADERS})
    configure_file(${header} ${ODB_COMPILE_OUTPUT_DIR}/${header} COPYONLY)
endforeach()

add_library(odb_binding STATIC ${SOURCES})
set_target_properties(odb_binding PROPERTIES LINKER_LANGUAGE CXX)

set(SOURCE_FILES main.cpp)
add_executable(odbSectionsSample ${SOURCE_FILES})
target_link_libraries(odbSectionsSample
        odb_binding
        odb
        odb-sqlite
        odb-mysql
        sqlite3
        mysqlclient
        #${CMAKE_LIBRARY_PATH}/libodb.a
        #${CMAKE_LIBRARY_PATH}/libodb-mysql.a
        #${CMAKE_LIBRARY_PATH}/libodb-sqlite.a
        #${CMAKE_LIBRARY_PATH}/mysql/libmysqlclient.a
        #${CMAKE_LIBRARY_PATH}/libsqlite3.a
        #${CMAKE_LIBRARY_PATH}/libssl.a
        #${CMAKE_LIBRARY_PATH}/libcurl.a
        #${CMAKE_LIBRARY_PATH}/libcrypto.a
        )

find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( odbSectionsSample ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )