SUMMARY = "Create calculator app"
DESCRIPTION = "A calculator application using libmath"
MAINTAINER = "Ahmed Aly <ahmedaly.g112@gmail.com>"

LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-or-later;md5=fed54355545ffd980b814dab4a3b312c"

SRC_URI = "file://main.c"

DEPENDS = " libmath "
RDEPENDS:{PN} = " libmath "

do_compile() {
    ${CC} ${CFLAGS} ${TARGET_LDFLAGS} -shared -I${STAGING_INCDIR} -L${STAGING_LIBDIR} -lmath ${WORKDIR}/main.c -o ${B}/calculator
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/calculator ${D}${bindir}/
}

FILES_${PN} = "${bindir}/calculator"
