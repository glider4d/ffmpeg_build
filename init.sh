#!/bin/bash


git subtree add --prefix=external/x264 https://code.videolan.org/videolan/x264.git master --squash || echo "Worktree already exists. Skipping."
git subtree add --prefix=external/x265 https://bitbucket.org/multicoreware/x265_git.git master --squash || echo "Worktree already exists. Skipping."
git subtree add --prefix=external/libvpx https://chromium.googlesource.com/webm/libvpx.git main --squash || echo "Worktree already exists. Skipping."
git subtree add --prefix=external/libfdk-aac https://github.com/mstorsjo/fdk-aac.git master --squash || echo "Worktree already exists. Skipping."
git subtree add --prefix=external/libopus https://github.com/xiph/opus.git main --squash || echo "Worktree already exists. Skipping."
git subtree add --prefix=external/aom https://aomedia.googlesource.com/aom.git main --squash || echo "Worktree already exists. Skipping."
git subtree add --prefix=external/libsvtav1 https://gitlab.com/AOMediaCodec/SVT-AV1.git master --squash || echo "Worktree already exists. Skipping."
git subtree add --prefix=external/dav1d https://code.videolan.org/videolan/dav1d.git master --squash || echo "Worktree already exists. Skipping."
git subtree add --prefix=external/vmaf https://github.com/Netflix/vmaf master --squash || echo "Worktree already exists. Skipping."


exit 0