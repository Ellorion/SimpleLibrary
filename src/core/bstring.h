#pragma once

#define BSTRING_BUCKET_MAX 10

struct Bucket {
    u64 size = 0;
    void *ptr = nullptr;
    bool is_reference = false;
};

// Bucket String
template <int BSTRING_BUCKETS = BSTRING_BUCKET_MAX>
struct BString {
    u64 length = 0;
    u64 bucket_count = 0;
    Bucket bucket[BSTRING_BUCKETS];
};

template <int BSTRING_BUCKETS>
void
BString_Print(
    const BString<BSTRING_BUCKETS> &s_data
) {
    FOR(s_data.bucket_count, index) {
        auto bucket = s_data.bucket[index];
        printf("%.*s", bucket.size, bucket.ptr);
    }
}

template <int BSTRING_BUCKETS>
void
BString_PrintLine(
    const BString<BSTRING_BUCKETS> &s_data
) {
    BString_Print(s_data);
    printf("\n");
}

template <int BSTRING_BUCKETS>
String
BString_CombineData(
    const BString<BSTRING_BUCKETS> &s_data
) {
    auto mem = (const char *)MemoryArena_Alloc(context.arena, s_data.length);
    auto mem_it = (char *)(mem);

    FOR(s_data.bucket_count, index) {
        auto bucket = s_data.bucket[index];
        Memory_Copy(mem_it, bucket.ptr, bucket.size);
        mem_it += bucket.size;
    }

    return S(mem, s_data.length);
}

template <int BSTRING_BUCKETS>
void
BString_Update(
    BString<BSTRING_BUCKETS> &s_data,
    u64 bucket_index,
    const String &s_append
) {
    Assert(bucket_index < s_data.bucket_count);

    auto &bucket = s_data.bucket[bucket_index];
    s_data.length -= bucket.size;
    bucket.size = String_GetLength(s_append);

    bucket.is_reference = s_append.is_reference;

    if (s_append.is_reference) {
        bucket.ptr = (void *)(s_append.value);
    }
    else {
        bucket.ptr = MemoryArena_Alloc(context.arena, bucket.size);
        Memory_Copy(bucket.ptr, s_append.value, bucket.size);
    }

    s_data.length += bucket.size;
}

template <int BSTRING_BUCKETS>
void
BString_Append(
    BString<BSTRING_BUCKETS> &s_data,
    const String &s_append
) {
    Assert(s_data.bucket_count + 1 <= ARRAY_COUNT(s_data.bucket));

    ++s_data.bucket_count;
    BString_Update(s_data, s_data.bucket_count - 1, s_append);
}
