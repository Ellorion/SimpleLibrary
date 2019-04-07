#include "src/SLib.h"
#include "test/core.h"

instant u64 WINAPI
Listener_Thread(
	void *data
) {
	u16 port = *(u16 *)data;
	Network network_listener = Network_Listen(port);

	String s_data;
	String_CreateBuffer(&s_data, 1024, false);

	Network socket_connection;
	Network_WaitForConnection(&network_listener, &socket_connection);

	u32 recv;
	while ((recv = Network_Receive(&socket_connection, &s_data, false))) {
		String_PrintLine(s_data, recv);
	}

	Network_Destroy(&socket_connection);

	return 0;
};

int main() {
	u16 port = 80;

	Thread thread = Thread_Create(&port, Listener_Thread);
	Thread_Execute(&thread);

	Network network = Network_Connect(S("127.0.0.1"), port);

	u32 counter = 5;

	while(counter) {
		Network_Send(&network, S("Hello, World!"));
		Sleep(100);
		--counter;
	}

	Network_Destroy(&network);

	Thread_WaitFor(&thread);

	return 0;
}
