#pragma once

Array<u32> Prime_Fill(u32 max) {
    Array<u32> a_prime;

    if (max+1 > max)
        ++max;

    Array_Reserve(a_prime, max);

    auto isUnknownPrime = [&](Array<u32> a_prime, u32 x) -> bool {
        u32 q = sqrt(x);

        FOR_ARRAY(a_prime, it) {
            auto i = ARRAY_IT(a_prime, it);

            if (i>q)
                break;

            if((x%i)==0) [[unlikely]]
                return false;
        }

        return true;
    };

    FOR_START_AUTO(10u, max, it) {
        ++it;

        if (!(it % 3))
            continue;

        if (!(it % 5))
            continue;

        if (!(it % 7)) [[unlikely]]
            continue;

        if (isUnknownPrime(a_prime, it))
            Array_Add(a_prime, it);
    }

    if (max > 0) Array_Add(a_prime, 1u);
    if (max > 1) Array_Add(a_prime, 2u);
    if (max > 2) Array_Add(a_prime, 3u);
    if (max > 4) Array_Add(a_prime, 5u);
    if (max > 6) Array_Add(a_prime, 7u);

    return a_prime;
}
