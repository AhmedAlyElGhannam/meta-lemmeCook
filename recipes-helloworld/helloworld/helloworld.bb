SUMMARY = "This is a simple recipe to build a c program using cmake"
MAINTAINER = "Ahmed Aly <ahmedaly.g112@gmail.com>"
LICENSE = "CLOSED"
FILESEXTRAPATHS:prepend := "${THISDIR}/ITI45:"
SRC_URI:append = " file://CMakeLists.txt"
SRC_URI:append = " file://main.c"

S = "${WORKDIR}"
	
inherit pkgconfig cmake

#EXTRA_OECMAKE:append

B = "${WORKDIR}/build"

do_configure(){
	cmake -S ${S} -B ${B} 
}

do_compile(){
	cmake --build ${B}
}

do_install(){
	install -d ${D}/${bindir}
	install -m 0777 ${B} ${D}/${bindir}
}



COMPATIBLE_MACHINE = "raspberrypi3-64"
