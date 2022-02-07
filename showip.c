
/*

....

=BUILD INSTRUCTIONS

$> make

or 

$> gcc -Wall `pkg-config --cflags gtk+-2.0 lxpanel` -shared -fPIC showip.c -o showip.so `pkg-config --libs lxpanel`


=INSTALL INSTRUCTIONS

$> python install.py

or 

sudo cp showip.so /usr/lib/arm-linux-gnueabihf/lxpanel/plugins/
reboot or restart lxpanel
then right click on top panel and add "showip"

*/

#include <lxpanel/plugin.h>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdio.h>


// internal to the plugin source, not used by the 'priv' variable
static int iInstanceCount = 0;

static char ip[256] = {0};

typedef struct
{
	LXPanel *panel;
	gint iMyId;
	GtkWidget *gLabel;
	unsigned int timer;
} ShowIp;


int get_ip_from_interface(const char * interface, char * buf, size_t len) {
	int fd;
	struct ifreq ifr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	/* I want to get an IPv4 IP address */
	ifr.ifr_addr.sa_family = AF_INET;

	/* I want IP address attached to "eth0" */
	strncpy(ifr.ifr_name, interface, IFNAMSIZ-1);

	int ret = ioctl(fd, SIOCGIFADDR, &ifr);
	if (ret) {
		perror("fuck");
	}
	close(fd);
	if (! ret) {
		snprintf(buf, len, "%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
		printf("buf: %s\n", buf);
		return 0;
	}
	else {
		return 1;
	}
}


static void update_display(ShowIp *pPlugin)
{
	char ipbuf[256] = {0};

	memset(ip, 0, sizeof(ip));
	int ret = get_ip_from_interface("eth0", ipbuf, 256);
	if (!ret) {
		strncat(ip, "eth0: ", 255);
		strncat(ip, ipbuf, 255);
	}
	ret = get_ip_from_interface("wlan0", ipbuf, 256);
	if (!ret) {
		if (strlen(ip)) {
			strncat(ip, "\n", 255);
		}
		strncat(ip, "wlan0: ", 255);
		strncat(ip, ipbuf, 255);
	}

	// float textsize = strstr(ip, "\n") ? 0.1 : 1.0;  // doesn't seem to matter
	float textsize = 1.0;
	lxpanel_draw_label_text(pPlugin->panel, pPlugin->gLabel, ip, TRUE, textsize, FALSE);


	gtk_widget_set_tooltip_text(pPlugin->gLabel, "tooltip");
}

static gboolean update_display_timeout(gpointer user_data)
{
	if (g_source_is_destroyed(g_main_current_source()))
		return FALSE;
	update_display(user_data);
	return TRUE; /* repeat later */
}

GtkWidget *showip_constructor(LXPanel *panel, config_setting_t *settings)
{
	/* panel is a pointer to the panel and
	 settings is a pointer to the configuration data. */
	(void)settings;

	// allocate our private structure instance
	ShowIp *pPlugin = g_new0(ShowIp, 1);

	// update the instance count
	pPlugin->iMyId = ++iInstanceCount;
	pPlugin->panel = panel;

	// make a label out of the ID
	char cIdBuf[20];
	cIdBuf[sizeof(cIdBuf)-1] = '\0';
	snprintf(cIdBuf, sizeof(cIdBuf)-1, "IP %s", "127.0.0.1");

	// create a label widget instance
	pPlugin->gLabel = gtk_label_new(cIdBuf);

	update_display(pPlugin);

	// set the label to be visible
	gtk_widget_show(pPlugin->gLabel);

	// need to create a container to be able to set a border
	GtkWidget *p = gtk_event_box_new();

	// our widget doesn't have a window...
	// it is usually illegal to call gtk_widget_set_has_window() from application but for GtkEventBox it doesn't hurt
	gtk_widget_set_has_window(p, FALSE);

	// bind private structure to the widget assuming it should be freed using g_free()
	lxpanel_plugin_set_data(p, pPlugin, g_free);

	// set border width
	gtk_container_set_border_width(GTK_CONTAINER(p), 1);

	// add the label to the container
	gtk_container_add(GTK_CONTAINER(p), pPlugin->gLabel);

	// set the size we want
	gtk_widget_set_size_request(p, 140, 25);

	pPlugin->timer = g_timeout_add_seconds(4, (GSourceFunc) update_display_timeout, (gpointer)pPlugin);

	// success!!!
	return p;
}

FM_DEFINE_MODULE(lxpanel_gtk, showip)

/* Plugin descriptor. */
LXPanelPluginInit fm_module_init_lxpanel_gtk = {
	.name = "ShowIP",
	.description = "Display current IP address.",

	// assigning our functions to provided pointers.
	.new_instance = showip_constructor
};

