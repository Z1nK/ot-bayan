set(LOCAL_LIB_NAME filesystem-helper)

add_library(${LOCAL_LIB_NAME} 
  filesystem.cpp
  filesystem.hpp
)

add_library(bayan::filesystem-helper ALIAS ${LOCAL_LIB_NAME})

target_include_directories(${LOCAL_LIB_NAME} PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
  )

target_link_libraries(${LOCAL_LIB_NAME} PUBLIC 
  Boost::filesystem
)
