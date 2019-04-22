#pragma once

instant void
Random_Init(
	u16 seed = 0
) {
	if (seed)
		srand(seed);
	else
		srand(time(0));
}

instant s32
Random_Get(
	s32 min,
	s32 max
) {
	if (min > max)
		SWAP(s32, &min, &max);

	s32 value = min + (rand() % (max - min));

	return value;
}
