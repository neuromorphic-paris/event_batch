# Library
add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME} INTERFACE ${${LIB_NAME}_INCLUDE_DIR}
  ${pontella_SOURCE_DIR}/source ${sepia_SOURCE_DIR}/source)
target_link_libraries(${LIB_NAME} INTERFACE pthread)
