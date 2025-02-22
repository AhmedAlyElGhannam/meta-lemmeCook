SUMMARY = "This is a simple recipe to fetch and build my qt app and attach a systemd service for it"
MAINTAINER = "Ahmed Aly <ahmedaly.g112@gmail.com>"
LICENSE = "CLOSED"
SRC_URI = "git://github.com/AhmedAlyElGhannam/Simple-Infotainment-System-Using-Qt6-and-Yocto-on-Raspberry-Pi-3B.git;branch=main"

inherit pkgconfig qt6-cmake cmake

RDEPENDS:${PN} += "qtwayland qtmultimedia qtbase qtdeclarative qtlocation qtpositioning qtquick3d"

S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

APP_NAME="InfotainmentSystem-UI"

do_configure(){
    cmake -S ${S} -B ${B}
}

do_compile(){
    cmake --build ${B}
}

do_install(){
    install -d ${D}/${bindir}
    install -m 0777 ${B}/${APP_NAME} ${D}/${bindir}
}
