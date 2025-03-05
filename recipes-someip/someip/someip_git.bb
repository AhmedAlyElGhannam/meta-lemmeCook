# Recipe created by recipetool
LICENSE = "CLOSED"

SRC_URI = "git://github.com/COVESA/vsomeip.git;protocol=https;branch=release_3.5.4"

SRCREV = "${AUTOREV}"

DEPENDS = "boost dlt-daemon googletest"

S = "${WORKDIR}/git"

inherit cmake pkgconfig

EXTRA_OECMAKE = "-DINSTALL_LIB_DIR:PATH=${baselib} \
                 -DINSTALL_CMAKE_DIR:PATH=${baselib}/cmake/vsomeip3 \
                "

do_compile:append() {
    cmake_runcmake_build --target examples
}

do_install:append() {
    install -d ${D}${sysconfdir}/vsomeip
    mv ${D}/usr/etc/vsomeip/* ${D}${sysconfdir}/vsomeip/ 2>/dev/null || true
    rmdir ${D}/usr/etc/vsomeip ${D}/usr/etc 2>/dev/null || true
}
