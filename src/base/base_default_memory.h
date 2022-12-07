#ifndef BASE_DEFAULT_MEMORY_H
#define BASE_DEFAULT_MEMORY_H

#include <stdlib.h>
#if !defined(m_reserve)
# define m_reserve os_memory_reserve
#endif
#if !defined(m_commit)
# define m_commit os_memory_commit
#endif
#if !defined(m_decommit)
# define m_decommit os_memory_decommit
#endif
#if !defined(mem_release)
# define mem_release os_memory_release
#endif

#endif //BASE_DEFAULT_MEMORY_H
