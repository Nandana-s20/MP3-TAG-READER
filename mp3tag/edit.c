#include <stdio.h>
#include "edit.h"
#include <string.h>
#include <stdlib.h>
#include "type.h"

void convert(unsigned int size, unsigned char *size_buf)
{
    size_buf[0] = (size >> 24) & 0XFF;
    size_buf[1] = (size >> 16) & 0XFF;
    size_buf[2] = (size >> 8) & 0XFF;
    size_buf[3] = (size) & 0XFF;
}

Status read_and_validate_for_edit(char *argv[], EditInfo *editInfo)
{
    // argv[2] validate
    if (strcmp(argv[2], "-t") == 0)
    {
        printf("-------------------------------------------select Title change option---------------------------------\n");

        strcpy(editInfo->tag_id, "TIT2");
    }
    else if (strcmp(argv[2], "-A") == 0)
    {
        printf("-------------------------------------------select Album change option---------------------------------\n");

        strcpy(editInfo->tag_id, "TALB");
    }
    else if (strcmp(argv[2], "-a") == 0)
    {
        printf("-------------------------------------------select Artist change option---------------------------------\n");

        strcpy(editInfo->tag_id, "TPE1");
    }
    else if (strcmp(argv[2], "-y") == 0)
    {
        printf("-------------------------------------------select Year change option---------------------------------\n");
        strcpy(editInfo->tag_id, "TYER");
    }
    else if (strcmp(argv[2], "-c") == 0)
    {
        printf("-------------------------------------------select Content change option---------------------------------\n");

        strcpy(editInfo->tag_id, "TCON");
    }
    else if (strcmp((argv[2]), "-m") == 0)
    {
        printf("-------------------------------------------select Composer change option---------------------------------\n");

        strcpy(editInfo->tag_id, "COMM");
    }
    else
    {
        printf("Enter valid tag use ./a.out --help sample.mp3 if needed tag id");
        return m_failure;
    }

    // argv[4] validation

    char *ret = strstr(argv[4], ".mp3");
    if (ret != NULL && strcmp(ret, ".mp3") == 0)
    {
        strcpy(editInfo->song_name, argv[4]);
    }
    else
    {
        printf("not an .mp3 file\n");
        return m_failure;
    }
    editInfo->new_tagdata = argv[3];

    // opening song file
    editInfo->fptr_song = fopen(editInfo->song_name, "rb");
    // error handling
    if (editInfo->fptr_song == NULL)
    {
        perror("fopen");
        printf("error opening %s\n", editInfo->song_name);
        return m_failure;
    }

    // opening temp file
    editInfo->fptr_temp_song = fopen("temp.mp3", "wb");
    // error handling
    if (editInfo->fptr_temp_song == NULL)
    {
        perror("fopen");
        printf("error opening temporary file");
        return m_failure;
    }
    return m_success;
}
Status do_edit(EditInfo *editInfo)
{
    // copy 10 byte header
    unsigned char buffer[10];
    fread(buffer, 1, 10, editInfo->fptr_song);
    fwrite(buffer, 1, 10, editInfo->fptr_temp_song);

    char id_buff[5] = {0};
    unsigned char size[4];
    unsigned int buff_size;
    unsigned char flag[2];

    while (1)
    {
        // read the id
        if (fread(id_buff, 1, 4, editInfo->fptr_song) != 4)
        {
            break;
        }

        id_buff[4] = '\0';

        if (id_buff[0] == '\0')
        {
            break;
        }

        // read size of id

        fread(size, 1, 4, editInfo->fptr_song);
        buff_size = (size[0] << 24) | (size[1] << 16) | (size[2] << 8) | (size[3]);

        fread(flag, 1, 2, editInfo->fptr_song);

        // check tag matches or not

        if (strcmp(id_buff, editInfo->tag_id) == 0)
        {
            unsigned new_size = strlen(editInfo->new_tagdata) + 1;
            unsigned char null = '\0';
            unsigned char new_size_buffer[4];

            convert(new_size, new_size_buffer);

            // write temp.mp3

            fwrite(id_buff, 1, 4, editInfo->fptr_temp_song);
            fwrite(new_size_buffer, 1, 4, editInfo->fptr_temp_song);
            fwrite(flag, 1, 2, editInfo->fptr_temp_song);
            fwrite(&null, 1, 1, editInfo->fptr_temp_song);

            fwrite(editInfo->new_tagdata, 1, strlen(editInfo->new_tagdata), editInfo->fptr_temp_song);
            fseek(editInfo->fptr_song, buff_size, SEEK_CUR);
        }
        else
        {
            // copy the frames
            char *buffer = malloc(buff_size);
            if (buffer == NULL)
            {
                printf("error in memory allocation");
                return m_failure;
            }

            fread(buffer, 1, buff_size, editInfo->fptr_song);
            fwrite(id_buff, 1, 4, editInfo->fptr_temp_song);
            fwrite(size, 1, 4, editInfo->fptr_temp_song);
            fwrite(flag, 1, 2, editInfo->fptr_temp_song);
            fwrite(buffer, 1, buff_size, editInfo->fptr_temp_song);
            free(buffer);
        }
    }
    // copy remaining
    unsigned char rem_data[1024];
    size_t bytes;
    while ((bytes = fread(rem_data, 1, sizeof(rem_data), editInfo->fptr_song)) > 0)
    {
        fwrite(rem_data, 1, bytes, editInfo->fptr_temp_song);
    }
    fclose(editInfo->fptr_song);
    fclose(editInfo->fptr_temp_song);

    // remove song.mp3 and rename temp

    remove(editInfo->song_name);
    rename("temp.mp3", editInfo->song_name);

    // printf("Successfully edited\n");
    if (strcmp(editInfo->tag_id, "TIT2") == 0)
    {
        printf("Title : %s\n", editInfo->new_tagdata);
        printf("-----------------------------------Title changed succesfully-----------------------------------------------------------\n");
    }
    else if (strcmp(editInfo->tag_id, "TPE1") == 0)
    {
        printf("Artist : %s\n", editInfo->new_tagdata);
        printf("-----------------------------------Artist changed succesfully-----------------------------------------------------------\n");
    }
    else if (strcmp(editInfo->tag_id, "TALB") == 0)
    {
        printf("Album : %s\n", editInfo->new_tagdata);
        printf("-----------------------------------Album changed succesfully-----------------------------------------------------------\n");
    }
    else if (strcmp(editInfo->tag_id, "COMM") == 0)
    {
        printf("Composer : %s\n", editInfo->new_tagdata);
        printf("-----------------------------------Composer changed succesfully-----------------------------------------------------------\n");
    }
    else if (strcmp(editInfo->tag_id, "TCON") == 0)
    {
        printf("Content : %s\n", editInfo->new_tagdata);
        printf("-----------------------------------Content changed succesfully-----------------------------------------------------------\n");
    }
    else if (strcmp(editInfo->tag_id, "TYER") == 0)
    {
        printf("Year : %s\n", editInfo->new_tagdata);
        printf("-----------------------------------Year changed succesfully-----------------------------------------------------------\n");
    }
    return m_success;
}
