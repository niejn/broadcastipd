# Maintainer: Julian Sanin <sanin89julian@gmail.com>
pkgname=broadcastipd
pkgver=0.1
pkgrel=1
arch=('armv6h' 'armv7h')
license=('MIT')
source=('broadcastipd.c')

build() {
  gcc "${pkgname}.c" -o "${pkgname}"
}

package() {
  install -o root -g root -Dm755 "${srcdir}/${pkgname}" "${pkgdir}/usr/local/bin/${pkgname}"
}

