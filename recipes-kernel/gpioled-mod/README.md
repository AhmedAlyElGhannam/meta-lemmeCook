```bash
cd /lib/modules/5.15.92-v8/extra/
insmod gpiotoggle.ko.xz
cd /proc
echo 1 > hehe # turns on led connected to GPIO26
echo 0 > hehe # turns off led connected to GPIO26
rmmod gpiotoggle.ko.xz
```
 
