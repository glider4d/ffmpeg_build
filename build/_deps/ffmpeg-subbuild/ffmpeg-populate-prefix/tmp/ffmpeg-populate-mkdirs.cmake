# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/glider/repo_emcc/ffmpeg_project/build/_deps/ffmpeg-src"
  "/home/glider/repo_emcc/ffmpeg_project/build/_deps/ffmpeg-build"
  "/home/glider/repo_emcc/ffmpeg_project/build/_deps/ffmpeg-subbuild/ffmpeg-populate-prefix"
  "/home/glider/repo_emcc/ffmpeg_project/build/_deps/ffmpeg-subbuild/ffmpeg-populate-prefix/tmp"
  "/home/glider/repo_emcc/ffmpeg_project/build/_deps/ffmpeg-subbuild/ffmpeg-populate-prefix/src/ffmpeg-populate-stamp"
  "/home/glider/repo_emcc/ffmpeg_project/build/_deps/ffmpeg-subbuild/ffmpeg-populate-prefix/src"
  "/home/glider/repo_emcc/ffmpeg_project/build/_deps/ffmpeg-subbuild/ffmpeg-populate-prefix/src/ffmpeg-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/glider/repo_emcc/ffmpeg_project/build/_deps/ffmpeg-subbuild/ffmpeg-populate-prefix/src/ffmpeg-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/glider/repo_emcc/ffmpeg_project/build/_deps/ffmpeg-subbuild/ffmpeg-populate-prefix/src/ffmpeg-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
