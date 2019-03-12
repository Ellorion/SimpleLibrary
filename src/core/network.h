#pragma once

/// Server: socket() -> bind() -> listen() -> accept() |-> send()/recv() -> close()
/// Client: socket() -> connect()                      |-> send()/recv() -> close()

struct Network {
	SOCKET socket;
};

instant bool
Network_IsSocketValid(
	Network *network
) {
	Assert(network);

	return (network->socket != INVALID_SOCKET);
}

instant void
Network_Close(
	Network *network_out
) {
	Assert(network_out);

	s32 result = closesocket(network_out->socket);

	if (result == SOCKET_ERROR)
		LOG_ERROR("closesocket: " << WSAGetLastError());

	*network_out = {};
	network_out->socket = INVALID_SOCKET;
}

instant Network
Network_Create(
) {
	Network network = {};
	network.socket = INVALID_SOCKET;

	WSADATA winsock_data;
	if (WSAStartup(MAKEWORD(2, 2), &winsock_data) != NO_ERROR) {
		LOG_ERROR("Winsock initialisation failed.");
		return network;
	}

	network.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (network.socket == INVALID_SOCKET)
		LOG_ERROR("Network socket is invalid.");

	return network;
}

instant Network
Network_Listen(
	u16 port
) {
	Network network = Network_Create();

	sockaddr_in socket_addr;
	socket_addr.sin_family      = AF_INET;
	socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socket_addr.sin_port        = htons(port);

	s32 result = bind(network.socket, (SOCKADDR *)&socket_addr, sizeof(socket_addr));
	if (result == SOCKET_ERROR) {
		LOG_ERROR("Network binding failed: " << WSAGetLastError());

		Network_Close(&network);
		return network;
	}

	if (listen(network.socket, SOMAXCONN) == SOCKET_ERROR) {
		LOG_ERROR("Network socket listening failed: " << WSAGetLastError());

		Network_Close(&network);
		return network;
	}

	LOG_INFO("Network listening to port: " << port);

	return network;
}

instant Network
Network_Connect(
	const char* c_ip_addres,
	u16 port
) {
	Assert(c_ip_addres);

	sockaddr_in socket_addr;
	socket_addr.sin_family      = AF_INET;
	socket_addr.sin_addr.s_addr = inet_addr(c_ip_addres);
	socket_addr.sin_port        = htons(port);

	Network network = Network_Create();

	s32 result = connect(network.socket, (SOCKADDR *)&socket_addr, sizeof(socket_addr));

	if (result == SOCKET_ERROR) {
		LOG_ERROR("Network socket connection failed: " << WSAGetLastError());
		Network_Close(&network);
	}

	return network;
}

instant void
Network_WaitForConnection(
	Network *network_listener,
	Network *network_connection_out
) {
	Assert(network_listener);
	Assert(network_connection_out);

	if (!Network_IsSocketValid(network_listener))
		return;

	*network_connection_out = {};
	network_connection_out->socket = accept(network_listener->socket, 0, 0);

    if (Network_IsSocketValid(network_connection_out))
		LOG_INFO("New socket connection established: " << network_connection_out->socket);
	else
		LOG_WARNING("New socket connection failed: " << network_connection_out->socket);
}

instant void
Network_Send(
	Network *network,
	String s_data
) {
	Assert(network);

	if (!Network_IsSocketValid(network))
		AssertMessage(false, "Invalid network socket [send].");

	send(network->socket, s_data.value, s_data.length, 0);
}

instant s32
Network_Receive(
	Network *network,
	String *s_data_io
) {
	Assert(network);
	Assert(s_data_io);
	Assert(s_data_io->length);

	if (!Network_IsSocketValid(network))
		AssertMessage(false, "Invalid network socket [recv].");

	return recv(network->socket, s_data_io->value, s_data_io->length, 0);
}
