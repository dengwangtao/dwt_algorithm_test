# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/22379/Desktop/algorithm/out/build/GCC 15.1.0 x86_64-w64-mingw32/_deps/googletest-src")
  file(MAKE_DIRECTORY "C:/Users/22379/Desktop/algorithm/out/build/GCC 15.1.0 x86_64-w64-mingw32/_deps/googletest-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/22379/Desktop/algorithm/out/build/GCC 15.1.0 x86_64-w64-mingw32/_deps/googletest-build"
  "C:/Users/22379/Desktop/algorithm/out/build/GCC 15.1.0 x86_64-w64-mingw32/_deps/googletest-subbuild/googletest-populate-prefix"
  "C:/Users/22379/Desktop/algorithm/out/build/GCC 15.1.0 x86_64-w64-mingw32/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "C:/Users/22379/Desktop/algorithm/out/build/GCC 15.1.0 x86_64-w64-mingw32/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "C:/Users/22379/Desktop/algorithm/out/build/GCC 15.1.0 x86_64-w64-mingw32/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "C:/Users/22379/Desktop/algorithm/out/build/GCC 15.1.0 x86_64-w64-mingw32/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/22379/Desktop/algorithm/out/build/GCC 15.1.0 x86_64-w64-mingw32/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/22379/Desktop/algorithm/out/build/GCC 15.1.0 x86_64-w64-mingw32/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
