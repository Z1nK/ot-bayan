
set(LOCAL_LIB_NAME hash-methods)

add_library(${LOCAL_LIB_NAME} INTERFACE)

add_library(bayan::hash-methods ALIAS ${LOCAL_LIB_NAME})

target_include_directories(${LOCAL_LIB_NAME} INTERFACE
  $<INSTALL_INTERFACE:include>  
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
  )