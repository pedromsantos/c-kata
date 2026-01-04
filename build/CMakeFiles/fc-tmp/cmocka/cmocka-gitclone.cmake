# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

if(EXISTS "/Users/pedro/src/c-kata/build/CMakeFiles/fc-stamp/cmocka/cmocka-gitclone-lastrun.txt" AND EXISTS "/Users/pedro/src/c-kata/build/CMakeFiles/fc-stamp/cmocka/cmocka-gitinfo.txt" AND
  "/Users/pedro/src/c-kata/build/CMakeFiles/fc-stamp/cmocka/cmocka-gitclone-lastrun.txt" IS_NEWER_THAN "/Users/pedro/src/c-kata/build/CMakeFiles/fc-stamp/cmocka/cmocka-gitinfo.txt")
  message(VERBOSE
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/Users/pedro/src/c-kata/build/CMakeFiles/fc-stamp/cmocka/cmocka-gitclone-lastrun.txt'"
  )
  return()
endif()

# Even at VERBOSE level, we don't want to see the commands executed, but
# enabling them to be shown for DEBUG may be useful to help diagnose problems.
cmake_language(GET_MESSAGE_LOG_LEVEL active_log_level)
if(active_log_level MATCHES "DEBUG|TRACE")
  set(maybe_show_command COMMAND_ECHO STDOUT)
else()
  set(maybe_show_command "")
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/Users/pedro/src/c-kata/build/_deps/cmocka-src"
  RESULT_VARIABLE error_code
  ${maybe_show_command}
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/Users/pedro/src/c-kata/build/_deps/cmocka-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/opt/homebrew/bin/git"
            clone --no-checkout --depth 1 --no-single-branch --config "advice.detachedHead=false" -c http.sslVerify=false "https://git.cryptomilk.org/projects/cmocka.git" "cmocka-src"
    WORKING_DIRECTORY "/Users/pedro/src/c-kata/build/_deps"
    RESULT_VARIABLE error_code
    ${maybe_show_command}
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(NOTICE "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://git.cryptomilk.org/projects/cmocka.git'")
endif()

execute_process(
  COMMAND "/opt/homebrew/bin/git"
          checkout "cmocka-1.1.5" --
  WORKING_DIRECTORY "/Users/pedro/src/c-kata/build/_deps/cmocka-src"
  RESULT_VARIABLE error_code
  ${maybe_show_command}
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'cmocka-1.1.5'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/opt/homebrew/bin/git" -c;http.sslVerify=false
            submodule update --recursive --init 
    WORKING_DIRECTORY "/Users/pedro/src/c-kata/build/_deps/cmocka-src"
    RESULT_VARIABLE error_code
    ${maybe_show_command}
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/Users/pedro/src/c-kata/build/_deps/cmocka-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/Users/pedro/src/c-kata/build/CMakeFiles/fc-stamp/cmocka/cmocka-gitinfo.txt" "/Users/pedro/src/c-kata/build/CMakeFiles/fc-stamp/cmocka/cmocka-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  ${maybe_show_command}
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/Users/pedro/src/c-kata/build/CMakeFiles/fc-stamp/cmocka/cmocka-gitclone-lastrun.txt'")
endif()
