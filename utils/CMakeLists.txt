project(redlandpp-utils)

include_directories(
  ${redlandpp_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  /usr/include/rasqal
)

add_executable(rdfprocpp rdfprocpp.cpp)
target_link_libraries(rdfprocpp redlandpp clucene /home/ben/dev/clucene-rdf/redland-1.0.9/gcc/install/lib/librdf.so )

add_executable(rdfprocpp2 rdfprocpp2.cpp)
target_link_libraries(rdfprocpp2 redlandpp clucene /home/ben/dev/clucene-rdf/redland-1.0.9/gcc/install/lib/librdf.so )
