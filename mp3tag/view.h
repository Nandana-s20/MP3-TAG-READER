#ifndef VIEW_H
#define VIEW_H
#include <stdio.h>
#include "type.h"

typedef struct
{
    char song_name[200];
    long song_size;
    FILE *fptr_song;

} ViewInfo;

Status read_and_validate_for_view(char *argv[], ViewInfo *viewInfo);

Status do_view(ViewInfo *viewInfo);
int convert_endian(unsigned char *size);

#endif
