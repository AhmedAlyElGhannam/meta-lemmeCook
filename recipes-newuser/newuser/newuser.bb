LICENSE = "CLOSED"

inherit useradd

# adding bash dependency here
DEPENDS = " bash "

# printf "%q" $(openssl passwd 123)
PASSWD = "\$1\$N12AYgwa\$O/0MImfsfDt13ZoRcenyA."

NEWUSER = "nemesis"

USERADD_PACKAGES = "${PN}"

# added default shell as well
USERADD_PARAM:${PN} += " -d /home/${NEWUSER} -p '${PASSWD}' -s /bin/bash ${NEWUSER}"

do_install() {
	install -d ${D}/home/${NEWUSER}
}

FILES:${PN} += "/home/${NEWUSER}"
