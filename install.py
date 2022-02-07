#!/usr/bin/env python3

import time
import re
import subprocess
import os.path

import common


def main():
    target_dirs = ["/usr/lib/arm-linux-gnueabihf/lxpanel/plugins/", "/usr/lib/aarch64-linux-gnu/lxpanel/plugins/"]
    for d in target_dirs:
        if os.path.isdir(d):
            subprocess.call(["cp", "showip.so", d])
            break
    else:
        print("Failed to install plugin. Unknown lib directory")
        return

    config = None
    path = os.path.expanduser("~/.config/lxpanel/LXDE-pi/panels/panel")
    try:
        config = open(path, 'r').read()
    except FileNotFoundError:
        print("Failed to open LXDE panel config.")
        return

    showip_config = 'Plugin {\n  type=showip\n  Config {\n  }\n}'
    index = config.find(showip_config)
    if index >= 0:
        print("Plugin was already added to lxpanel")
        return

    # config = common.ReplaceBetween(config, 'Plugin {\n  type=volumealsa\n  Config {\n  }\n}\n', 'Plugin', showip_config+"\n")

    with open(path, "a") as f:
        f.write(showip_config)

    subprocess.call(["lxpanelctl", "restart"])

if __name__ == '__main__':
    main()
