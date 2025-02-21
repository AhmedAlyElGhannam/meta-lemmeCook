SUMMARY = "Math library"
DESCRIPTION = "A shared math library"
MAINTAINER = "Ahmed Aly <ahmedaly.g112@gmail.com>"

LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-or-later;md5=fed54355545ffd980b814dab4a3b312c"

SRC_URI = "file://libmath.c file://libmath.h"

PROVIDES = "libmath"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} -fPIC -shared ${WORKDIR}/libmath.c -o ${B}/libmath.so.0.1
    ln -sf libmath.so.0.1 ${B}/libmath.so
}

do_install() {
    install -d ${D}${libdir}
    install -m 0755 ${B}/libmath.so.0.1 ${D}${libdir}/
    ln -sf libmath.so.0.1 ${D}${libdir}/libmath.so

    install -d ${D}${includedir}
    install -m 0644 ${WORKDIR}/libmath.h ${D}${includedir}/
}

FILES:${PN} = "${libdir}/libmath.so.*"
FILES:${PN}-dev = "${libdir}/libmath.so ${includedir}/libmath.h"
