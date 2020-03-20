#include <stdio.h>
#include <stdlib.h>

int main () {

     unsigned char buffer[512];
     int n = 0;

     //SEND STDIN DATA TO BUFFER ARRAY
     while (fread(buffer, 512, 1, stdin) == 1) {

          //IMAGE FOUNDED
          if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] == 0xe0) {

               //OUTPUT FILE NAME
               char filename[8];
               sprintf(filename, "%03d.jpg", n);

               //CREATE AN OUTPUT FILE
               FILE* output;
               output = fopen (filename, "wb");

               //WRITE FIRST 512 BYTES
               fwrite(buffer, 512, 1, output);

               //SEND NEXTS BYTES TO OUTPUT FILE
               while (fread(buffer, 512, 1, stdin) == 1 && (buffer[0] != 0xff || buffer[1] != 0xd8 || buffer[2] != 0xff || buffer[3] != 0xe0))
                    fwrite(buffer, 512, 1, output);

               n++;

               fclose(output);
               fseek(stdin, -512, SEEK_CUR);
          }
     }

     //CASE DON'T FIND PICTURES
     if (!n)
          printf("Could not find pictures\n");

     return 0;
}
