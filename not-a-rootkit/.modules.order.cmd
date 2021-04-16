cmd_/root/makingDrivers/not-a-rootkit/modules.order := {   echo /root/makingDrivers/not-a-rootkit/scriptKiddieRootkit.ko; :; } | awk '!x[$$0]++' - > /root/makingDrivers/not-a-rootkit/modules.order
