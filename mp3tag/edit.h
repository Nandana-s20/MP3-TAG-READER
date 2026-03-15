#ifndef EDIT_H
#define EDIT_H
#include <stdio.h>
#include "type.h"

typedef struct
{
    char song_name[200];

    FILE *fptr_song;
    FILE *fptr_temp_song;

    char *new_tagdata;
    char tag_id[5];

} EditInfo;

Status read_and_validate_for_edit(char *argv[], EditInfo *editInfo);

Status do_edit(EditInfo *editInfo);

#endif
