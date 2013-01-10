#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
 
#define INDEX(B, P) ((P) - (B))
 
int uz(const char * h, char * f, size_t len) {
  char a[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
  int d,c,b,m,l,k,j,n, g = 0, o = 0;
 
  if(!h) {
    return 0;
  }
 
  do{ 
    m = INDEX(a, strchr(a, h[g++]));
    l = INDEX(a, strchr(a, h[g++]));
    k = INDEX(a, strchr(a, h[g++]));
    j = INDEX(a, strchr(a, h[g++]));
    n = ( m << 18 ) | ( l << 12 ) | ( k << 6) | j;
    d = ( n >> 16 ) & 255;
    c = ( n >> 8 ) & 255;
    b = n & 255;
    if(k == 64) {
      f[o++] = d;
    } else {
      if(j == 64) {
        f[o++] = d;
        f[o++] = c;
      } else {
        f[o++] = d;
        f[o++] = c;
        f[o++] = b;
      }   
    }   
  }while(g < len);
 
  return o;
}
 
int main(int argc, char **argv) {
  
  if(argc < 2) {
    return 1;
  }
 
  char *src = NULL, *dst = NULL;
  struct stat fdistat;
  int o = 0;
  int fdi = open(argv[1], O_RDONLY);
 
  if(fdi < 0) {
    return 1;
  }
 
  fstat(fdi, &fdistat);
 
  src = mmap(0, fdistat.st_size, PROT_READ, MAP_PRIVATE, fdi, 0);
  if(src == MAP_FAILED) {
    perror("mmap src");
    close(fdi);
    return 1;
  }
 
  dst = mmap(NULL, fdistat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if(dst == MAP_FAILED) {
    perror("mmap dst");
    close(fdi);
    return 1;
  }
 
  o = uz(src, dst, fdistat.st_size);
  write(1, dst, o);
  if(munmap(dst, fdistat.st_size) == -1) {
    perror("munmap");
  }
  close(fdi);
  return 0;
}
