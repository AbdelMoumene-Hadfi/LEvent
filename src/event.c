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

DIR *fd_d ;
struct dirent *pDir;
int main() {
    int fd_f , count =0;
    char file_path[50],name[256]="Unkown";
    if((fd_d = opendir("/dev/input/")) == NULL ) {
      perror("unable to open DIR");
      return EXIT_FAILURE;
    }
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
        printf("[%s] : %s\n",file_path,name);
        count++;
        close(fd_f);

      }
    }
    int event_nm;
    printf("Entrer event number [0-%d] : ",count-1);
    scanf("%d",&event_nm);
    while(!(0<=event_nm && event_nm<count)) {
      printf("Entrer event number [0-%d] : ",count-1);
      scanf("%d",&event_nm);
    }
    sprintf(file_path,"/dev/input/event%d",event_nm);
    if((fd_f = open(file_path,O_RDONLY))<0) {
      perror("enable to open file");
      closedir(fd_d);
      return EXIT_FAILURE;
    }
    int version;
    struct input_id device_info;
    if(ioctl(fd_f, EVIOCGNAME(sizeof(name)), name) < 0) {
      perror("enable to control device");
    }
    else {
        printf("[INFO] name : %s\n",name);
    }
    if(ioctl(fd_f,EVIOCGVERSION,&version) < 0) {
      perror("[ERROR] unable to get driver version");
    }
    else {
      printf("[INFO] driver_version : %d.%d.%d\n",version>>16,(version>>8)&0xff,version & 0xff);
    }
    if(ioctl(fd_f, EVIOCGID, &device_info) < 0) {
      perror("[ERROR] unable to get device info");
    }
    else {
      printf("[INFO] VENDOR_ID : %04hx\n[INFO] PRODUCT_ID : %04hx\n[INFO] VERSION : %04hx\n",device_info.vendor, device_info.product,device_info.version);
      switch(device_info.bustype) {
        case BUS_PCI	: printf("[INFO] BUS : PCI \n");break;
        case BUS_ISAPNP : printf("[INFO] BUS : ISAPNP \n");break;
        case BUS_USB	: printf("[INFO] BUS : USB\n");break;
        case BUS_HIL	: printf("[INFO] BUS : HIL\n");break;
        case BUS_BLUETOOTH : printf("[INFO] BUS : BLUETOOTH\n");break;
        case BUS_VIRTUAL: printf("[INFO] BUS : VIRTUAL \n");break;
        case BUS_ISA	: printf("[INFO] BUS : ISA\n");break;
        case BUS_I8042: printf("[INFO] BUS : I8042\n");break;
        case BUS_XTKBD: printf("[INFO] BUS : XTKBD \n");break;
        case BUS_RS232: printf("[INFO] BUS : RS232\n");break;
        case BUS_GAMEPORT: printf("[INFO] BUS : GAMEPORT\n");break;
        case BUS_PARPORT: printf("[INFO] BUS : PARPORT\n");break;
        case BUS_AMIGA: printf("[INFO] BUS : AMIGA\n");break;
        case BUS_ADB	: printf("[INFO] BUS : ADB\n");break;
        case BUS_I2C	: printf("[INFO] BUS : I2C\n");break;
        case BUS_HOST: printf("[INFO] BUS : HOST\n");break;
        case BUS_GSC	: printf("[INFO] BUS : GSC\n");break;
        case BUS_ATARI: printf("[INFO] BUS : ATARI\n");break;
        case BUS_SPI	: printf("[INFO] BUS : SPI\n");break;
        case BUS_RMI	: printf("[INFO] BUS : RMI\n");break;
        case BUS_CEC	: printf("[INFO] BUS : CEC\n");break;
        case BUS_INTEL_ISHTP	: printf("[INFO] BUS : INTEL_ISHTP\n");break;
        default : break;
      }

    }


    close(fd_f);
    closedir(fd_d);
    return EXIT_SUCCESS;
}
