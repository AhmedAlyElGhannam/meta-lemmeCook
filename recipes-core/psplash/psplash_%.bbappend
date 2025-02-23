FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:remove = "file://psplash.png"
SRC_URI += "file://logo.png \
	    file://psplash.service \
"

inherit features_check
inherit systemd

SYSTEMD_SERVICE:${PN} = "splash.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

do_install:append(){
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 splash.service ${D}${systemd_system_unitdir}
}

FILES:${PN}:append = " ${systemd_system_unitdir}/splash.service"

