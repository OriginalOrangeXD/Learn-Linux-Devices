cmd_/root/makingDrivers/Module.symvers := sed 's/\.ko$$/\.o/' /root/makingDrivers/modules.order | scripts/mod/modpost  -a   -o /root/makingDrivers/Module.symvers -e -i Module.symvers  -N -T -
