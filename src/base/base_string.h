#ifndef BASE_STRING_H
#define BASE_STRING_H

//=========================
// String Types
//=========================

struct String8
{
    U8 *str;
    U64 size;   // does this need to be 64-bit?
};

struct String8Node
{
    String8Node *next;
    String8 string;
};

struct String8List
{
    String8Node *first;
    String8Node *last;
    U64 node_count;
    U64 total_size;
};

struct StringJoin
{
    String8 pre;
    String8 mid;
    String8 post;
};

typedef U32 StringMatchFlags;
enum
{
    kStringMatchFlags_Exact = 0,
    kStringMatchFlags_NoCase = (1 << 0),
};

struct String16
{
    U16 *str;
    U64 size;
};

struct String32
{
    U32 *str;
    U64 size;
};

//=========================
// Unicode Helper Types
//=========================

struct StringDecode
{
    U32 codepoint;
    U32 size;
};

//=========================
// Character Functions
//=========================

function B32 str8_char_is_slash(U8 c);

function U8 str8_char_uppercase(U8 c);
function U8 str8_char_lowercase(U8 c);

//=========================
// String Functions
//=========================

function String8 str8(U8 *str, U64 size);
function String8 str8_range(U8 *first, U8 *opl);
function String8 str8_cstring(U8* cstr);

#define str8_lit(s) str8((U8*)(s), sizeof(s)-1)
#define str8_typed(s) str8((U8*)(s), sizeof(*(s)))

function String8 str8_prefix(String8 str, U64 size);
function String8 str8_chop(String8 str, U64 amount);
function String8 str8_postfix(String8 str, U64 size);
function String8 str8_skip(String8 str, U64 amount);
function String8 str8_substr_opl(String8 str, U64 first, U64 opl);
function String8 str8_substr_size(String8 str, U64 first, U64 size);

#define str8_expand(s) (int)((s).size), ((s).str)

function void str8_list_push_explicit(String8List *list, String8 string, String8Node *node_memory);
function void str8_list_push(MemArena *arena, String8List *list, String8 string);

function String8 str8_join(MemArena *arena, String8List *list, StringJoin *optional_join);

function String8List str8_split(MemArena *arena, String8 string, U8 *split_characters, U32 count);

function String8 str8_pushfv(MemArena *arena, char *fmt, va_list args);
function String8 str8_pushf(MemArena *arena, char *fmt, ...);
function void    str8_list_pushf(MemArena *arena, String8List *list, char *fmt, ...);

function String8 str8_push_copy(MemArena *arena, String8 string);

function String16 str16(U16 *str, U64 size);
function String16 str16_cstring(U16 *cstr);

//=========================
// String Path Helpers
//=========================

function String8 str8_chop_last_slash(String8 string);

//=========================
// String Comparison
//=========================

function B32 str8_match(String8 str1, String8 str2, StringMatchFlags flags);

//=========================
// Unicode Functions
//=========================

function StringDecode str_decode_utf8(U8 *str, U64 cap);
function U32          str_encode_utf8(U8 *dst, U32 codepoint);
function StringDecode str_decode_utf16(U16 *str, U64 cap);
function U32          str_encode_utf16(U16 *dst, U32 codepoint);

function String32     str32_from_str8(MemArena *arena, String8 string);
function String8      str8_from_str32(MemArena *arena, String32 string);
function String16     str16_from_str8(MemArena *arena, String8 string);
function String8      str8_from_str16(MemArena *arena, String16 string);

//=========================
// String Serialization
//=========================

function B32 str8_read(String8 data, U64 off, void *dst, U64 size);
#define str8_read_typed(d,o,p) str8_read((d), (o), p, sizeof(*(p)))

#endif // BASE_STRING_H
