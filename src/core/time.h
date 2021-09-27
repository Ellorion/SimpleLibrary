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
	Timer &timer_out
) {
	timer_out.lo_timer = GetTickCount();

	LARGE_INTEGER largeCounter;
	QueryPerformanceCounter(&largeCounter);

	timer_out.hi_timer = largeCounter.QuadPart;
	timer_out.counter = 0;
}

/// milliseconds
instant double
Time_Measure(
	Timer &timer,
	bool restart_after_measuring
) {
	static double perfFreq = 0.0;

	if (!perfFreq) {
        LARGE_INTEGER largeFreq;
        QueryPerformanceFrequency(&largeFreq);
        perfFreq = largeFreq.QuadPart / 1000.0;
	}

	if (!timer.hi_timer) {
        Time_Reset(timer);
        return 0.0;
	}

	LARGE_INTEGER largeCounter;
	QueryPerformanceCounter(&largeCounter);

	u64 current_time = largeCounter.QuadPart;

	double diff = (current_time - timer.hi_timer) / perfFreq;

	if (restart_after_measuring) {
 		timer.hi_timer = current_time;
	}

	return diff;
}

instant bool
Time_HasElapsed(
	Timer &timer,
	u32 interval_in_ms,
	bool run_once = false
) {
	bool result = Time_Measure(timer, false) >= interval_in_ms;

	if (result) {
		Time_Reset(timer);
	}

	return result;
}

instant u32
Time_Get() {
	return GetTickCount();
}

/// has to be used every frame or the calculation will be wrong
instant u32
Time_GetFPS(
	Timer &timer,
	bool get_worst = false
) {
	u32 diff = Time_Get() - timer.lo_timer;

	if (timer.lo_timer > 0 AND diff < 1000) {
		++timer.counter;
	}
	else {
		timer.fps_worst = MIN(timer.fps_worst,
                              timer.counter);

		if (timer.fps_worst == 0)
			timer.fps_worst = timer.counter;

		timer.fps = timer.counter;
		timer.counter = 1;
		timer.lo_timer = Time_Get();
	}

	return (get_worst ? timer.fps_worst : timer.fps);
}

/// Reach the end (from the start) in the timespan of the duration
/// Returns: step increase
instant float
Time_Move(
	Timer &timer,
	u32 timespan_in_ms,
	float distance
) {
	if (!timespan_in_ms) {
        return 0.0f;
	}

	double step_size = Time_Measure(timer, true) / timespan_in_ms;
	return distance * step_size;
}
