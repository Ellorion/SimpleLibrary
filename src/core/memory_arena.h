#pragma once

struct MemoryArena {
    void *pool = nullptr;
    u64 size = 0;
    u64 pos = 0;
    const char *debug_name = nullptr;
};

instant MemoryArena
MemoryArena_Create(
    u64 size,
    const char *name = ""
) {
    MemoryArena arena;

    arena.size = size;
    arena.pool = _Memory_Alloc_Empty(arena.size);
    arena.debug_name = name;

    return arena;
}

constexpr
instant void *
_MemoryArena_Alloc (
    MemoryArena &arena,
    u64 size
) {
    if (size > arena.size - arena.pos) {
        VALUE(arena.size)
        VALUE(arena.debug_name)
        AssertMessage(false, "Memory (temp) could not be allocated");
    }

    if (!arena.pool) {
        AssertMessage(false, "Memory (temp) not initialized.");
    }

    void *mem = arena.pool;

    arena.pool = (char *)arena.pool + size;
    arena.pos  += size;

    return mem;
}

constexpr
instant void
MemoryArena_Clear(
    MemoryArena &arena
) {
    arena.pos = 0;
}

constexpr
instant void
MemoryArena_Free(
    MemoryArena &arena
) {
    MemoryArena_Clear(arena);
    arena.size = 0;
    Memory_Free(arena.pool);
}
