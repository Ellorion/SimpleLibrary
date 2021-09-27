#pragma once

struct Context {
    Timer tmrPerf;
    MemoryArena arena;
};

Context Context_Init() {
    Context context;

    context.arena = MemoryArena_Create(Kilobyte(1));

    return context;
}

inline Context context = Context_Init();
