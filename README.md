# ShowIP
Lxpanel plugin to show eth0/wlan0 IP address on top bar

# Installation
```
sudo apt-get --assume-yes install libfm-dev libfm-gtk-dev build-essential libx11-dev libxtst-dev libxi-dev x11proto-randr-dev libxrandr-dev lxpanel-dev pkg-config libgtk2.0-dev
it clone https://github.com/duckythescientist/raspberry-pi-showip
cd raspberry-pi-showip
make
sudo make install
```

TODO: figure out which apt dependencies are actually needed

# Manual Installation
Compile source code (directly on Raspberry Pi) => `showip.so` is produced
Paste it in `/usr/lib/arm-linux-gnueabihf/lxpanel/plugins/showip.so`
or `/usr/lib/aarch64-linux-gnu/lxpanel/plugins/showip.so` for a 64-bit OS
Restart lxpanel with the following under X, or reboot:
```
%> lxpanelctl restart
```
But the panel won't appear yet, you have to add it in place by right-clicking on the top bar.
Programatically you can edit `~/.config/lxpanel/LXDE-pi/panels`



# pisetup
showip is also part of pisetup
https://github.com/olignyf/pisetup

# Developpers Documentation
https://wiki.lxde.org/en/How_to_write_plugins_for_LXPanel

# Some music to keep you entertained
https://youtu.be/RarDSAz_jGs
https://youtu.be/9UaJAnnipkY

# License
Free for educational use.
5$ fee for redistribution and/or commercial use.
