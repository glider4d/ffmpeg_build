# cmake_minimum_required(VERSION 3.10)

# project(SimpleFFmpegPlayer)

# # Добавляем поддерево с исходным кодом FFmpeg
# #add_subdirectory(external/ffmpeg)

# # Сборка FFmpeg (если необходимо)
# add_custom_command(
#     TARGET FFmpeg
#     COMMAND ./configure --enable-gpl --enable-libx264 --enable-libmp3lame --enable-libvorbis --enable-openssl
#     COMMAND make -j$(nproc)
#     WORKING_DIRECTORY external/ffmpeg
#     COMMENT "Compiling FFmpeg"
# )

# # Добавляем наш проект
# add_executable(simple_ffmpeg_player main.cpp)

# # Устанавливаем зависимости для нашего проекта
# target_link_libraries(simple_ffmpeg_player PRIVATE avcodec avformat avutil swresample)

cmake_minimum_required(VERSION 3.10)

project(SimpleFFmpegPlayer)

# Путь к каталогу, где расположены заголовочные файлы FFmpeg
set(FFMPEG_INCLUDE_DIR external/ffmpeg)
set(FFMPEG_DIR external/ffmpeg)

# Путь к каталогу, где расположены библиотеки FFmpeg
set(FFMPEG_LIB_DIR external/ffmpeg/lib)

# set(X264_DIR external/x264)




# cd x264 && \
# PATH="$HOME/bin:$PATH" PKG_CONFIG_PATH="$HOME/ffmpeg_build/lib/pkgconfig" ./configure --prefix="$HOME/ffmpeg_build" --bindir="$HOME/bin" --enable-static --enable-pic && \
# PATH="$HOME/bin:$PATH" make && \
# make install


set(X264_DIR ${CMAKE_SOURCE_DIR}/external/x264)
set(X264_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/external/x264/build)
set(X264_BIN_DIR ${CMAKE_SOURCE_DIR}/external/x264/bin)

set(X264_INCLUDE ${CMAKE_SOURCE_DIR}/external/x264/build/include)
set(X264_LIB ${CMAKE_SOURCE_DIR}/external/x264/build/lib)

file(MAKE_DIRECTORY ${X264_INSTALL_PREFIX})
file(MAKE_DIRECTORY ${X264_BIN_DIR})



# add_custom_target(
#     X264
#     COMMAND ./configure 
#     --prefix=${X264_INSTALL_PREFIX} 
#     --bindir=${X264_BIN_DIR} --enable-static --enable-pic
#     # --prefix=${X264_INSTALL_PREFIX}
#     # --bindir=${X264_BIN_DIR}
#     COMMAND make
#     WORKING_DIRECTORY ${X264_DIR}
#     COMMAND make install
#     #../external/x264
#     COMMENT "Compiling x264"
#)


add_custom_command(
    OUTPUT ${X264_LIB}/libx264.a

    # X264
    COMMAND ./configure 
    --prefix=${X264_INSTALL_PREFIX} 
    --bindir=${X264_BIN_DIR} --enable-static --enable-pic
    COMMAND make
    WORKING_DIRECTORY ${X264_DIR}
    COMMAND make install
    COMMENT "Compiling x264"
)

# add_custom_target(
#     x264 ALL
#     DEPENDS ${CMAKE_BINARY_DIR}/external/x264/build/lib/libx264.a
# )

# output lib: ./external/x264/build/lib/
# output include: ./external/x264/build/include/

include_directories(${X264_INCLUDE})
link_directories(${X264_LIB})

add_custom_target (
    x264 ALL 
    DEPENDS ${X264_LIB}/libx264.a
    COMMENT "Building x264"
)



add_custom_target(
    FFmpeg
    # COMMAND ./configure --enable-gpl --enable-libx264 --enable-libmp3lame --enable-libvorbis --enable-openssl --enable-version3
    COMMAND ./configure --prefix=$(pwd)/install --enable-shared --enable-gpl --enable-libx264 --enable-libmp3lame --enable-libvorbis --enable-openssl --enable-version3

    COMMAND make -j$(nproc)
    WORKING_DIRECTORY ../external/ffmpeg
    COMMENT "Compiling FFmpeg"
)



# Добавляем наш основной проект
add_executable(simple_ffmpeg_player main.cpp)



# Указываем CMake, где искать заголовочные файлы FFmpeg
include_directories(${FFMPEG_INCLUDE_DIR})

# Указываем CMake, где искать библиотеки FFmpeg
link_directories(${FFMPEG_LIB_DIR})



# Устанавливаем зависимости для нашего проекта
# Убедитесь, что вы указываете FFmpeg как зависимость для вашего проекта
# add_dependencies(simple_ffmpeg_player x264)
# add_dependencies(simple_ffmpeg_player FFmpeg)


# Ссылка на библиотеки FFmpeg
# target_link_libraries(simple_ffmpeg_player PRIVATE avcodec avformat avutil swresample)


# Сборка FFmpeg (если необходимо)
# add_custom_command(
#     TARGET FFmpeg
#     COMMAND ./configure --enable-gpl --enable-libx264 --enable-libmp3lame --enable-libvorbis --enable-openssl --enable-version3
#     COMMAND make -j$(nproc)
#     WORKING_DIRECTORY external/ffmpeg
#     COMMENT "Compiling FFmpeg"
# )

# # Проверка, создался ли TARGET 'FFmpeg'
# add_custom_target(
#     VerifyFFmpeg
#     DEPENDS FFmpeg
# )

# # Добавляем наш проект
# add_executable(simple_ffmpeg_player main.cpp)

# # Устанавливаем зависимости для нашего проекта
# target_link_libraries(simple_ffmpeg_player PRIVATE avcodec avformat avutil swresample)
