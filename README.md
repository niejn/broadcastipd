broadcastipd
============
This package can be used to broadcast a message from the server.
This is accomplished on the server by the means of sending UDP broadcast packets to the entire network.
Clients that wish to get the message from the server should simply execute `netcat -ulvn -p 23456` that listens to the UDP packets.

Required Packages for Building
------------------------------
* base-devel

Building
--------
Make sure you have `base-devel` installed and run `makepkg` to build the package.

Installing
----------
Install the package via `sudo pacman -U broadcastipd*.pkg.tar.xz`

Usage
-----
`broadcastipd <dgram_message> <dgram_port> <dgram_sleep_sec>`

The protocol is quite simple. `broadcastipd` broadcasts a datagram package with a given port and message.
It sends the message continuously in a loop with a given sleep period between each package.

See for further details also the package [mkinitcpio-multiflash](https://github.com/j54n1n/alarm-mkinitcpio-multiflash).
