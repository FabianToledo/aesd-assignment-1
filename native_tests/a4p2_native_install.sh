#!/bin/bash

# This test file will execute natively the building and installation 
# to test the Assignment 4 Part 2 setup

MAKE=make
D="$(dirname $0)/.."
INSTALL=install
TARGET_DIR=

${MAKE} -C ${D}/finder-app clean
${MAKE} -C ${D}/finder-app all

sudo ${INSTALL} -m 0755 -d ${D}/conf/ /etc/finder-app/conf/
sudo ${INSTALL} -m 0644 ${D}/conf/* /etc/finder-app/conf/
sudo ${INSTALL} -m 0755 ${D}/assignment-autotest/test/assignment4/* /bin
sudo ${INSTALL} -m 0755 ${D}/finder-app/writer /usr/bin
sudo ${INSTALL} -m 0755 ${D}/finder-app/finder.sh /usr/bin
sudo ${INSTALL} -m 0755 ${D}/finder-app/finder-test.sh /usr/bin
