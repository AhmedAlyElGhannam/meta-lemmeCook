SUMMARY = "This is a simple recipe to fetch and build my qt app and attach a systemd service for it"
MAINTAINER = "Ahmed Aly <ahmedaly.g112@gmail.com>"
LICENSE = "CLOSED"
SRC_URI = "git://github.com/AhmedAlyElGhannam/Simple-Infotainment-System-Using-Qt6-and-Yocto-on-Raspberry-Pi-3B.git;branch=main;protocol=https"

# since I do not want to use a specific commit I will set it to AUTOREV
SRCREV = "${AUTOREV}"

inherit qt6-cmake qt6-paths

DEPENDS += " \
    qtbase \
    qtdeclarative-native \
    qtmultimedia \
    qtwayland \
    qtlocation \
    qtpositioning \
    qtquick3d \
    qtquicktimeline \
    qttools \
    qtapplicationmanager-native \
"

RDEPENDS:${PN} += " \
    qtbase \
    qtdeclarative \
    qtmultimedia \
    qtwayland \
    qtlocation \
    qtpositioning \
    qtquick3d \
    qtquicktimeline \
    qttools \
    qtapplicationmanager \
"

EXTRA_OECMAKE = " \
    -DCMAKE_FIND_DEBUG_MODE=TRUE \
    -DQT_HOST_PATH=${STAGING_DIR_NATIVE}${prefix_native} \
"

S = "${WORKDIR}/git"
#B = "${WORKDIR}/buildd"

APP_NAME="InfotainmentSystem-UI"


