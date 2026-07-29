set(LOCAL_LIB_NAME cli-parser)

add_library(${LOCAL_LIB_NAME} 
  cli-parser.cpp
  cli-parser.hpp
)

add_library(cli-parser::cli-parser ALIAS ${LOCAL_LIB_NAME})

target_include_directories(${LOCAL_LIB_NAME} PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>
  )

target_link_libraries(${LOCAL_LIB_NAME} PUBLIC 
  Boost::program_options
  bayan::project-version
)
