// (c) ZaKlaus 2016; Apache 2 Licensed, see LICENSE;;

#if !defined(DZM_MEM_H)

typedef struct
{
    mi Size;
    u8 *Base;
    mi Used;
    
    s32 TempCount;
} MEMORY_ARENA;

typedef struct
{
    MEMORY_ARENA *Arena;
    mi Used;
} TEMP_MEMORY;

typedef struct
{
    u32 Flags;
    u32 Alignment;
} ARENA_PUSH_PARAMS;

enum
{
    ArenaFlag_ClearToZero = 0x1
};

static inline void
initialize_arena(MEMORY_ARENA *Arena, mi Size, void *Base)
{
    Arena->Size = Size;
    Arena->Base = (u8 *)Base;
    Arena->Used = 0;
    Arena->TempCount = 0;
}

static inline mi
get_alignment_offset(MEMORY_ARENA *Arena, mi Alignment)
{
    mi AlignmentOffset = 0;
    
    mi ResultPointer = (mi)Arena->Base + Arena->Used;
    mi AlignmentMask = Alignment - 1;
    if(ResultPointer & AlignmentMask)
    {
        AlignmentOffset = Alignment - (ResultPointer & AlignmentMask);
    }
    
    return(AlignmentOffset);
}

static inline ARENA_PUSH_PARAMS
default_arena_params(void)
{
    ARENA_PUSH_PARAMS Params;
    Params.Flags = ArenaFlag_ClearToZero;
    Params.Alignment = 4;
    return(Params);
}

static inline ARENA_PUSH_PARAMS
align_noclear(u32 Alignment)
{
    ARENA_PUSH_PARAMS Params = default_arena_params();
    Params.Flags &= ~ArenaFlag_ClearToZero;
    Params.Alignment = Alignment;
    return(Params);
}

static inline ARENA_PUSH_PARAMS
align(u32 Alignment, b32 Clear)
{
    ARENA_PUSH_PARAMS Params = default_arena_params();

    if(Clear)
    {
        Params.Flags |= ArenaFlag_ClearToZero;
    }
    else
    {
        Params.Flags &= ~ArenaFlag_ClearToZero;
    }
    Params.Alignment = Alignment;
    return(Params);
}

static inline ARENA_PUSH_PARAMS
noclear(void)
{
    ARENA_PUSH_PARAMS Params = default_arena_params();
    Params.Flags &= ~ArenaFlag_ClearToZero;
    return(Params);
}

static inline mi
get_arena_size_remaining(MEMORY_ARENA *Arena, ARENA_PUSH_PARAMS Params)
{
    return (Arena->Size - (Arena->Used + get_alignment_offset(Arena, Params.Alignment)));
}

static inline mi
get_effective_size_for(MEMORY_ARENA *Arena, mi SizeInit, ARENA_PUSH_PARAMS Params)
{
    mi Size = SizeInit;
    
    mi AlignmentOffset = get_alignment_offset(Arena, Params.Alignment);
    
    return(Size + AlignmentOffset);
}

static inline b32
arena_hasroom_for(MEMORY_ARENA *Arena, mi SizeInit, ARENA_PUSH_PARAMS Params)
{
    mi Size = get_effective_size_for(Arena, SizeInit, Params);
    
    return(((Arena->Used + Size) <= Arena->Size));
}

static inline void *
push_size(MEMORY_ARENA *Arena, mi SizeInit, ARENA_PUSH_PARAMS Params)
{
    mi Size = get_effective_size_for(Arena, SizeInit, Params);
    
    zassert((Arena->Used + Size) <= Arena->Size);
    
    mi AlignmentOffset = get_alignment_offset(Arena, Params.Alignment);
    void *Result = Arena->Base + Arena->Used + AlignmentOffset;
    Arena->Used += Size;
    
    zassert(Size >= SizeInit);
    
    if(Params.Flags & ArenaFlag_ClearToZero)
    {
        zero_size(SizeInit, Result);
    }
    
    return(Result);
}

static inline char *
push_string(MEMORY_ARENA *Arena, char *Source)
{
    u32 Size = 1;
    for(char *At = Source;
        *At;
        ++At)
    {
        ++Size;
    }
    
    char *Dest = (char *)push_size(Arena, Size, noclear());
    for(u32 CharIdx = 0;
        CharIdx < Size;
        ++CharIdx)
    {
        Dest[CharIdx] = Source[CharIdx];
    }
    
    return(Dest);
}

static inline char *
push_string0(MEMORY_ARENA *Arena, u32 Length, char *Source)
{
    char *Dest = (char *)push_size(Arena, Length + 1, noclear());
    for(u32 CharIdx = 0;
        CharIdx < Length;
        ++CharIdx)
    {
        Dest[CharIdx] = Source[CharIdx];
    }
    Dest[Length] = 0;
    
    return(Dest);
}

static inline TEMP_MEMORY
begin_temp(MEMORY_ARENA *Arena)
{
    TEMP_MEMORY Result = {
        .Arena = Arena,
        .Used = Arena->Used
    };
    
    ++Arena->TempCount;
    
    return(Result);
}

static inline void
end_temp(TEMP_MEMORY TempMem)
{
    MEMORY_ARENA *Arena = TempMem.Arena;
    zassert(Arena->Used >= TempMem.Used);
    Arena->Used = TempMem.Used;
    zassert(Arena->TempCount > 0);
    --Arena->TempCount;
}

static inline void
clear_arena(MEMORY_ARENA *Arena)
{
    initialize_arena(Arena, Arena->Size, Arena->Base);
}

/*static inline void
check_arena(MEMORY_ARENA *Arena)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
    zassert(Arena->TempCount == 0);
#pragma clang diagnostic pop
}*/

static inline void *
copy(mi Size, void *SourceInit, void *DestInit)
{
    u8 *Source = (u8 *)SourceInit;
    u8 *Dest = (u8 *)DestInit;
    while(Size--) {*Dest++ = *Source++;}
    
    return(DestInit);
}

// NOTE(zaklaus): Initialize and only use with TEMP_MEMORY!!!
MEMORY_ARENA StringArena;
MEMORY_ARENA TempArena;

void
init_mem(void)
{
    initialize_arena(&StringArena, Megabytes(64), malloc(Megabytes(64)));
    initialize_arena(&TempArena, Megabytes(32), malloc(Megabytes(32)));
}

#define push_struct(Arena, type, ...) (type *)push_size(Arena, sizeof(type), ## __VA_ARGS__)
#define push_array(Arena, Count, type, ...) (type *)push_size(Arena, Count*sizeof(type), ## __VA_ARGS__)
#define push_copy(Arena, Size, Source, ...) copy(Size, Source, push_size(Arena, Size, ## __VA_ARGS__))
#define push_type push_struct

#define DPARAMS default_arena_params

#define DZM_MEM_H
#endif
