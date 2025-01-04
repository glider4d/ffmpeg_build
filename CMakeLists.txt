cmake_minimum_required(VERSION 3.10)

project(SimpleFFmpegPlayer)

# Добавляем поддерево с исходным кодом FFmpeg
add_subdirectory(external/ffmpeg)

# Сборка FFmpeg
add_custom_command(
    TARGET FFmpeg
    COMMAND ./configure --enable-gpl --enable-libx264 --enable-libmp3lame --enable-libvorbis --enable-openssl
    COMMAND make -j$(nproc)
    WORKING_DIRECTORY external/ffmpeg
    COMMENT "Compiling FFmpeg"
)

# Добавляем наш проект
add_executable(simple_ffmpeg_player main.cpp)

# Устанавливаем зависимости для нашего проекта
target_link_libraries(simple_ffmpeg_player PRIVATE avcodec avformat avutil swresample)
