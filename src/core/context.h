#pragma once

enum class Context_Arena_Type {
    Flush,
    Temp,
    Pool,

    Count,
};

struct Context {
    Timer timer_performance;

    MemoryArena arena_flush;
    MemoryArena arena_temp;
    MemoryArena arena_pool;
    MemoryArena *arena_current = &arena_pool;
};

Context Context_Init() {
    Context context;

    context.arena_flush = MemoryArena_Create(Gigabyte(1), "Flush");
    context.arena_temp  = MemoryArena_Create(Gigabyte(1), "Temp");
    context.arena_pool  = MemoryArena_Create(Gigabyte(1), "Pool");

    return context;
}

inline Context context = Context_Init();

instant void *
MemoryArena_Alloc (
    u64 size
) {
    return _MemoryArena_Alloc(*context.arena_current, size);
}

instant MemoryArena*
Context_GetArena(Context_Arena_Type type) {
    switch (type) {
        case Context_Arena_Type::Flush:
            return &context.arena_flush;

        case Context_Arena_Type::Temp:
            return &context.arena_temp;

        case Context_Arena_Type::Pool:
            return &context.arena_pool;

        default:
            AssertMessage(false, "Unhandled Content Arena Type");
            return nullptr;
    }
}

instant Context_Arena_Type
Context_GetArenaType() {
    auto type = Context_Arena_Type::Count;

    FOR((u32)Context_Arena_Type::Count, it) {
        auto arena = Context_GetArena(Context_Arena_Type(it));
        bool isMatch = (context.arena_current == arena);

        if (isMatch) {
            type = Context_Arena_Type(it);
            break;
        }
    }

    return type;
}

instant void
Context_SetArena(
    Context_Arena_Type type
) {
    context.arena_current = Context_GetArena(type);
}
