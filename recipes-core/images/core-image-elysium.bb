SUMMARY = "A based wayland image MAIDO INU HEAVEN"
MAINTAINER = "Ahmed Aly <ahmedaly.g112@gmail.com>"

LICENSE = "CLOSED"

inherit core-image

MACHINE_FEATURES:append = " alsa bluetooth touchscreen wifi"

DISTRO_FEATURES:append = " alsa bluetooth ipv4 ipv6 opengl systemd systemd-resolved wayland"

IMAGE_FEATURES += " \
    hwcodecs \
    package-management \
    splash \
    ssh-server-openssh \
    tools-debug \
    tools-sdk \
    tools-testapps \
    weston \
"

IMAGE_INSTALL:append = " \
    bash \
    connman-client \
    htop \
    vim \
    nano \
    rsync \
    make \
    cmake \
    qt6app \
    fastfetch \
"

# Add extra space to the root filesystem (5 GB, adjust if needed)
IMAGE_ROOTFS_EXTRA_SPACE = "5242880"
