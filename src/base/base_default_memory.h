#ifndef BASE_DEFAULT_MEMORY_H
#define BASE_DEFAULT_MEMORY_H

#include <stdlib.h>
#if !defined(m_reserve)
// # define m_reserve os_memory_reserve
# define m_reserve malloc
#endif
#if !defined(m_commit)
// # define m_commit os_memory_commit
# define m_commit(p,s) true
#endif
#if !defined(m_decommit)
// # define m_decommit os_memory_decommit
# define m_decommit(p,s) 
#endif
#if !defined(m_release)
// # define m_release os_memory_release
# define m_release(p,s) free(p)
#endif

#endif //BASE_DEFAULT_MEMORY_H
