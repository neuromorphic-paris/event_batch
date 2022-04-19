# Disable installing googletest
option(INSTALL_GTEST "" OFF)
set(INSTALL_GTEST OFF)

# Download googletest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929 # release-1.11.0
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Function to add new tests
function(add_new_test ARG)
  add_executable(test_${ARG} ${ARG}.cpp)
  target_link_libraries(test_${ARG} ${LIB_NAME} gtest_main)
  gtest_discover_tests(test_${ARG}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endfunction()

# List of tests
add_new_test(batch)
add_new_test(event_stream_statistics)
add_new_test(global_decay)
