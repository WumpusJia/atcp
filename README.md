# atcp
Linux userspace TCP/IP stack.

Implemented with TUN/TAP.
##Usage:
* Create device node:
```
	sudo mkdir /dev/net (if it doesn't exist already)
    sudo mknod /dev/net/tap c 10 200
```
* Set permissions:
```
    e.g. sudo chmod 0666 /dev/net/tap
```
* Add a new tap dev and configure it(make tap0 persistent)
```
	sudo ip tuntap add mode tap dev tap0
    sudo ip link set dev tap0 up  # bring the if up
    sudo ip route add dev tap0 10.0.0.0/24
    sudo ip address add dev tap0 local 10.0.0.3/24
```

* Download atcp　
* Compile and run
```
	cd atcp
    make
    ./build/atcp
```


_If you prefer a temporary tap dev, just read this [article](http://backreference.org/2010/03/26/tuntap-interface-tutorial/). Do a liitle change to code and run it with sudo_


##Progress

Finished basic arp reply

Test:
```
	$ arping -I tap0 10.0.0.2
	ARPING 10.0.0.2 from 10.0.0.3 tap0
	Unicast reply from 10.0.0.2 [52:27:1E:B1:F6:60]  0.567ms
	Unicast reply from 10.0.0.2 [52:27:1E:B1:F6:60]  0.651ms
```
##License
Apache Version 2.0

##Reference
* Linux kernel TCP/IP stack
