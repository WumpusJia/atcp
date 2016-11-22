# atcp
Linux userspace TCP/IP stack.

Implemented with TUN/TAP.
##Usage:
* Download atcp

* Run settap script
```
	sudo chmod +x ./settap.sh
	sudo ./settap.sh
```
* Compile and run
```
	cd atcp
    make
    ./build/atcp
```


_If you prefer a temporary tap dev, just read this [article](http://backreference.org/2010/03/26/tuntap-interface-tutorial/). Do a liitle change to code and run it with sudo_


##Support

* Basic arp send/request
* ICMPv4 echo reply


Test:
```
	$ arping -I tap0 10.0.0.2
	$ ping -c3 10.0.0.2
```

** Due to without using multithread yet, you need to arping first!**

##License
Apache Version 2.0

##Reference
* Linux kernel TCP/IP stack
