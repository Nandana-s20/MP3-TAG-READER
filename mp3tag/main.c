#include <stdio.h>
#include "type.h"
#include "view.h"
#include "edit.h"
#include <string.h>

OperationType check_operation_type(char *);

int main(int argc, char *argv[])
{

    if (argc <= 1)
    {
        printf("Error insufficient argument!\n");
        return m_failure;
    }
    ViewInfo viewInfo;
    OperationType op = check_operation_type(argv[1]);
    if (op == m_view)
    {

        if (argc < 3)
        {
            printf("Enter 3 arguments as eg ./a.out -v song.mp3\n");
            return m_failure;
        }
        if (read_and_validate_for_view(argv, &viewInfo) == m_success)
        {
            if (do_view(&viewInfo) == m_success)
            {
                return m_success;
            }
            else
            {
                printf("error in   viewing\n");
                return m_failure;
            }
        }
        else
        {
            printf("error in  validation\n ");
            return m_failure;
        }
    }
    else if (op == m_edit)
    {
        printf("-------------------------------select Edit option----------------------------------------\n");
        if (argc < 5)
        {
            printf("Enter 5 arguments as example ./a.out -e <tag> <new_id> song.mp3\n");
            return m_failure;
        }
        EditInfo editInfo;
        if (read_and_validate_for_edit(argv, &editInfo) == m_success)
        {
            if (do_edit(&editInfo) == m_success)
            {
                return m_success;
            }
            else
            {
                printf("Error in do editing\n");
                return m_failure;
            }
        }
        else
        {
            printf("error in validation\n");
            return m_failure;
        }
    }
    else if (op == m_help)
    {
        printf("usage:\n-t\tModifies the Title tag\n-a\tModifies an Artist tag\n-A\tModifies Album tag\n-y\tModifies year tag\n-c\tModifies the Content type\n-m\tModifies the Composer tag\n");
        return m_success;
    }
    else
    {
        printf("Error in argument use valid one \n");
        return m_failure;
    }
}
OperationType check_operation_type(char *symbol)
{
    if (strcmp(symbol, "-v") == 0)
    {
        return m_view;
    }
    else if (strcmp(symbol, "-e") == 0)
    {
        return m_edit;
    }
    else if (strcmp(symbol, "--help") == 0)
    {
        return m_help;
    }
    else
    {
        return m_unsupported;
    }
}