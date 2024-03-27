int tuntap_create_tun_device(char *tun_dev_name);
int tuntap_set_tun_dev_ip(char *tun_dev_name, char *tun_ip);
int tuntap_delete_tun_device(char *tun_dev_name);

int tuntap_create_tun_route(char *tun_dev_name);

// void create_tap_device(char *tap_dev_name);
// void set_tap_dev_ip(char *tap_ip);