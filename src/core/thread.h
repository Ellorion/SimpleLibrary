#pragma once

typedef u64 WINAPI (*Thread_Function)(void *data);

struct Thread {
	HANDLE handle        = 0;
	u64 id               = 0;
	void *data           = 0;
	Thread_Function func = 0;
};

instant Thread
Thread_Create(
	void *data,
	Thread_Function thread_function
) {
	Thread thread = {};
	thread.data = data;
	thread.func = thread_function;

	return thread;
}

instant void
Thread_Execute(
	Thread *thread_io
) {
    Assert( thread_io);
    Assert(!thread_io->handle);
    Assert(!thread_io->id);
    Assert( thread_io->func);

    thread_io->handle = CreateThread(
							0,
							0,
							thread_io->func,
							thread_io->data,
							0,
							&thread_io->id
						);
}

instant void
Thread_WaitFor(
	Thread *thread
) {
	Assert(thread);
	Assert(thread->handle);

	WaitForMultipleObjects(1, &thread->handle, TRUE, INFINITE);
}
