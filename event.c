#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <dirent.h>

DIR *fd_d ;
struct dirent *pDir;
int main() {
    if((fd_d = opendir("/dev/input/")) == NULL ) {
      perror("unable to open DIR");
      return EXIT_FAILURE;
    }
    while((pDir = readdir(fd_d))!=NULL) {
      if(pDir->d_type == DT_CHR) {
        printf("[%s]\n",pDir->d_name);
      }

    }
    closedir(fd_d);
    return EXIT_SUCCESS;
}
