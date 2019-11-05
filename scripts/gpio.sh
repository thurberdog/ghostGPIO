#! /bin/sh
echo 175 > /sys/class/gpio/export
echo 42  > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio42/direction
echo out > /sys/class/gpio/gpio175/direction
echo 0 > /sys/class/gpio/gpio175/value
echo 0 > /sys/class/gpio/gpio42/value
sleep 1
echo 0 > /sys/class/gpio/gpio175/value
echo 1 > /sys/class/gpio/gpio42/value
