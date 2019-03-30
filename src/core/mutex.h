#pragma once

struct Mutex_Ticket {
	s64 ticket  = 0;
	s64 serving = 0;
};

instant void
Mutex_Begin(
	Mutex_Ticket *mutex
) {
	Assert(mutex);

	s64 ticket = InterlockedIncrement(&mutex->ticket);

	while(ticket != mutex->serving)
		Sleep(1);
}

instant void
Mutex_End(
	Mutex_Ticket *mutex
) {
	Assert(mutex);

	mutex->serving = InterlockedIncrement(&mutex->serving);
}
