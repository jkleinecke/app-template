//=========================
// Character Functions
//=========================

function B32 str8_char_is_slash(U8 c)
{
    return(c == '/' || c == '\\');
}

function U8 str8_char_uppercase(U8 c)
{
    if('a' <= c && c <= 'z')
    {
        c += 'A' - 'a';
    }
    return(c);
}

function U8 str8_char_lowercase(U8 c)
{
    if('A' <= c && c <= 'Z')
    {
        c += 'a' - 'A';
    }
    return(c);
}

//=========================
// String Functions
//=========================

function String8 str8(U8 *str, U64 size)
{
    String8 ret = {str,size};
    return(ret);
}

function String8 str8_range(U8 *first, U8 *opl)
{
    String8 ret = {first, (U64)(opl - first)};
    return(ret);
}

function String8 str8_cstring(U8* cstr)
{
    U8 *ptr = cstr;
    for(;*ptr != 0; ptr += 1);
    return(str8_range(cstr,ptr));
}

function String8 str8_prefix(String8 str, U64 size)
{
    U64 size_clamped = ClampTop(size, str.size);
    String8 ret = {str.str, size_clamped};
    return(ret);
}

function String8 str8_chop(String8 str, U64 amount)
{
    U64 amount_clamped = ClampTop(amount, str.size);
    U64 remaining_size = str.size - amount_clamped;
    String8 ret = {str.str, remaining_size};
    return(ret);
}

function String8 str8_postfix(String8 str, U64 size)
{
    U64 size_clamped = ClampTop(size, str.size);
    U64 skip_to = str.size - size_clamped;
    String8 ret = {str.str + skip_to, size_clamped};
    return(ret);
}

function String8 str8_skip(String8 str, U64 amount)
{
    U64 amount_clamped = ClampTop(amount, str.size);
    U64 remaining_size = str.size - amount_clamped;
    String8 ret = {str.str + amount_clamped, remaining_size};
    return(ret);
}

function String8 str8_substr_opl(String8 str, U64 first, U64 opl)
{
    U64 opl_clamped = ClampTop(opl, str.size);
    U64 first_clamped = ClampTop(first, opl_clamped);
    String8 ret = {str.str + first_clamped, opl_clamped - first_clamped};
    return(ret);
}

function String8 str8_substr_size(String8 str, U64 first, U64 size)
{
    String8 ret = str8_substr_opl(str, first, first + size);
    return(ret);
}

function void str8_list_push(M_Arena *arena, String8List *list, StringJoin *join_optional)
{
    // setup join params
    local StringJoin dummy_join = {};
    StringJoin *join = join_optional;
    if(join == 0)
    {
        join = &dummy_join;
    }

    // compute total size
    U64 size = (join->pre.size +
                join->post.size +
                join->mid.size*(list->node_count - 1) +
                list->total_size);
    
    // begin string build
    U8 *str = push_array(arena, U8, size+1);
    U8 *ptr = str;

    // Write pre
    MemoryCopy(ptr, join->pre.str, join->pre.size);
    ptr += join->pre.size;

    B32 is_mid = false;
    for(String8Node *node = list->first; node != 0; node = node->next)
    {
        // write mid
        if(is_mid)
        {
            MemoryCopy(ptr, join->mid.str, join->mid.size);
            ptr += join->mid.size;
        }

        // write node
        MemoryCopy(ptr, node->string.str, node->string.size);
        ptr += node->string.size;

        is_mid = true;
    }

    // write post
    MemoryCopy(ptr, join->post.str, join->post.size);
    ptr += join->post.size;

    // write null
    *ptr = 0;

    String8 result = str8(str,size);
    return(result);
}
