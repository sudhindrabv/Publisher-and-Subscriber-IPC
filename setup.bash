mkdir project2
cd /root/project2

cp callnr.h /usr/src/include/minix/
cp table.c /usr/src/servers/pm/
cp proto.h /usr/src/servers/pm/
cp misc.c /usr/src/servers/pm/
cp topic.c /usr/src/servers/pm/
cp topic.h /usr/src/servers/pm/

cd /usr/src/servers/pm/
make
cd /root/project2
cp user_ipc.h /usr/include/
cd /usr/src/releasetools
make install
sync
reboot
