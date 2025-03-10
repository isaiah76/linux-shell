# Look for the readline header file
find_path(READLINE_INCLUDE_DIR readline/readline.h
  PATHS /usr/include /usr/local/include
  DOC "Path to readline include directory"
)

# Look for the readline library
find_library(READLINE_LIBRARY readline
  PATHS /usr/lib /usr/local/lib
  DOC "Path to readline library"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Readline DEFAULT_MSG READLINE_LIBRARY READLINE_INCLUDE_DIR)

if(READLINE_FOUND)
  set(Readline_INCLUDE_DIRS ${READLINE_INCLUDE_DIR})
  set(Readline_LIBRARIES ${READLINE_LIBRARY})
endif()

