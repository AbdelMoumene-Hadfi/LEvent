#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include "./event.h"

int main() {
    int fd_f , count =0;
    char file_path[50],name[256]="Unkown";
    DIR *fd_d ;
    struct dirent *pDir;
    if((fd_d = opendir("/dev/input/")) == NULL ) {
      perror("unable to open DIR");
      return EXIT_FAILURE;
    }
    printf("List of Devices : \n");
    while((pDir = readdir(fd_d))!=NULL) {
      /*get just file*/
      if(pDir->d_type == DT_CHR) {
        strcpy(file_path,"/dev/input/");
        strcat(file_path,pDir->d_name);
        if((fd_f = open(file_path,O_RDONLY))<0) {
          //perror("enable to open file");
          continue ;
        }
        if(ioctl(fd_f, EVIOCGNAME(sizeof(name)), name) < 0) {
          //perror("enable to control device");
          close(fd_f);
          continue;
        }
        printf(" |- [%s] : %s\n",file_path,name);
        count++;
        close(fd_f);

      }
    }
    int event_nm;
    printf("  => Entrer event number [0-%d] : ",count-1);
    scanf("%d",&event_nm);
    while(!(0<=event_nm && event_nm<count)) {
      printf("  => Entrer event number [0-%d] : ",count-1);
      scanf("%d",&event_nm);
    }
    sprintf(file_path,"/dev/input/event%d",event_nm);
    if((fd_f = open(file_path,O_RDONLY))<0) {
      perror("enable to open file");
      closedir(fd_d);
      return EXIT_FAILURE;
    }
    printf("Device info : \n");
    int version;
    struct input_id device_info;
    if(ioctl(fd_f, EVIOCGNAME(sizeof(name)), name) < 0) {
      perror("enable to control device");
    }
    else {
        printf(" |- name : %s\n",name);
    }
    if(ioctl(fd_f,EVIOCGVERSION,&version) < 0) {
      perror("[ERROR] unable to get driver version");
    }
    else {
      printf(" |- driver_version : %d.%d.%d\n",version>>16,(version>>8)&0xff,version & 0xff);
    }
    if(ioctl(fd_f, EVIOCGID, &device_info) < 0) {
      perror("[ERROR] unable to get device info");
    }
    else {
      printf(" |- VENDOR_ID : %04hx\n |- PRODUCT_ID : %04hx\n |- VERSION : %04hx\n",device_info.vendor, device_info.product,device_info.version);
      switch(device_info.bustype) {
        case BUS_PCI	: printf(" |- BUS : PCI \n");break;
        case BUS_ISAPNP : printf(" |- BUS : ISAPNP \n");break;
        case BUS_USB	: printf(" |- BUS : USB\n");break;
        case BUS_HIL	: printf(" |- BUS : HIL\n");break;
        case BUS_BLUETOOTH : printf(" |- BUS : BLUETOOTH\n");break;
        case BUS_VIRTUAL: printf(" |- BUS : VIRTUAL \n");break;
        case BUS_ISA	: printf(" |- BUS : ISA\n");break;
        case BUS_I8042: printf(" |- BUS : I8042\n");break;
        case BUS_XTKBD: printf(" |- BUS : XTKBD \n");break;
        case BUS_RS232: printf(" |- BUS : RS232\n");break;
        case BUS_GAMEPORT: printf(" |- BUS : GAMEPORT\n");break;
        case BUS_PARPORT: printf(" |- BUS : PARPORT\n");break;
        case BUS_AMIGA: printf(" |- BUS : AMIGA\n");break;
        case BUS_ADB	: printf(" |- BUS : ADB\n");break;
        case BUS_I2C	: printf(" |- BUS : I2C\n");break;
        case BUS_HOST: printf(" |- BUS : HOST\n");break;
        case BUS_GSC	: printf(" |- BUS : GSC\n");break;
        case BUS_ATARI: printf(" |- BUS : ATARI\n");break;
        case BUS_SPI	: printf(" |- BUS : SPI\n");break;
        case BUS_RMI	: printf(" |- BUS : RMI\n");break;
        case BUS_CEC	: printf(" |- BUS : CEC\n");break;
        case BUS_INTEL_ISHTP	: printf(" |- BUS : INTEL_ISHTP\n");break;
        default : break;
      }
    }
    printf("Supported events :\n");
    unsigned long evtype_b[EV_MAX][NBITS(KEY_MAX)];
    if(ioctl(fd_f, EVIOCGBIT(0,EV_MAX), &evtype_b[0]) < 0) {
      perror("[ERROR] unable to get device info");
    }
    for(int yalv=0;yalv<EV_MAX;yalv++) {
      if(test_bit(yalv,evtype_b[0])) {
        printf(" |- Event type %d (%s)\n",yalv,events[yalv]?events[yalv]:"?");
        if(!yalv)
          continue;
        ioctl(fd_f, EVIOCGBIT(yalv,maxes[yalv]), &evtype_b[yalv]);
        for(int yalv2=0;yalv2<maxes[yalv];yalv2++) {
          if(test_bit(yalv2,evtype_b[yalv])) {
            printf("    |- Event code %d (%s)\n",yalv2,names[yalv]?(names[yalv][yalv2]?names[yalv][yalv2]:"?"):"?");
          }
        }
      }
    }

    close(fd_f);
    closedir(fd_d);
    return EXIT_SUCCESS;
}
