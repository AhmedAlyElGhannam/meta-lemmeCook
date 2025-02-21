SUMMARY = "A based wayland image MAIDO INU HEAVEN"
MAINTAINER = "Ahmed Aly <ahmedaly.g112@gmail.com>"

LICENSE = "CLOSED"

inherit core-image

MACHINE_FEATURES = " alsa bluetooth touchscreen wifi "

DISTRO_FEATURES = " alsa bluetooth ipv4 ipv6 opengl systemd systemd-resolved wayland " 

# have to set PSPLASH variable to the image I want to show
IMAGE_FEATURES = " hwcodecs package-management splash ssh-server-openssh tools-debug tools-sdk tools-testapps weston"

IMAGE_INSTALL:append = " qtwayland bash connman-client"

IMAGE_ROOTFS_EXTRA_SPACE = "5242880"

