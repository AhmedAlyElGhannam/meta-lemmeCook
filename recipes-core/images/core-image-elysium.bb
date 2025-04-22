SUMMARY = "A based wayland image MAIDO INU HEAVEN"
MAINTAINER = "Ahmed Aly <ahmedaly.g112@gmail.com>"

LICENSE = "CLOSED"

inherit core-image

MACHINE_FEATURES:append = " alsa bluetooth touchscreen wifi"

DISTRO_FEATURES:append = " alsa bluetooth ipv4 ipv6 opengl systemd systemd-resolved wayland"

IMAGE_FEATURES += " \
    hwcodecs \
    ssh-server-openssh \
    tools-debug \
    tools-sdk \
    tools-testapps \
    weston \
    splash \
"

IMAGE_INSTALL:append = " \
    bash \
    connman-client \
    htop \
    vim \
    nano \
    rsync \
    make \
    fastfetch \
    qt6app \
    weston-init \
    freerdp \ 
    newuser \
    someip \
    systemd \
    gpiotoggle-mod \
    gpioled-mod \
    update-alternatives \
"



#     qt6app     splash cmake     openjdk-21-jre 
# update-alternatives (for jdk21 rpm)

IMAGE_ROOTFS_EXTRA_SPACE = "5242880"

# adding support for package management to install JavaFX successfully
PACKAGE_CLASSES ?= "package_rpm"
EXTRA_IMAGE_FEATURES += "package-management"
