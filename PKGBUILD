# Maintainer: Julian Sanin <sanin89julian@gmail.com>
pkgname=broadcastipd
pkgver=1.0.0
pkgrel=1
arch=('armv6h' 'armv7h')
license=('MIT')
install="${pkgname}.install"
source=('LICENSE'
        "${pkgname}.c"
        "${pkgname}.service"
        "${pkgname}.conf")
md5sums=('292e0d6c7f6827b97f4defdd9e99a7a3'
         'bfd45e92bf5760c616f93e9a30567a3d'
         '20b16a9194f402f99613b69de67a16e2'
         '847c5a101dd3f9a7e13afa7d55fd0295')

build() {
  gcc "${pkgname}.c" -o "${pkgname}"
}

package() {
  install -o root -g root -Dm755 "${srcdir}/${pkgname}" "${pkgdir}/usr/local/bin/${pkgname}"
  install -o root -g root -Dm644 "${pkgname}.service" "${pkgdir}/etc/systemd/system/${pkgname}.service"
  install -o root -g root -Dm644 "${pkgname}.conf" "${pkgdir}/etc/${pkgname}.conf"
  install -o root -g root -Dm644 LICENSE "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE"
}
