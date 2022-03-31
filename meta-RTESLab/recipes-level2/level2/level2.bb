DESCRIPTION = "Applicazione pthread - livello 2"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://level2.c"

S = "${WORKDIR}"

do_compile() {
	${CC} level2.c ${LDFLAGS} -o level2 -lpthread
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 level2 ${D}${bindir}
}
