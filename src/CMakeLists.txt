cmake_policy(SET CMP0003 NEW)

project(redlandpp-core)

include_directories(
  ${redlandpp_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  "/usr/include/rasqal"
)

set( REDLANDPP_CORE_SRCS
  Exception.cpp
  Log.cpp
  Model.cpp
  Node.cpp
  Parser.cpp
  Serializer.cpp
  Statement.cpp
  Storage.cpp
  Stream.cpp
  Uri.cpp
  World.cpp
)
#find our headers
file(GLOB_RECURSE REDLANDPP_CORE_HEADERS ${redlandpp-SOURCE_DIR}/redlandpp/*.h)


CHECK_INCLUDE_FILE_CXX ( dlfcn.h HAVE_DLFCN_H )
CHECK_INCLUDE_FILE_CXX ( inttypes.h HAVE_INTTYPES_H )
CHECK_INCLUDE_FILE_CXX ( memory.h HAVE_MEMORY_H )
CHECK_INCLUDE_FILE_CXX ( redland.h HAVE_REDLAND_H )
CHECK_INCLUDE_FILE_CXX ( stdint.h HAVE_STDINT_H )
CHECK_INCLUDE_FILE_CXX ( stdlib.h HAVE_STDLIB_H )
CHECK_INCLUDE_FILE_CXX ( strings.h HAVE_STRINGS_H )
CHECK_INCLUDE_FILE_CXX ( string.h HAVE_STRING_H )
CHECK_INCLUDE_FILE_CXX ( sys/stat.h HAVE_SYS_STAT_H )
CHECK_INCLUDE_FILE_CXX ( sys/types.h HAVE_SYS_TYPES_H )
CHECK_INCLUDE_FILE_CXX ( unistd.h HAVE_UNISTD_H )


configure_file(${redlandpp-core_SOURCE_DIR}/redlandpp_config.h.cmake ${redlandpp-core_BINARY_DIR}/redlandpp_config.h)

add_library(redlandpp SHARED ${REDLANDPP_CORE_SRCS})

#target_link_libraries(
  #redlandpp
  #redland
#)

set_target_properties(
  redlandpp
  PROPERTIES
  VERSION 1.1.0
  SOVERSION 1
  DEFINE_SYMBOL MAKE_REDLANDPP_LIB
  INSTALL_NAME_DIR ${LIB_DESTINATION}
)

INSTALL(TARGETS redlandpp
	LIBRARY DESTINATION ${LIB_DESTINATION}
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
)

install(
  FILES 
  indexfiltermodel.h
  cluceneindex.h
  indexqueryhit.h
  DESTINATION include/soprano
)
