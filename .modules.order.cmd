cmd_/root/makingDrivers/modules.order := {   echo /root/makingDrivers/hello.ko; :; } | awk '!x[$$0]++' - > /root/makingDrivers/modules.order
