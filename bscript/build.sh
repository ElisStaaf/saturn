#/bin/bash


# This is a generic package build script 

#Package Name
PKGNAME=

#Version Number
VERSION=0.0.0

#Package Archive
PKGAR=$PKGNAME-$VERSION.tar.gz

#Package hash
PKGHASH=$PKGAR.sha512

#Web location
URL_DOMAIN=
URL_AR=$URL_DOMAIN/$PKGAR
URL_HASH=$URL_DOMAIN/$PKGHASH


SRCDIR= /usr/src
ARDIR= /usr/src/ar
BUILDDIR=/usr/src/build
SCRIPTDIR=/usr/src/script
INSTALLDIR= /usr


SRC= $SRCDIR/$PKGNAME
ARCHIVE= $ARDIR/$PKGAR
BUILD= $BUILDDIR/$PKGNAME-$VERSION

download() {
	if [ $(id -u) -ne 0 ]; then
		echo "[ERROR]: This must be run as root or package maintainer"
	fi	

	curl -L -f -o $ARCHIVE $URL_AR 

	curl -L -f -o /tmp/$PKGHASH $URL_HASH

}

setup() {

	if [ $(id -u) -ne 0 ]; then
		echo "[ERROR]: This must be run as root or package maintainer."
	fi	

	if [ ! -f $ARCHIVE ]; then
		echo "[ERROR]: Could not find $PKGAR, exiting..."
		exit -1
	fi

	mkdir -p $SRC

	if [ sha512sum -c --status $ARDIR/$PKGHASH ]; then
		echo "[ERROR]: Invalid hash for package: $PKGNAME, $PKGHASH."
		exit -1
	fi

	tar -xf $ARDIR/$PKGAR -C $SRCDIR/$PKGNAME


}

build() {
	if [ $(id -u) -ne 0 ]; then
		echo "[ERROR]: bscript/build.sh must be run as root or package maintainer."
	fi	

	mkdir -p $BUILD

	cd $BUILD

	$SRC/configure --prefix $INSTALLDIR

	make -j $(nproc --ignore=1)


}

install() {
	if [ $(id -u) -ne 0 ]; then
		echo "[ERROR]: bscript/build.sh must be run as root or package maintainer."
	fi	
	cd $BUILD
	make install

	touch /tmp/$PKGNAME
	echo "1" > /tmp/$PKGNAME
}


