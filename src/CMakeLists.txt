add_subdirectory(${LIB_NAME})

# Build runtime benchmarks
if(${LIB_NAME}_BUILD_RUNTIME_BENCHMARK)
  add_subdirectory(runtime_benchmark)
endif()

# Function to add new executables
function(add_new_executable ARG)
  add_executable(${ARG} ${ARG}.cpp)
  target_link_libraries(${ARG} ${LIB_NAME})
endfunction()

# List of executables
add_new_executable(batch_size)
add_new_executable(batch_timestamp)
