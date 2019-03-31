#pragma once

struct Mutex_Ticket {
	s64 ticket  = 0;
	s64 serving = 0;
};

instant void
Mutex_Lock(
	Mutex_Ticket *mutex
) {
	Assert(mutex);

	s64 ticket = InterlockedIncrement(&mutex->ticket);

	while(ticket > mutex->serving)
		Sleep(1);
}

instant void
Mutex_LockUntil(
	Mutex_Ticket *mutex,
	s64 serving
) {
	Assert(mutex);

	while(serving != mutex->serving)
		Sleep(1);
}

instant void
Mutex_Release(
	Mutex_Ticket *mutex
) {
	Assert(mutex);

	if (mutex->serving < mutex->ticket)
		mutex->serving = InterlockedIncrement(&mutex->serving);
	else
		LOG_WARNING("Mutex_Release before Mutex_Lock.");
}

instant void
Mutex_Reset(
	Mutex_Ticket *mutex
) {
	Assert(mutex);

	InterlockedExchange(&mutex->ticket , 0);
	InterlockedExchange(&mutex->serving, 0);
}

