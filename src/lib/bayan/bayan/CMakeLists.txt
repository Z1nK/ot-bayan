set(LOCAL_LIB_NAME bayan-core)

add_library(${LOCAL_LIB_NAME}
  bayan.cpp
  bayan.hpp
)

add_library(bayan::core ALIAS ${LOCAL_LIB_NAME})

target_include_directories(${LOCAL_LIB_NAME} PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
  )

target_link_libraries(${LOCAL_LIB_NAME} PUBLIC
  cli-parser::cli-parser
  bayan::project-version
  bayan::filesystem-helper
  bayan::duplicate-finder
)
