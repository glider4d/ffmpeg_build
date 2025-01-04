sudo apt update
sudo apt install -y build-essential git pkg-config yasm cmake
sudo apt install -y libx264-dev libx265-dev libvpx-dev libfreetype6-dev libass-dev libfdk-aac-dev libopus-dev libvorbis-dev libtheora-dev libmp3lame-dev libopus-dev libspeex-dev libsdl2-dev

sudo apt-get install nasm

git subtree add --prefix=external/x264 https://code.videolan.org/videolan/x264.git master --squash
git subtree add --prefix=external/x265 https://bitbucket.org/multicoreware/x265_git.git master --squash
                                         

git subtree add --prefix=external/libvpx https://chromium.googlesource.com/webm/libvpx.git master --squash





cd ~/ffmpeg_sources && \
git -C libvpx pull 2> /dev/null || git clone --depth 1 https://chromium.googlesource.com/webm/libvpx.git && \
cd libvpx && \
PATH="$HOME/bin:$PATH" ./configure --prefix="$HOME/ffmpeg_build" --disable-examples --disable-unit-tests --enable-vp9-highbitdepth --as=yasm && \
PATH="$HOME/bin:$PATH" make && \
make install
