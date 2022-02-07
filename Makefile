TYPE=APPLICATION

CFLAGS+=-Wall

.PHONY: all clean install

all: showip.so

showip.so: showip.c
	gcc -Wall `pkg-config --cflags gtk+-2.0 lxpanel` -shared -fPIC showip.c -o showip.so `pkg-config --libs lxpanel`

clean:
	rm -f showip.so

install:
	if [ -d "/usr/lib/arm-linux-gnueabihf/lxpanel/plugins" ]; then \
		cp showip.so "/usr/lib/arm-linux-gnueabihf/lxpanel/plugins"; \
		echo "Installed to "
	else \
		if [ -d "/usr/lib/aarch64-linux-gnu/lxpanel/plugins" ]; then \
			cp showip.so "/usr/lib/aarch64-linux-gnu/lxpanel/plugins"; \
		fi \
	fi
