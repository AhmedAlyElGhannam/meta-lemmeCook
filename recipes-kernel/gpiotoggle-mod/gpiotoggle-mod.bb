SUMMARY = "Example of how to build an external Linux kernel module"
DESCRIPTION = "${SUMMARY}"
LICENSE = "CLOSED"

inherit module

# change the .c file name
SRC_URI = "file://Makefile \
           file://gpiotoggle.c \
           file://BCM2837.h \
          "

S = "${WORKDIR}"

# kernel module is located under ./lib/modules/5.15.92-v8/extra/gpiotoggle.ko.xz
RPROVIDES:${PN} += "kernel-module-gpiotoggle"
