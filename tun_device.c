#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
int create_tun_device(char *dev) {
 struct ifreq ifr;
 int fd, err;
 if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
 perror("Open /dev/net/tun");
 return fd;
 }
 memset(&ifr, 0, sizeof(ifr));
 ifr.ifr_flags = IFF_TUN | IFF_NO_PI; // Create a TUN device
 strncpy(ifr.ifr_name, dev, IFNAMSIZ);
 if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
 perror("Ioctl TUNSETIFF");
 close(fd);
 return err;
 }
 printf("TUN device %s created successfully.\n", ifr.ifr_name);
 return fd;
}
int main() {
 char tun_name[IFNAMSIZ] = "tun0";
 int tun_fd = create_tun_device(tun_name);
 if (tun_fd < 0) {
 printf("Failed to create TUN device.\n");
 return 1;
 }
 printf("TUN device setup complete: %s\n", tun_name);
 return 0;
}
