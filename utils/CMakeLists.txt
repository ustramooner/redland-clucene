project(redlandpp-utils)

include_directories(
  ${redlandpp_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  /usr/include/rasqal
)

add_executable(rdfprocpp rdfprocpp.cpp)
target_link_libraries(rdfprocpp redlandpp rdf )
