
cmake_minimum_required(VERSION 3.10)

project(SimpleFFmpegPlayer)

# ffmpeg path
set(FFMPEG_INCLUDE_DIR external/ffmpeg)
set(FFMPEG_DIR external/ffmpeg)
set(FFMPEG_LIB_DIR external/ffmpeg/lib)



#################################x264 BEGIN

set(X264_DIR ${CMAKE_SOURCE_DIR}/external/x264)
set(X264_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/external/x264/build)
set(X264_BIN_DIR ${CMAKE_SOURCE_DIR}/external/x264/bin)

set(X264_INCLUDE ${CMAKE_SOURCE_DIR}/external/x264/build/include)
set(X264_LIB ${CMAKE_SOURCE_DIR}/external/x264/build/lib)

file(MAKE_DIRECTORY ${X264_INSTALL_PREFIX})
file(MAKE_DIRECTORY ${X264_BIN_DIR})


 

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

 
include_directories(${X264_INCLUDE})
link_directories(${X264_LIB})

add_custom_target (
    x264 ALL 
    DEPENDS ${X264_LIB}/libx264.a
    COMMENT "Building x264"
)


#################################x264 END

#################################x265 BEGIN
set(X265_DIR ${CMAKE_SOURCE_DIR}/external/x265)
set(X265_LIB ${X265_DIR}/build/linux)
set(X265_INCLUDE ${X265_LIB})
set(X265_SRC ${X265_DIR}/source)

add_subdirectory(${X265_SRC} ${X265_LIB})

include_directories(${X265_INCLUDE})
link_directories(${X265_LIB})
#################################x265 END

#################################libvpx BEGIN

set(LIBVPX_DIR ${CMAKE_SOURCE_DIR}/external/libvpx)
set(LIBVPX_BUILD ${LIBVPX_DIR}/build_lib)
set(LIBVPX_INCLUDE ${LIBVPX_BUILD}/include)
set(LIBVPX_LIB ${LIBVPX_BUILD}/lib)

file(MAKE_DIRECTORY ${LIBVPX_BUILD})

add_custom_command(
    OUTPUT ${LIBVPX_LIB}/libvpx.a    
    COMMAND ./configure 
    --prefix=${LIBVPX_BUILD} 
    --disable-examples --disable-unit-tests --enable-vp9-highbitdepth --as=yasm 
    COMMAND make
    WORKING_DIRECTORY ${LIBVPX_DIR}
    COMMAND make install
    COMMENT "Compiling libvpx"
)

add_custom_target(
    LIBVPX ALL
    DEPENDS ${LIBVPX_LIB}/libvpx.a
    COMMENT "Building libvpx"
)

include_directories(${LIBVPX_INCLUDE})
link_directories(${LIBVPX_LIB})

#################################libvpx END
 
#################################libfdk-aac BEGIN

set(LIBFDKAAC_DIR ${CMAKE_SOURCE_DIR}/external/libfdk-aac)
set(LIBFDKAAC_BUILD ${LIBFDKAAC_DIR}/build_lib)
set(LIBFDKAAC_INCLUDE ${LIBFDKAAC_BUILD}/include)
set(LIBFDKAAC_LIB ${LIBFDKAAC_BUILD}/lib)

file(MAKE_DIRECTORY ${LIBFDKAAC_BUILD})

add_custom_command(
    OUTPUT ${LIBFDKAAC_LIB}/libfdk-aac.a    
    COMMAND autoreconf -fiv
    COMMAND ./configure 
    --prefix=${LIBFDKAAC_BUILD} 
    --disable-shared 
    COMMAND make
    WORKING_DIRECTORY ${LIBFDKAAC_DIR}
    COMMAND make install
    COMMENT "Compiling libfdk-aac"
)

add_custom_target(
    LIBFDKAAC ALL
    DEPENDS ${LIBFDKAAC_LIB}/libfdk-aac.a
    COMMENT "Building libfdk-aac"
)

include_directories(${LIBFDKAAC_INCLUDE})
link_directories(${LIBFDKAAC_LIB})

#################################libfdk-aac END

#################################libopus BEGIN

set(LIBOPUS_DIR ${CMAKE_SOURCE_DIR}/external/libopus)
set(LIBOPUS_BUILD ${LIBOPUS_DIR}/build_lib)
set(LIBOPUS_INCLUDE ${LIBOPUS_BUILD}/include)
set(LIBOPUS_LIB ${LIBOPUS_BUILD}/lib)

file(MAKE_DIRECTORY ${LIBOPUS_BUILD})

# ./autogen.sh && \
# ./configure --prefix="$HOME/ffmpeg_build" --disable-shared && \


add_custom_command(
    OUTPUT ${LIBOPUS_LIB}/libopus.a    
    COMMAND ./autogen.sh
    COMMAND ./configure 
    --prefix=${LIBOPUS_BUILD} 
    --disable-shared 
    COMMAND make
    WORKING_DIRECTORY ${LIBOPUS_DIR}
    COMMAND make install
    COMMENT "Compiling libopus"
)

add_custom_target(
    LIBOPUS ALL
    DEPENDS ${LIBOPUS_LIB}/libopus.a
    COMMENT "Building libopus"
)

include_directories(${LIBOPUS_INCLUDE})
link_directories(${LIBOPUS_LIB})

#################################libopus END

add_custom_target(
    FFmpeg
    COMMAND ./configure --prefix=$(pwd)/install --enable-shared --enable-gpl --enable-libx264 --enable-libmp3lame --enable-libvorbis --enable-openssl --enable-version3
    COMMAND make -j$(nproc)
    WORKING_DIRECTORY ../external/ffmpeg
    COMMENT "Compiling FFmpeg"
)



# main executable
add_executable(simple_ffmpeg_player main.cpp)



# ffmpeg include path
include_directories(${FFMPEG_INCLUDE_DIR})

#  ffmpeg lib path
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
