
cmake_minimum_required(VERSION 3.10)

project(SimpleFFmpegPlayer)


set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)


set(FFMPEG_BUILD ${CMAKE_SOURCE_DIR}/build/ffmpeg_build)
set(FFMPEG_BIN ${CMAKE_SOURCE_DIR}/build/bin)
set(FFMPEG_BUILD_LIB ${FFMPEG_BUILD}/lib)

# file(MAKE_DIRECTORY ${FFMPEG_BUILD})
# file(MAKE_DIRECTORY ${FFMPEG_BIN})
# file(MAKE_DIRECTORY ${FFMPEG_BUILD}/lib)


# file(MAKE_DIRECTORY ${EXTERNAL_DIR})

# add_custom_target(run_script ALL
#     COMMAND /bin/bash ${CMAKE_SOURCE_DIR}/init.sh
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#     COMMENT "Running iniy.sh"
# )

execute_process(
    COMMAND /bin/bash ${CMAKE_SOURCE_DIR}/init.sh
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE INIT_RESULT
    OUTPUT_VARIABLE INIT_OUTPUT
    ERROR_VARIABLE INIT_ERROR
)

if (NOT INIT_RESULT EQUAL 0)
    message(FATAL_ERROR "Error during run_script:\n${INIT_ERROR}")
endif()

message(STATUS "run_script output:\n${INIT_OUTPUT}")


add_custom_target(create_directories
    COMMAND ${CMAKE_COMMAND} -E make_directory ${FFMPEG_BUILD}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${FFMPEG_BIN}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${FFMPEG_BUILD}/lib
    COMMAND ${CMAKE_COMMAND} -E make_directory ${EXTERNAL_DIR}
    COMMENT "Creating required directories"
)

add_dependencies(create_directories run_script)


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
set(X265_INCLUDE ${X265_DIR}/source)
set(X265_SRC ${X265_DIR}/source)

set(PKG_CONFIG_NAME x265)
# add_subdirectory(${X265_SRC} ${X265_LIB})


# PATH="$HOME/bin:$PATH" cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$HOME/ffmpeg_build" -DENABLE_SHARED=off ../../source && \
# PATH="$HOME/bin:$PATH" make && \
# make install

add_custom_command(
    OUTPUT ${X265_LIB}/libx265.a    
    COMMAND cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="${FFMPEG_BUILD}" -DENABLE_SHARED=off ../../source
    COMMAND make
    WORKING_DIRECTORY ${X265_LIB}
    # COMMAND make install
    COMMENT "Compiling libx265"
)

add_custom_target(
    LIBX265 ALL
    DEPENDS ${X265_LIB}/libx265.a
    COMMENT "Building libx265"
)


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


#################################aom BEGIN

set(AOM_DIR ${CMAKE_SOURCE_DIR}/external/aom)
set(AOM_BUILD ${AOM_DIR}/build)
# set(AOM_LIB ${AOM_DIR}/build_lib)
set(AOM_INCLUDE ${AOM_DIR}/aom)

# file(MAKE_DIRECTORY ${AOM_LIB})
file(MAKE_DIRECTORY ${AOM_BUILD})

set(PKG_CONFIG_NAME aom) 
# add_subdirectory(
#   ${AOM_DIR} ${AOM_BUILD}
# )

add_custom_command(
    OUTPUT ${AOM_BUILD}/libaom.a    
    COMMAND cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="${FFMPEG_BUILD}" -DENABLE_TESTS=OFF -DENABLE_NASM=on ${AOM_DIR}
    COMMAND make
    WORKING_DIRECTORY ${AOM_BUILD}
    # COMMAND make install
    COMMENT "Compiling libxaom"
)

add_custom_target(
    LIBAOM ALL
    DEPENDS ${AOM_BUILD}/libaom.a
    COMMENT "Building libaom"
)

include_directories(${AOM_INCLUDE})
link_directories(${AOM_BUILD})

#################################aom END




#################################libsvtav1  BEGIN

set(LIBSVTAV1_DIR ${CMAKE_SOURCE_DIR}/external/libsvtav1 )
set(LIBSVTAV1_BUILD ${LIBSVTAV1_DIR}/build)
# set(AOM_LIB ${AOM_DIR}/build_lib)
set(LIBSVTAV1_INCLUDE ${LIBSVTAV1_DIR}/Source/API)
set(LIBSVTAV1_LIB ${LIBSVTAV1_DIR}/Bin/Release)
# file(MAKE_DIRECTORY ${AOM_LIB})
file(MAKE_DIRECTORY ${LIBSVTAV1_BUILD})

option (BUILD_SHARED_LIBS "Build shared libraries" ON)
set(BUILD_SHARED_LIBS OFF) 

 set(PKG_CONFIG_NAME SvtAv1Enc)
# add_subdirectory(
#  ${LIBSVTAV1_DIR} ${LIBSVTAV1_BUILD}
# )
add_custom_command(
    OUTPUT ${LIBSVTAV1_LIB}/libSvtAv1Enc.a    
    COMMAND cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="${FFMPEG_BUILD}" -DCMAKE_BUILD_TYPE=Release -DBUILD_DEC=OFF -DBUILD_SHARED_LIBS=OFF ..
    #cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$HOME/ffmpeg_build" -DCMAKE_BUILD_TYPE=Release -DBUILD_DEC=OFF -DBUILD_SHARED_LIBS=OFF ..
    COMMAND make
    WORKING_DIRECTORY ${LIBSVTAV1_BUILD}
    COMMENT "Compiling libsvtav1"
)

add_custom_target(
    LIBSVTAV1 ALL
    DEPENDS ${LIBSVTAV1_LIB}/libSvtAv1Enc.a
    COMMENT "Building libsvtav1"
)
include_directories(${LIBSVTAV1_INCLUDE})
link_directories(${LIBSVTAV1_DIR}/Bin/Release)

#################################libsvtav1  END


#################################libdav1d  BEGIN

set(LIBDAV1D_DIR ${CMAKE_SOURCE_DIR}/external/dav1d )
set(LIBDAV1D_BUILD ${LIBDAV1D_DIR}/build_lib)

set(LIBDAV1D_LIB ${LIBDAV1D_BUILD}/lib)
set(LIBDAV1D_INCLUDE ${LIBDAV1D_LIB}/include)

file(MAKE_DIRECTORY ${LIBDAV1D_BUILD})
file(MAKE_DIRECTORY ${LIBDAV1D_LIB})


 


add_custom_command(
    OUTPUT ${LIBDAV1D_LIB}/libdav1d.a    
    COMMAND meson setup -Denable_tools=false -Denable_tests=false --default-library=static ${LIBDAV1D_DIR} --prefix ${LIBDAV1D_LIB} --libdir=${LIBDAV1D_LIB}    
    WORKING_DIRECTORY ${LIBDAV1D_BUILD}
    COMMAND ninja
    COMMAND ninja install
    COMMENT "Compiling libdav1d"
)



add_custom_target(
    LIBDAV1D ALL
    DEPENDS ${LIBDAV1D_LIB}/libdav1d.a
    COMMENT "Building libdav1d"
)

include_directories(${LIBDAV1D_INCLUDE})
link_directories(${LIBDAV1D_LIB})

#################################libdav1d  END


#################################libvmaf BEGIN
set(LIBVMAF_DIR ${CMAKE_SOURCE_DIR}/external/vmaf/libvmaf )
set(LIBVMAF_BUILD ${LIBVMAF_DIR}/build_lib)


set(LIBVMAF_BIN ${LIBVMAF_BUILD}/bin)
set(LIBVMAF_LIB ${LIBVMAF_BUILD}/lib)
set(LIBVMAF_INCLUDE ${LIBVMAF_DIR}/include)

file(MAKE_DIRECTORY ${LIBVMAF_BUILD})
file(MAKE_DIRECTORY ${LIBVMAF_LIB})
file(MAKE_DIRECTORY ${LIBVMAF_BIN})


 


add_custom_command (
    OUTPUT ${LIBVMAF_LIB}/libvmaf.a    
    COMMAND meson setup -Denable_tests=false -Denable_docs=false  --buildtype=release --default-library=static ${LIBVMAF_DIR} --prefix ${LIBVMAF_LIB} --bindir=${LIBVMAF_BIN} --libdir=${LIBVMAF_LIB}  
    WORKING_DIRECTORY ${LIBVMAF_BUILD}
    COMMAND ninja
    COMMAND ninja install
    COMMENT "Compiling libvmaf"
)



add_custom_target(
    LIBVMAF ALL
    DEPENDS ${LIBVMAF_LIB}/libvmaf.a
    COMMENT "Building libvmaf"
)

include_directories(${LIBVMAF_INCLUDE})
link_directories(${LIBVMAF_LIB})
#################################libvmaf END

#################################ffmpeg BEGIN





#################################X264 FILES COPY BEGIN
add_custom_target(COPY_X264LIB ALL
    WORKING_DIRECTORY ${X264_LIB}
    COMMAND /bin/bash -c "cp -r ./*.a ./*.la ./*.so ./*.so.* ./pkgconfig ${FFMPEG_BUILD}/lib " 2>/dev/null || true    
    COMMENT "Copy lib files X264"
)

add_custom_target(COPY_X264INCLUDE ALL
    WORKING_DIRECTORY ${X264_INCLUDE}
    COMMAND /bin/bash -c "cp -r ${X264_INCLUDE} ${FFMPEG_BUILD} " 2>/dev/null || true    
    COMMENT "Copy header files X264"
)
#################################X264 FILES COPY END


#################################X265 FILES COPY BEGIN


add_custom_target(COPY_X265LIB ALL
    WORKING_DIRECTORY ${X265_LIB}
    COMMAND /bin/bash -c "cp -r ./*.a ./*.la ./*.so ./*.so.* ./pkgconfig ${FFMPEG_BUILD}/lib " 2>/dev/null || true    
    COMMAND /bin/bash -c "cp -r ./*.pc  ${FFMPEG_BUILD}/lib/pkgconfig " 2>/dev/null || true    
    COMMENT "Copy lib files X265"
)

add_custom_target(COPY_X265INCLUDE ALL
    WORKING_DIRECTORY ${X265_INCLUDE}
    COMMAND /bin/bash -c "cp -r ${X265_INCLUDE}/*.h ${FFMPEG_BUILD}/include " 2>/dev/null || true    
    COMMENT "Copy header files X265"
)

#################################X265 FILES COPY END


#################################AOM FILES COPY BEGIN
add_custom_target(COPY_AOMLIB ALL
    WORKING_DIRECTORY ${AOM_BUILD}
    COMMAND /bin/bash -c "cp -r ./*.a ./*.la ./*.so ./*.so.* ./pkgconfig ${FFMPEG_BUILD}/lib " 2>/dev/null || true    
    COMMAND /bin/bash -c "cp -r ./*.pc  ${FFMPEG_BUILD}/lib/pkgconfig " 2>/dev/null || true    
    COMMENT "Copy lib files AOM"
)

add_custom_target(COPY_AOMINCLUDE ALL
    WORKING_DIRECTORY ${AOM_INCLUDE}
    COMMAND /bin/bash -c "cp -r ${AOM_INCLUDE} ${FFMPEG_BUILD}/include " 2>/dev/null || true    
    COMMENT "Copy header files AOM"
)

#################################AOM FILES COPY END


#################################LIBVPX FILES COPY BEGIN

add_custom_target(COPY_LIBVPX ALL
    WORKING_DIRECTORY ${LIBVPX_LIB}
    COMMAND /bin/bash -c "cp -r ./*.a ./*.la ./*.so ./*.so.* ./pkgconfig ${FFMPEG_BUILD}/lib " 2>/dev/null || true    
    COMMAND /bin/bash -c "cp -r ./*.pc  ${FFMPEG_BUILD}/lib/pkgconfig " 2>/dev/null || true    
    COMMENT "Copy lib files LIBVPX"
)

add_custom_target(COPY_VPXINCLUDE ALL
    WORKING_DIRECTORY ${LIBVPX_INCLUDE}
    COMMAND /bin/bash -c "cp -r ${LIBVPX_INCLUDE} ${FFMPEG_BUILD} " 2>/dev/null || true    
    COMMENT "Copy header files LIBVPX"
)

#################################LIBVPX FILES COPY END

#################################FDKAAC FILES COPY BEGIN

add_custom_target(COPY_LIBFDKAAC ALL
    WORKING_DIRECTORY ${LIBFDKAAC_LIB}
    COMMAND /bin/bash -c "cp -r ./*.a ./*.la ./*.so ./*.so.* ./pkgconfig ${FFMPEG_BUILD}/lib " 2>/dev/null || true    
    COMMAND /bin/bash -c "cp -r ./*.pc  ${FFMPEG_BUILD}/lib/pkgconfig " 2>/dev/null || true    
    COMMENT "Copy lib files LIBFDKAAC"
)

add_custom_target(COPY_FDKAACINCLUDE ALL
    WORKING_DIRECTORY ${LIBFDKAAC_INCLUDE}
    COMMAND /bin/bash -c "cp -r ${LIBFDKAAC_INCLUDE} ${FFMPEG_BUILD} " 2>/dev/null || true    
    COMMENT "Copy header files FDKAAC"
)

#################################FDKAAC FILES COPY END

#################################OPUS FILES COPY BEGIN

add_custom_target(COPY_LIBOPUS ALL
    WORKING_DIRECTORY ${LIBOPUS_LIB}
    COMMAND /bin/bash -c "cp -r ./*.a ./*.la ./*.so ./*.so.* ./pkgconfig ${FFMPEG_BUILD}/lib " 2>/dev/null || true    
    COMMAND /bin/bash -c "cp -r ./*.pc  ${FFMPEG_BUILD}/lib/pkgconfig " 2>/dev/null || true    
    COMMENT "Copy lib files LIBFDKAAC"
)

add_custom_target(COPY_OPUSINCLUDE ALL
    WORKING_DIRECTORY ${LIBOPUS_INCLUDE}
    COMMAND /bin/bash -c "cp -r ${LIBOPUS_INCLUDE} ${FFMPEG_BUILD} " 2>/dev/null || true    
    COMMENT "Copy header files OPUS"
)

#################################OPUS FILES COPY END


#################################BSVTAV1 FILES COPY BEGIN

add_custom_target(COPY_LIBSVTAV1 ALL
    WORKING_DIRECTORY ${LIBSVTAV1_LIB}
    COMMAND /bin/bash -c "cp -r ./*.a ./*.la ./*.so ./*.so.* ./pkgconfig ${FFMPEG_BUILD}/lib " 2>/dev/null || true    
    COMMAND /bin/bash -c "cp -r ./*.pc  ${FFMPEG_BUILD}/lib/pkgconfig " 2>/dev/null || true    
    COMMENT "Copy lib files BSVTAV1"
)



add_custom_target(COPY_LIBSVTAV1_BUILD ALL
    WORKING_DIRECTORY ${LIBSVTAV1_BUILD}
    COMMAND /bin/bash -c "cp -r ./*.a ./*.la ./*.so ./*.so.* ./pkgconfig ${FFMPEG_BUILD}/lib " 2>/dev/null || true    
    COMMAND /bin/bash -c "cp -r ./*.pc  ${FFMPEG_BUILD}/lib/pkgconfig " 2>/dev/null || true    
    COMMENT "Copy lib files BSVTAV1"
)



add_custom_target(COPY_CP_BUILD ALL
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build
    COMMAND /bin/bash -c "cp ./*.pc ${FFMPEG_BUILD}/lib/pkgconfig " 2>/dev/null || true
    COMMENT "Copy pc files COPY_CP_BUILD"
)

add_custom_target(COPY_BSVTAV1INCLUDE ALL
    WORKING_DIRECTORY ${LIBSVTAV1_INCLUDE}
    COMMAND /bin/bash -c "cp -r ${LIBSVTAV1_INCLUDE} ${FFMPEG_BUILD}/include/svt-av1" 2>/dev/null || true    
    COMMENT "Copy header files BSVTAV1 ${LIBSVTAV1_INCLUDE} ${FFMPEG_BUILD}/include/svt-av1"
)

#################################BSVTAV1 FILES COPY END

#################################DAV1D FILES COPY BEGIN

add_custom_target(COPY_LIBDAV1D ALL
    WORKING_DIRECTORY ${LIBDAV1D_LIB}
    COMMAND /bin/bash -c "cp -r ./*.a ./*.la ./*.so ./*.so.* ./pkgconfig ${FFMPEG_BUILD}/lib " 2>/dev/null || true    
    COMMAND /bin/bash -c "cp -r ./*.pc  ${FFMPEG_BUILD}/lib/pkgconfig " 2>/dev/null || true    
    COMMENT "Copy lib files LIBFDKAAC"
)

add_custom_target(COPY_DAV1DINCLUDE ALL
    WORKING_DIRECTORY ${LIBDAV1D_INCLUDE}
    COMMAND /bin/bash -c "cp -r ${LIBDAV1D_INCLUDE} ${FFMPEG_BUILD} " 2>/dev/null || true    
    COMMENT "Copy header files DAV1D"
)

#################################DAV1D FILES COPY END

#################################VMAF FILES COPY BEGIN

add_custom_target(COPY_LIBVMAF ALL
    WORKING_DIRECTORY ${LIBVMAF_LIB}
    COMMAND /bin/bash -c "cp -r ./*.a ./*.la ./*.so ./*.so.* ./pkgconfig ${FFMPEG_BUILD}/lib " 2>/dev/null || true    
    COMMAND /bin/bash -c "cp -r ./*.pc  ${FFMPEG_BUILD}/lib/pkgconfig " 2>/dev/null || true    
    COMMENT "Copy lib files LIBFDKAAC"
)

add_custom_target(COPY_VMAFINCLUDE ALL
    WORKING_DIRECTORY ${LIBVMAF_INCLUDE}
    COMMAND /bin/bash -c "cp -r ${LIBVMAF_INCLUDE} ${FFMPEG_BUILD} " 2>/dev/null || true    
    COMMENT "Copy header files VMAF"
)

#################################VMAF FILES COPY END

# add_custom_target(COPY_X265LIB ALL
#     WORKING_DIRECTORY ${X265_LIB}
#     COMMAND /bin/bash -c "cp -r ./*.a ./*.so ./*.so.* ./pkgconfig ${FFMPEG_BUILD}/lib " 2>/dev/null || true    
#     COMMAND /bin/bash -c "cp -r ./*.pc  ${FFMPEG_BUILD}/lib/pkgconfig " 2>/dev/null || true    
#     COMMENT "Copy lib filex X265"
# )





# ffmpeg path
set(FFMPEG_INCLUDE_DIR external/ffmpeg)
set(FFMPEG_DIR external/ffmpeg)
set(FFMPEG_LIB_DIR external/ffmpeg/lib)

# add_custom_target(
#     FFmpeg
#     COMMAND ./configure --prefix=$(pwd)/install --enable-shared --enable-gpl --enable-libx264 --enable-libmp3lame --enable-libvorbis --enable-openssl --enable-version3
#     COMMAND make -j$(nproc)
#     WORKING_DIRECTORY ../external/ffmpeg
#     COMMENT "Compiling FFmpeg"
# )


# ./update_pc.sh ~/repo_emcc/ffmpeg2/ffmpeg_build/build/ffmpeg_build/lib/pkgconfig /home/glider/repo_emcc/ffmpeg2/ffmpeg_build/build/ffmpeg_build/
add_custom_target(update_pc ALL
    COMMAND /bin/bash ${CMAKE_SOURCE_DIR}/update_pc.sh ${FFMPEG_BUILD}/lib/pkgconfig ${FFMPEG_BUILD}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running update_pc.sh"
)

# add_custom_target(configure_ffmpeg ALL
#     COMMAND ${CMAKE_COMMAND} -E env
#             PKG_CONFIG_PATH="${FFMPEG_BUILD}/lib/pkgconfig"
#             ./configure
#             --prefix="${FFMPEG_BUILD}"
#             --pkg-config-flags="--static"
#             --extra-cflags="-I${FFMPEG_BUILD}/include"
#             --extra-ldflags="-L${FFMPEG_BUILD}/lib"
#             --extra-libs="-lpthread -lm"
#             --ld="g++"
#             --enable-gpl
#             --enable-gnutls
#             --enable-libaom
#             --enable-libass
#             --enable-libfdk-aac
#             --enable-libfreetype
#             --enable-libmp3lame
#             --enable-libopus
#             --enable-libsvtav1
#             --enable-libdav1d
#             --enable-libvorbis
#             --enable-libvpx
#             --enable-libx264
#             --enable-libx265
#             --enable-nonfree
#     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external/ffmpeg"
#     COMMENT "Running FFmpeg configure script"
# )
# add_custom_target(configure_ffmpeg ALL
#     COMMAND ${CMAKE_COMMAND} -E env
#             PKG_CONFIG_PATH="${FFMPEG_BUILD}/lib/pkgconfig"
#             /bin/bash -c "./configure \
#             --prefix=${FFMPEG_BUILD} \
#             --pkg-config-flags=--static \
#             --extra-cflags=-I${FFMPEG_BUILD}/include \
#             --extra-ldflags=-L${FFMPEG_BUILD}/lib \
#             --extra-libs=-lpthread -lm \
#             --ld=g++ \
#             --enable-gpl \
#             --enable-gnutls \
#             --enable-libaom \
#             --enable-libass \
#             --enable-libfdk-aac \
#             --enable-libfreetype \
#             --enable-libmp3lame \
#             --enable-libopus \
#             --enable-libsvtav1 \
#             --enable-libdav1d \
#             --enable-libvorbis \
#             --enable-libvpx \
#             --enable-libx264 \
#             --enable-libx265 \
#             --enable-nonfree"
#     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external/ffmpeg"
#     COMMENT "Running FFmpeg configure script"
# )

# add_custom_target(configure_ffmpeg ALL
#     COMMAND ${CMAKE_COMMAND} -E env
#             PKG_CONFIG_PATH="${FFMPEG_BUILD}/lib/pkgconfig"
#             /bin/bash -c "./configure \
#             --prefix=${FFMPEG_BUILD} \
#             --pkg-config-flags=--static \
#             --extra-cflags=-I${FFMPEG_BUILD}/include \
#             --extra-ldflags=-L${FFMPEG_BUILD}/lib \
#             --extra-libs=-lpthread \
#             --ld=g++ \
#             --enable-gpl \
#             --enable-gnutls \
#             --enable-libaom \
#             --enable-libass \
#             --enable-libfdk-aac \
#             --enable-libfreetype \
#             --enable-libmp3lame \
#             --enable-libopus \
#             --enable-libsvtav1 \
#             --enable-libdav1d \
#             --enable-libvorbis \
#             --enable-libvpx \
#             --enable-libx264 \
#             --enable-libx265 \
#             --enable-nonfree"
#     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external/ffmpeg"
#     COMMENT "Running FFmpeg configure script"
# )
# add_custom_target(configure_ffmpeg ALL
#      COMMAND ${CMAKE_COMMAND} -E env
#              PKG_CONFIG_PATH="${FFMPEG_BUILD}/lib/pkgconfig"
#              /bin/bash -c "./configure \
#              --prefix=${FFMPEG_BUILD} \
#              --pkg-config-flags=--static \
#              --extra-cflags=-I${FFMPEG_BUILD}/include \
#              --extra-ldflags=-L${FFMPEG_BUILD}/lib \
#              --extra-libs=-lpthread \
#              --ld=g++ \
#              --enable-gpl \
#              --enable-gnutls \
#              --enable-libaom \#  # This line has been commented out as per request
#              --enable-libass \
#              --enable-libfdk-aac \
#              --enable-libfreetype \
#              --enable-libmp3lame \
#              --enable-libopus \
#              --enable-libsvtav1 \
#              --enable-libdav1d \
#              --enable-libvorbis \
#              --enable-libvpx \
#              --enable-libx264 \
#              --enable-libx265 \
#              --enable-nonfree"
#      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external/ffmpeg"
#      COMMENT "Running FFmpeg configure script"
#  )

add_custom_target(configure_ffmpeg ALL
    COMMAND ${CMAKE_COMMAND} -E env
            PKG_CONFIG_PATH="${FFMPEG_BUILD}/lib/pkgconfig" 
            /bin/bash -c "./configure \
            --prefix=${FFMPEG_BUILD} \
            --pkg-config-flags='--static' \
            --extra-cflags=-I${FFMPEG_BUILD}/include \
            --extra-ldflags=-L${FFMPEG_BUILD}/lib \
            --extra-libs='-lpthread -lm' \
            --ld='g++' \
            --enable-gpl \
            --enable-gnutls \
            --enable-libaom \
            --enable-libass \
            --enable-libfdk-aac \
            --enable-libfreetype \
            --enable-libmp3lame \
            --enable-libopus \
            --enable-libsvtav1 \
            --enable-libdav1d \
            --enable-libvorbis \
            --enable-libvpx \
            --enable-libx264 \
            --enable-nonfree"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external/ffmpeg"
    COMMAND make
    COMMAND make install
    COMMENT "Running FFmpeg configure script"
)
# add_custom_target(configure_ffmpeg ALL
#     COMMAND ${CMAKE_COMMAND} 
#             -E env
#             PKG_CONFIG_PATH="/home/glider/ffmpeg1/lib/pkgconfig" 
#             /bin/bash -c "./configure \
#             --prefix='/home/glider/repo_emcc/ffmpeg/ffmpeg_build/build/ffmpeg_build' \
#             --pkg-config-flags='--static' \
#             --extra-cflags='-I/home/glider/repo_emcc/ffmpeg/ffmpeg_build/build/ffmpeg_build/include' \
#             --extra-ldflags='-L/home/glider/repo_emcc/ffmpeg/ffmpeg_build/build/ffmpeg_build/lib' \
#             --extra-libs='-lpthread -lm' \
#             --ld='g++' \
#             --enable-gpl \
#             --enable-gnutls \
#             --enable-libaom \
#             --enable-libass \
#             --enable-libfdk-aac \
#             --enable-libfreetype \
#             --enable-libmp3lame \
#             --enable-libopus \
#             --enable-libsvtav1 \
#             --enable-libdav1d \
#             --enable-libvorbis \
#             --enable-libvpx \
#             --enable-libx264 \
#             --enable-libx265 \
#             --enable-nonfree"
#     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/external/ffmpeg"
#     COMMENT "Running FFmpeg configure script"
# )

#################################ffmpeg END


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
