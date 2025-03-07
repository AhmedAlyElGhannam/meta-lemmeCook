SUMMARY = "This is a simple recipe to fetch and build my qt app and attach a systemd service for it"
MAINTAINER = "Ahmed Aly <ahmedaly.g112@gmail.com>"
LICENSE = "CLOSED"
SRC_URI:append = " file://qt6app.service"

APP_NAME="InfotainmentSystem-UI"

SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE:${PN} = "qt6app.service"

do_install:append() {
	install -d ${D}${systemd_unitdir}/system
	
}
