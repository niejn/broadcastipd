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
  install -o root -g root -Dm644 "${pkgname}.service" "${pkgdir}/etc/systemd/system/${pkgname}.service"
  install -o root -g root -Dm644 "${pkgname}.conf" "${pkgdir}/etc/${pkgname}.conf"
}

md5sums=('bfd45e92bf5760c616f93e9a30567a3d'
         '20b16a9194f402f99613b69de67a16e2'
         '56e92f031bf3e72c36ab0ccd7e2b4f88')
