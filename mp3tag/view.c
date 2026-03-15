#include <stdio.h>
#include "view.h"
#include <string.h>
#include "type.h"

Status read_and_validate_for_view(char *argv[], ViewInfo *viewinfo)
{
     char *ret = strstr(argv[2], ".mp3");
     if (ret != NULL && (strcmp(ret, ".mp3")) == 0)
     {

          strcpy(viewinfo->song_name, argv[2]);
     }
     else
     {
          printf("Error in .mp3\n");
          return m_failure;
     }

     viewinfo->fptr_song = fopen(viewinfo->song_name, "r");
     if (viewinfo->fptr_song == NULL)
     {

          printf("unable to open %s\n", viewinfo->song_name);
          return m_failure;
     }
     // fseek(viewinfo->fptr_song,0,SEEK_SET);
     unsigned char buffer[3];
     fread(buffer, 1, 3, viewinfo->fptr_song); // for checking id3 or not
     if (buffer[0] != 'I' || buffer[1] != 'D' || buffer[2] != '3')
     {
          printf("not ID3 file\n");
          fclose(viewinfo->fptr_song);
          return m_failure;
     }
     unsigned char smallbuf[2];
     fread(smallbuf, 1, 2, viewinfo->fptr_song);
     if (smallbuf[0] != 3 && smallbuf[0] != 4 && smallbuf[1] != 0) // for checking the version
     {
          printf("not a ID3v3 or ID3v4 version\n");
          fclose(viewinfo->fptr_song);
          return m_failure;
     }
     rewind(viewinfo->fptr_song);
     return m_success;
}
int convert_endian(unsigned char *size)
{
     return (size[0] << 24) | (size[1] << 16) | (size[2] << 8) | (size[3]);
}

Status do_view(ViewInfo *viewinfo)
{
     printf("----------------------------------------------------------------------------------------------------------------------------\n");
     printf("MP3 Tag Reader and Editor for ID3\n");

     printf("----------------------------------------------------------------------------------------------------------------------------\n");

     fseek(viewinfo->fptr_song, 10, SEEK_SET); // skip the 10 header bytes

     char tag[6][5] = {"TIT2", "TALB", "TYER", "TCON", "TPE1", "COMM"};
     char label[6][10] = {"Title", "Album", "year", "content", "Artist", "composer"};

     int count = 0;
     while (count < 6)
     {
          char buffer[5] = {0};
          if (fread(buffer, 1, 4, viewinfo->fptr_song) != 4)
          {
               break;
          } // read the tag

          unsigned char size_buf[4];
          fread(size_buf, 1, 4, viewinfo->fptr_song);
          int buffer_size = convert_endian(size_buf);

          fseek(viewinfo->fptr_song, 3, SEEK_CUR); // flag and null

          int match = 0;

          for (int i = 0; i < 6; i++)
          {
               if (strcmp(buffer, tag[i]) == 0)
               {
                    char data[buffer_size];
                    fread(data, 1, buffer_size - 1, viewinfo->fptr_song);
                    data[buffer_size - 1] = '\0';

                    printf("%10s : %s\n", label[i], data);
                    count++;
                    match = 1;
                    break;
               }
          }
          if (!match)
          {
               // skip the data
               fseek(viewinfo->fptr_song, buffer_size - 1, SEEK_CUR);
          }
     }
     return m_success;
}
