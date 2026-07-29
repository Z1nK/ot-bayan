
set(LOCAL_LIB_NAME prversion)

add_library(${LOCAL_LIB_NAME} INTERFACE)

add_library(bayan::project-version ALIAS ${LOCAL_LIB_NAME})

target_include_directories(${LOCAL_LIB_NAME} INTERFACE
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${OT_BAYAN_AUTOGEN_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
  )