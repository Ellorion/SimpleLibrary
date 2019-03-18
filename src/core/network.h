#pragma once

/// Server: socket() -> bind() -> listen() -> accept() |-> send()/recv() -> close()
/// Client: socket() -> connect()                      |-> send()/recv() -> close()

struct Network {
	SOCKET socket;
};

struct Network_Info {
	String s_name_device;
	String s_name_adapter;
	String s_ip;
	String s_ip_mask;
	String s_gateway_ip;
	String s_gateway_mask;
	String s_mac;
};

instant bool
Network_Init(
) {
	WSADATA winsock_data;

	if (WSAStartup(MAKEWORD(2, 2), &winsock_data) != NO_ERROR) {
		LOG_ERROR("Winsock initialisation failed.");
		return false;
	}

	return true;
}

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

	Network_Init();

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

instant String
Network_GetName(
	const char *c_ip_address
) {
	Assert(c_ip_address);

	IPAddr   ip_address = inet_addr(c_ip_address);
	hostent *dest_info  = gethostbyaddr((char *)&ip_address, 4, AF_INET);

	String s_result;

 	if (dest_info)
		s_result = String_Copy(dest_info->h_name);

	return s_result;
}

instant String
Network_GetName(
) {
	char c_buffer[1024];

	String s_result;

	if(gethostname(c_buffer, sizeof(c_buffer)) != SOCKET_ERROR) {
		s_result = String_Copy(c_buffer, String_GetLength(c_buffer));
		String_Append(&s_result, S("\0", 1));
	}

	return s_result;
}

instant String
Network_GetIPByName(
	String s_name
) {
	String s_ip;

	if (String_EndWith(&s_name, S("\0", 1), false)) {
		hostent *host = gethostbyname(s_name.value);

		char *c_ip = inet_ntoa(*(in_addr *)host->h_addr);

		/// not by reference
		/// the return struct takes ownership
		/// also includes 0-terminator
		s_ip.value   = c_ip;
		s_ip.length  = String_GetLength(c_ip) + 1;
		s_ip.changed = true;
	}

	return s_ip;
}

instant String
Network_ConvertToMAC(
	u8  *mac_address,
	u64  mac_address_length
) {
	String s_result;

	char* mac_address_it = (char *)mac_address;
	char  c_hex[2];

	FOR(mac_address_length, it) {
		ToHex(*mac_address_it, &c_hex[0], &c_hex[1]);

		String_Append(&s_result, S((char *)&c_hex[0], 1));
		String_Append(&s_result, S((char *)&c_hex[1], 1));

		if (it < mac_address_length - 1)
			String_Append(&s_result, S("-", 1));

		++mac_address_it;
	}

	return s_result;
}

instant String
Network_GetMAC(
	String s_ip_address
) {
	String s_result;

	if (!String_EndWith(&s_ip_address, S("\0", 1), false))
		return s_result;

    u64 mac_address[2];
    u64 mac_address_len = 6;

    IPAddr ip_address = inet_addr(s_ip_address.value);

    s32 request = SendARP(ip_address, 0, mac_address, &mac_address_len);

    if (request == NO_ERROR AND mac_address_len) {
        s_result = Network_ConvertToMAC((u8 *)&mac_address, mac_address_len);
    }

	return s_result;
}

instant Network_Info
Network_GetInfo(
) {
	Network_Info info;
	info.s_name_device = Network_GetName();
	info.s_ip          = Network_GetIPByName(info.s_name_device);
	info.s_mac         = Network_GetMAC(info.s_ip);

	u64 len_buffer;
	GetAdaptersInfo(0, &len_buffer);

	IP_ADAPTER_INFO *adapter_info = (IP_ADAPTER_INFO *)Memory_Create(char, len_buffer);
	IP_ADAPTER_INFO *adapter_info_it;

	GetAdaptersInfo(adapter_info, &len_buffer);

	adapter_info_it = adapter_info;

	while(adapter_info_it) {
		String s_adapter_mac = Network_ConvertToMAC(adapter_info_it->Address, adapter_info_it->AddressLength);

		if (!String_IsEqual(info.s_mac, s_adapter_mac)) {
			adapter_info_it = adapter_info_it->Next;
			String_Destroy(&s_adapter_mac);
			continue;
		}

		info.s_name_adapter = String_Copy(adapter_info_it->Description);
		info.s_ip_mask      = String_Copy(adapter_info_it->IpAddressList.IpMask.String);
		info.s_gateway_ip   = String_Copy(adapter_info_it->GatewayList.IpAddress.String);
		info.s_gateway_mask = String_Copy(adapter_info_it->GatewayList.IpMask.String);

		String_Append(&info.s_gateway_ip, S("\0", 1));

		String_Destroy(&s_adapter_mac);
		break;
	}

	Memory_Free(adapter_info);

	return info;
}

instant void
Network_DestroyInfo(
	Network_Info *info
) {
	Assert(info);

	String_Destroy(&info->s_gateway_ip);
	String_Destroy(&info->s_gateway_mask);
	free(info->s_ip.value);
	String_Destroy(&info->s_ip_mask);
	String_Destroy(&info->s_mac);
	String_Destroy(&info->s_name_adapter);
	String_Destroy(&info->s_name_device);

	info->s_ip = {};
	info->s_ip.changed = true;
}

instant void
Network_PrintInfo(
	Network_Info *info
) {
	Assert(info);

	String_Print(S("Name:\t\t"));
	String_PrintLine(info->s_name_device);

	String_Print(S("IP:\t\t"));
	String_Print(info->s_ip);
	String_Print(S("- "));
	String_PrintLine(info->s_ip_mask);

	String_Print(S("MAC:\t\t"));
	String_PrintLine(info->s_mac);

	String_Print(S("Adapter:\t"));
	String_PrintLine(info->s_name_adapter);

	String_Print(S("Gateway:\t"));
	String_Print(info->s_gateway_ip);
	String_Print(S("- "));
	String_PrintLine(info->s_gateway_mask);
}
