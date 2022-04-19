# Check if Doxygen is installed
find_package(Doxygen REQUIRED)

# Find public headers
file(GLOB_RECURSE ${LIB_NAME}_PUBLIC_HEADERS ${${LIB_NAME}_INCLUDE_DIR}/*.hpp)

# Set doxygen variables
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.html)

# Configure Doxyfile
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Doxygen won't create this folder
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

# Add command and target
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                   DEPENDS ${${LIB_NAME}_PUBLIC_HEADERS}
                   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                   MAIN_DEPENDENCY ${DOXYFILE_IN}
                   COMMENT "Generating documentation with doxygen"
                   VERBATIM)

# General docs target
add_custom_target(doc DEPENDS ${DOXYGEN_INDEX_FILE})
