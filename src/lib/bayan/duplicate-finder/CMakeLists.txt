set(LOCAL_LIB_NAME duplicate-finder)

add_library(${LOCAL_LIB_NAME}
  duplicate_finder.cpp
  duplicate_finder.hpp
)

add_library(bayan::duplicate-finder ALIAS ${LOCAL_LIB_NAME})

target_include_directories(${LOCAL_LIB_NAME} PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
  )

target_link_libraries(${LOCAL_LIB_NAME} PUBLIC
  bayan::filesystem-helper
)
