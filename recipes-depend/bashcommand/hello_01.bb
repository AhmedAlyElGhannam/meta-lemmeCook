DESCRIPTION = "A calculator application using libmath"
MAINTAINER = "Ahmed Aly <ahmedaly.g112@gmail.com>"

LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-or-later;md5=fed54355545ffd980b814dab4a3b312c"

SRC_URI = "file://hello"

RDEPENDS:${PN} = " bash"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/hello ${D}${bindir}/
}

FILES_${PN} = "${bindir}/hello"