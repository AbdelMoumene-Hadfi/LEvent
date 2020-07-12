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
    if(ioctl(fd_f,EVIOCGVERSION,&version) < 0) {
      perror("[ERROR] unable to get driver version");
    }
    else {
      printf("[INFO] driver_version : %d.%d.%d\n",version>>16,(version>>8)&0xff,version & 0xff);
    }
    if(ioctl(fd_f, EVIOCGNAME(sizeof(name)), name) < 0) {
      perror("enable to control device");
    }
    else {
        printf("[INFO] name : %s\n",name);
    }

    close(fd_f);
    closedir(fd_d);
    return EXIT_SUCCESS;
}
