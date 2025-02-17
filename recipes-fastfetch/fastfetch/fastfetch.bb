DESCRIPTION = "A fastfetch recipe for maximum drip"
MAINTAINER = "Ahmed Aly <ahmedaly.g112@gmail.com>"

LICENSE = "CLOSED"

SRC_URI = "git://github.com/fastfetch-cli/fastfetch;protocol=https;branch=master"

SRCREV = "4a1037d2b6f5c8ece2a08b696f03272adb404d9b"

PN = "fastfetch"
PV = "1.0.0"
PR = "r0"

S = "${WORKDIR}/git"
#B = "${WORKDIR}/build"

inherit cmake

# will leave default do_configure && do_compile and will not override them

do_install(){
    install -d ${D}${bindir}
    install -m 0755 ${B}/${PN} ${D}${bindir}
}
