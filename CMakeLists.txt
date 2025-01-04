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
add_dependencies(simple_ffmpeg_player FFmpeg)

# Ссылка на библиотеки FFmpeg
target_link_libraries(simple_ffmpeg_player PRIVATE avcodec avformat avutil swresample)


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
