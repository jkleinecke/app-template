#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

//===============================
// Base Memory Pre-Requisites
//===============================

#if !defined(m_reserve)
# error missing definition for 'm_reserve' type: (U64)->void* 
#endif
#if !defined(m_commit)
# error missing definition for 'm_commit' type: (void*,U64)->B32
#endif
#if !defined(m_decommit)
# error missing definition for 'm_decommit' type: (void*,U64)->void
#endif
#if !defined(mem_release)
# error missing definition for 'mem_release' type: (void*,U64)->void
#endif

//===============================
// Memory Arena Structures
//===============================

struct MemArena
{
    U64 cap;
    U64 pos;
    U64 commit_pos;
};

struct MemArenaTemp
{
    MemArena *arena;
    U64      pos;
};

//===============================
// Memory Arena Operations
//===============================

#define MEM_DEFAULT_RESERVE_SIZE GB(1)
#define MEM_COMMIT_BLOCK_SIZE MB(64)

#define MEM_SCRATCH_POOL_COUNT 2

function MemArena*   mem_alloc_arena_reserve(U64 reserve_size);
function MemArena*   mem_alloc_arena(void);
function void       mem_arena_release(MemArena* arena);
function void*      mem_arena_push(MemArena* arena, U64 size);
function void       mem_arena_pop_to(MemArena* arena, U64 pos);

function void*      mem_arena_push_zero(MemArena* arena, U64 size);
function void       mem_arena_align(MemArena* arena, U64 pow2_align);
function void       mem_arena_align_zero(MemArena* arena, U64 pow2_align);
function void       mem_arena_pop_amount(MemArena* arena, U64 amount);

#define push_struct(a,T) (T*)mem_arena_push((a), sizeof(T))
#define push_array(a,T,c) (T*)mem_arena_push((a), sizeof(T)*(c))
#define push_array_zero(a,T,c) (T*)mem_arena_push_zero((a), sizeof(T)*(c))

function MemArenaTemp    mem_begin_temp(MemArena* arena);
function void           mem_end_temp(MemArenaTemp temp);

function MemArenaTemp    mem_get_scratch(MemArena **conflict_array, U32 count);
#define                 mem_release_scratch(temp) mem_end_temp(temp)

#endif  // BASE_MEMORY_H
