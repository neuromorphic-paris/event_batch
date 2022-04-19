# Function to add new runtime benchmarks
function(add_new_runtime ARG)
  add_executable(runtime_${ARG} ${ARG}.cpp)
  target_link_libraries(runtime_${ARG} ${LIB_NAME})
endfunction()

# List of runtime benchmarks
add_new_runtime(batch)
add_new_runtime(event_stream_statistics)
add_new_runtime(global_decay)
