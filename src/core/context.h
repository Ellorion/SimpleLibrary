#pragma once

enum class Content_Arena_Type {
    Flush,
    Temp,
    Pool,
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

void Context_SetArena(Content_Arena_Type type) {
    switch (type) {
        case Content_Arena_Type::Flush: {
            context.arena_current = &context.arena_flush;
        } break;

        case Content_Arena_Type::Temp: {
            context.arena_current = &context.arena_temp;
        } break;

        case Content_Arena_Type::Pool: {
            context.arena_current = &context.arena_pool;
        } break;

        default:
            AssertMessage(false, "Unhandled Content Arena Type");
    }
}
