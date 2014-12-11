#set remote X-packet disable
#set remotedebug 1
set remotebaud 57600
set remotelogfile sid.log
target remote /dev/ttyS0
set radix 16
set listsize 20
set height 0
directory /usr/src/newlib-1.8.2/newlib/libc/stdio
directory /usr/src/newlib-1.8.2/newlib/libc/stdlib
directory /usr/src/newlib-1.8.2/newlib/libc/string
directory sid
directory ucos
directory sh704x

# the following sets up the minimum hardware
# configuration needed to download code
set *(short*)0xffff8620=0x2007
set *(short*)0xffff8622=0x55ff
set *(short*)0xffff8624=0x2103
set *(short*)0xffff839a=0xa005
set *(short*)0xffff839c=0xfffe
set *(short*)0xffff838c=0x1500
set *(short*)0xffff83a8=0x0200
set *(short*)0xffff83ac=0xffff

set $r15=0x880000
#set $r15=0xfffffffc

