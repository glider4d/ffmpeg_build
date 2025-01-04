sudo apt update
sudo apt install -y build-essential git pkg-config yasm cmake
sudo apt install -y libx264-dev libx265-dev libvpx-dev libfreetype6-dev libass-dev libfdk-aac-dev libopus-dev libvorbis-dev libtheora-dev libmp3lame-dev libopus-dev libspeex-dev libsdl2-dev


git subtree add --prefix=external/x264 https://code.videolan.org/videolan/x264.git master --squash
