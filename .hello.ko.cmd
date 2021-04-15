cmd_/root/makingDrivers/hello.ko := ld -r -m elf_x86_64 --build-id=sha1  -T scripts/module.lds -o /root/makingDrivers/hello.ko /root/makingDrivers/hello.o /root/makingDrivers/hello.mod.o;  true
