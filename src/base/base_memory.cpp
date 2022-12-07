
//===============================
// Memory Arena Operations
//===============================

#define MEM_INITIAL_COMMIT KB(4)
StaticAssert(sizeof(MemArena) <= MEM_INITIAL_COMMIT, check_arena_size);

function MemArena*
mem_alloc_arena_reserve(U64 reserve_size)
{
    MemArena* result = 0;
    if(reserve_size >= MEM_INITIAL_COMMIT)
    {
        void* memory = m_reserve(reserve_size);
        if(m_commit(memory, MEM_INITIAL_COMMIT))
        {
            result = (MemArena*)memory;
            result->cap = reserve_size;
            result->pos = AlignUpPow2(sizeof(MemArena), 64);
            result->commit_pos = MEM_INITIAL_COMMIT;
        }
    }
    Assert(result != 0);
    return(result);
}

function MemArena*
mem_alloc_arena(void)
{
    MemArena *result = mem_alloc_arena_reserve(MEM_DEFAULT_RESERVE_SIZE);
    return(result);
}

function void
mem_arena_release(MemArena *arena)
{
    mem_release(arena, arena->cap);
}

function void*
mem_arena_push(MemArena *arena, U64 size)
{
    void* result = 0;
    if(arena->pos + size <= arena->cap)
    {
        // save the pointer we should return
        void *result_on_success = ((U8*)arena) + arena->pos;

        // grow the committed range
        U64 p = arena->pos + size;
        U64 commit_p = arena->commit_pos;
        if(p > commit_p)
        {
            U64 p_aligned = AlignUpPow2(p, MEM_COMMIT_BLOCK_SIZE);
            U64 next_commit_p = ClampTop(p_aligned, arena->cap);
            U64 commit_size = next_commit_p - commit_p;
            if(m_commit((U8*)arena + commit_p, commit_size))
            {
                commit_p = next_commit_p;
                arena->commit_pos = commit_p;
            }
        }

        // if the commit range contains the allocation range, save it
        if(p <= commit_p)
        {
            result = result_on_success;
            arena->pos = p;
        }
    }
    Assert(result != 0);
    return(result);
}

function void
mem_arena_pop_to(MemArena *arena, U64 pos)
{
    if(pos < arena->pos)
    {
        arena->pos = pos;

        U64 p = arena->pos;
        U64 p_aligned = AlignUpPow2(p, MEM_COMMIT_BLOCK_SIZE);
        U64 next_commit_p = ClampTop(p_aligned, arena->cap);

        U64 commit_p = arena->commit_pos;
        if(next_commit_p < commit_p)
        {
            U64 decommit_size = commit_p - next_commit_p;
            m_decommit((U8*)arena + next_commit_p, decommit_size);
            arena->commit_pos = next_commit_p;
        }
    }
}

function void*
mem_arena_push_zero(MemArena *arena, U64 size)
{
    void *result = mem_arena_push(arena, size);
    MemoryZero(result, size);
    return(result);
}

function void
mem_arena_align(MemArena *arena, U64 pow2_align)
{
    U64 p = arena->pos;
    U64 p_aligned = AlignUpPow2(p, pow2_align);
    U64 z = p_aligned - p;
    if(z > 0)
    {
        mem_arena_push(arena, z);
    }
}

function void
mem_arena_align_zero(MemArena *arena, U64 pow2_align)
{
    U64 p = arena->pos;
    U64 p_aligned = AlignUpPow2(p, pow2_align);
    U64 z = p_aligned - p;
    if(z > 0)
    {
        mem_arena_push_zero(arena, z);
    }
}

function void
mem_arena_pop_amount(MemArena *arena, U64 amount)
{
    if(amount <= arena->pos)
    {
        mem_arena_pop_to(arena, arena->pos - amount);
    }
}

function MemArenaTemp
mem_begin_temp(MemArena *arena)
{
    MemArenaTemp temp = {arena, arena->pos};
    return(temp);
}

function void
mem_end_temp(MemArenaTemp temp)
{
    mem_arena_pop_to(temp.arena, temp.pos);
}

threadvar MemArena *m__scratch_pool[MEM_SCRATCH_POOL_COUNT] = {};

function MemArenaTemp
mem_get_scratch(MemArena **conflict_array, U32 count)
{
    // TODO(james): Figure out exactly how this scratch function is going to work in practice

    // init on first time
    if(m__scratch_pool[0] == 0)
    {
        MemArena **scratch_slot = m__scratch_pool;
        for(U64 i = 0; i < MEM_SCRATCH_POOL_COUNT; i+=1, scratch_slot+=1)
        {
            *scratch_slot = mem_alloc_arena();
        }
    }

    // get non-conflicting arena
    MemArenaTemp result = {};
    MemArena **scratch_slot = m__scratch_pool;
    for(U64 i = 0; i < MEM_SCRATCH_POOL_COUNT; i+=1, scratch_slot+=1)
    {
        B32 is_non_conflict = true;
        MemArena **conflict_ptr = conflict_array;
        for(U32 j = 0; j < count; j+=1, conflict_ptr+=1)
        {
            if(*scratch_slot == *conflict_ptr)
            {
                is_non_conflict = false;
                break;
            }
        }
        if(is_non_conflict)
        {
            result = mem_begin_temp(*scratch_slot);
            break;
        }
    }

    return(result);
}
