DESCRIPTION = "Applicazione pthread - livello 0"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://level0.c"

S = "${WORKDIR}"

do_compile() {
	${CC} level0.c ${LDFLAGS} -o level0
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 level0 ${D}${bindir}
}
