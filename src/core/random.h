#pragma once

// xorshift64s
instant u64
Random_Get() {
    static u64 seed = time(0);

    auto x = seed;
	x ^= x >> 12; // a
	x ^= x << 25; // b
	x ^= x >> 27; // c
	seed = x;
	return x * 0x2545F4914F6CDD1DULL;
}
