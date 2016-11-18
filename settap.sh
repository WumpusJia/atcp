sudo mknod /dev/net/tap c 10 200
sudo chmod 0666 /dev/net/tap
sudo ip tuntap add mode tap dev tap0
sudo ip link set dev tap0 up  # bring the if up
sudo ip route add dev tap0 10.0.0.0/24
sudo ip address add dev tap0 local 10.0.0.3/24
