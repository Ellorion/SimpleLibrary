#pragma once

struct Timer {
	u32 lo_timer  = 0;
	u64 hi_timer  = 0;
	u32 counter   = 0;
	u32 fps       = 0;
	u32 fps_worst = 0;
};

instant void
Time_Reset(
	Timer *timer_out
) {
	if (!timer_out)
		return;

	timer_out->lo_timer = GetTickCount();

	LARGE_INTEGER largeCounter;
	QueryPerformanceCounter(&largeCounter);

	timer_out->hi_timer = largeCounter.QuadPart;
	timer_out->counter = 0;
}

instant u32
Time_Get() {
	return GetTickCount();
}

instant bool
Time_HasElapsed(
	Timer *timer_io,
	u32 interval_in_ms,
	bool run_once = false
) {
	Assert(timer_io);

	if (!timer_io->lo_timer) {
		Time_Reset(timer_io);
		return false;
	}

	bool result = true;

	if (Time_Get() - timer_io->lo_timer < interval_in_ms) {
		result = false;
	}
	else {
		if (!run_once) {
			timer_io->lo_timer = Time_Get();
		}
	}

	return result;
}

/// milliseconds
instant double
Time_Measure(
	Timer *timer_io
) {
	if (!timer_io)
		return 0.f;

	LARGE_INTEGER largeCounter;
	QueryPerformanceCounter(&largeCounter);

	/// init timer
	if (timer_io->hi_timer == 0) {
		timer_io->hi_timer = largeCounter.QuadPart;
		return 0.f;
	}

	u64 current_time = largeCounter.QuadPart;

	LARGE_INTEGER largeFreq;
	QueryPerformanceFrequency(&largeFreq);

	double diff = ((double)( (current_time - timer_io->hi_timer)
							* 1000.0)
							/ largeFreq.QuadPart);

	timer_io->hi_timer = current_time;

	return diff;
}

/// has to be used every frame or the calculation will be wrong
instant u32
Time_GetFPS(
	Timer *timer_io,
	bool get_worst = false
) {
	if (!timer_io)
		return 0;

	u32 diff = Time_Get() - timer_io->lo_timer;

	if (timer_io->lo_timer > 0 AND diff < 1000) {
		++timer_io->counter;
	}
	else {
		timer_io->fps_worst = MIN(	timer_io->fps_worst,
									timer_io->counter);

		if (timer_io->fps_worst == 0)
			timer_io->fps_worst = timer_io->counter;

		timer_io->fps = timer_io->counter;
		timer_io->counter = 1;
		timer_io->lo_timer = Time_Get();
	}

	return (get_worst ? timer_io->fps_worst : timer_io->fps);
}

/// Reach the end (from the start) in the timespan of the duration
/// Returns: step increase
instant float
Time_Move(
	Timer *timer_io,
	u32 timespan_in_ms,
	float distance
) {
	Assert(timer_io);

	if (!timespan_in_ms)  return 0.0f;

	double step_size = Time_Measure(timer_io) / timespan_in_ms;
	return distance * step_size;
}
