#ifndef TYPES_H
#define TYPES_H

typedef enum
{
    m_failure,
    m_success
} Status;

typedef enum
{
    m_view,
    m_edit,
    m_help,
    m_unsupported
} OperationType;

#endif