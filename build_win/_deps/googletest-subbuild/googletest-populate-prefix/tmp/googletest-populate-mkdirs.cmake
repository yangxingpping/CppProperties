# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/share/CppProperties/build_win/_deps/googletest-src"
  "D:/share/CppProperties/build_win/_deps/googletest-build"
  "D:/share/CppProperties/build_win/_deps/googletest-subbuild/googletest-populate-prefix"
  "D:/share/CppProperties/build_win/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "D:/share/CppProperties/build_win/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "D:/share/CppProperties/build_win/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "D:/share/CppProperties/build_win/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/share/CppProperties/build_win/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/share/CppProperties/build_win/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
