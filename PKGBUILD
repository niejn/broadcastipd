# Maintainer: Julian Sanin <sanin89julian@gmail.com>
pkgname=broadcastipd
pkgver=0.1
pkgrel=2
arch=('armv6h' 'armv7h')
license=('MIT')
install="${pkgname}.install"
source=("${pkgname}.c"
        "${pkgname}.service"
        "${pkgname}.conf")

build() {
  gcc "${pkgname}.c" -o "${pkgname}"
}

package() {
  install -o root -g root -Dm755 "${srcdir}/${pkgname}" "${pkgdir}/usr/local/bin/${pkgname}"
  install -o root -g root -Dm644 "${pkgname}.service" "${pkgdir}/etc/systemd/system/"
  install -o root -g root -Dm644 "${pkgname}.conf" "${pkgdir}/etc/"
}

