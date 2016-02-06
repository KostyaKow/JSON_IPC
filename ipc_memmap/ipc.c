#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <assert.h>
#include "crypto.h"

void check(int test, const char* msg, ...) {
   if (test) {
      va_list args;
      va_start(args, msg);
      vfprintf(stderr, msg, args);
      va_end(args);
      fprintf(stderr, "\n");
      exit(EXIT_FAILURE);
   }
}

const char* map_readonly(const char* file_name, size_t* size_return) {
   int fd = open(file_name, O_RDONLY);
   check(fd < 0, "Can't open %s: %s", file_name, strerror(errno));

   struct stat s;
   int status = fstat(fd, &s);
   check(status < 0, "Stat for file %s failed: %s", file_name, strerror(errno));
   size_t size = s.st_size;

   const char *mapped = mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
   /*if (mapped == MAP_FAILED)
      fprintf(stderr, "Mmap for file %s failed: %s", file_name, strerror(errno));*/
   check(mapped == MAP_FAILED, "Mmap for file %s failed: %s", file_name, strerror(errno));

   *size_return = size;
   return mapped;
}

//mmap MAP_SHARED
const char*
map_synced(const char* file_name, size_t* size_return) {
   int fd = open(file_name, O_RDWR);
   check(fd < 0, "Can't open %s: %s", file_name, strerror(errno));

   struct stat s;
   int status = fstat(fd, &s);
   check(status < 0, "Stat for file %s failed: %s", file_name, strerror(errno));
   size_t size = s.st_size;

   const char *mapped = mmap(0, size, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 0);
   /*if (mapped == MAP_FAILED)
      fprintf(stderr, "Mmap for file %s failed: %s", file_name, strerror(errno));*/
   check(mapped == MAP_FAILED, "Mmap for file %s failed: %s", file_name, strerror(errno));

   *size_return = size;
   return mapped;
}

int main(int nargs, uchar** args) {
   //return test_encrypt_decrypt(nargs, args);
   init_crypto();

   long test_data_size = strtol(args[1], NULL, 10)*1024;
   //printf("%i", test_data_size);
   char* to_encrypt = malloc(test_data_size);

   for (int i = 0; i < test_data_size; i++)
      to_encrypt[i] = i % 256;

   /**/
   uchar* encrypted = malloc(test_data_size + (test_data_size % 16)); /*20*1024*/
   uchar* decrypted = malloc(test_data_size);
   int cipher_len = encrypt_(to_encrypt, test_data_size-1, key, iv, encrypted);
   int decrypted_len = decrypt_(encrypted, cipher_len, key, iv, decrypted);

   printf("decrypted: %i\n", decrypted_len);
   printf("encrypted: %i\n", cipher_len);
   //assert(decrypted_len == test_data_size);
   shutdown_crypto();
    /**/

   //uchar* encrypted = encrypt(to_encrypt, test_data_size, key);
   //uchar* decrypted = decrypt(encrypted, test_data_size, key);

   return decrypted[0];
   size_t size;
   const char* mem = map_readonly("/tmp/a", &size);
   /*for (int i = 0; i < size; i++)
      putchar(mem[i]);*/

   while (1) {
      printf("\n");
      usleep(1000000); //1,000,000 = million
      if (mem[0] == '0')
         printf("{C} no messages from python");
      else if (mem[0] == '1')
         printf("{C} python is writing to our file");
      else if (mem[0] == '2') {
         printf("{C} python finish writing message");
         printf("message: %s", mem+1);
      }
      else {
         printf("got garbage, %c", mem[0]);
      }
   }
}

