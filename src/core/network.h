#pragma once

/// Server: socket() -> bind() -> listen() -> accept() |-> send()/recv() -> close()
/// Client: socket() -> connect()                      |-> send()/recv() -> close()

enum NETWORK_HTTP_STAGE {
	NETWORK_HTTP_STAGE_IDLE = 0,
	NETWORK_HTTP_STAGE_REQUESTED,
	NETWORK_HTTP_STAGE_RESPONSED_HEADER,
	NETWORK_HTTP_STAGE_RESPONSED_DATA,
	NETWORK_HTTP_STAGE_ERROR,
};

struct Network {
	SOCKET socket = INVALID_SOCKET;
	String s_error;

	/// @Note: does not support SSL
	struct HTTP {
		u16    packet_size    = 1024;
		u16    response_code  = 0;
		s32    header_size    = 0;
		s32    bytes_received = 0;
		String s_buffer_chunk;

		NETWORK_HTTP_STAGE stage = NETWORK_HTTP_STAGE_IDLE;
		bool   is_receiving   = false;
	} HTTP;
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
Network_HasError(
	Network *network
) {
	Assert(network);

	return (   network->HTTP.stage == NETWORK_HTTP_STAGE_ERROR
			OR network->s_error.length);
}

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

	if (result == SOCKET_ERROR) {
		String_Overwrite(&network_out->s_error, S("closesocket error."));
	}

	network_out->HTTP = {};
	network_out->socket = INVALID_SOCKET;
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
Network_Destroy(
	Network *network_out
) {
	Assert(network_out);

	Network_Close(network_out);
	String_Destroy(&network_out->s_error);
	*network_out = {};
}

instant Network
Network_Create(
) {
	Network network = {};

	if (!Network_Init())
		return network;

	network.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (network.socket == INVALID_SOCKET)
		String_Overwrite(&network.s_error, S("Network socket is invalid."));

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
		String_Overwrite(&network.s_error, S("Network binding failed."));
		return network;
	}

	if (listen(network.socket, SOMAXCONN) == SOCKET_ERROR) {
		String_Overwrite(&network.s_error, S("Network socket listening failed."));
		return network;
	}

	LOG_INFO("Network listening to port: " << port);

	return network;
}

instant String
Network_GetIPByName(
	String s_name
) {
	String s_ip;

	hostent *host = gethostbyname(s_name.value);

	if (!host)
		return s_ip;

	char *c_ip = inet_ntoa(*(in_addr *)host->h_addr);

	/// not by reference
	/// the return struct takes ownership
	/// also includes 0-terminator
	s_ip.value   = c_ip;
	s_ip.length  = String_GetLength(c_ip) + 1;
	s_ip.changed = true;

	return s_ip;
}

instant Network
Network_Connect(
	String s_host_adress,
	u16 port
) {
	Network network = Network_Create();

	if (Network_HasError(&network)) {
		return network;
	}

 	String s_ip_address = Network_GetIPByName(s_host_adress);

	if (String_IsEmpty(&s_ip_address)) {
		String_Overwrite(&network.s_error, S("IP loopup failed."));
		return network;
	}

	sockaddr_in socket_addr;
	socket_addr.sin_family      = AF_INET;
	socket_addr.sin_addr.s_addr = inet_addr(s_ip_address.value);
	socket_addr.sin_port        = htons(port);

	s32 result = connect(network.socket, (SOCKADDR *)&socket_addr, sizeof(socket_addr));

	if (result == SOCKET_ERROR) {
		String_Overwrite(&network.s_error, S("Network socket connection failed."));
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

instant bool
Network_Send(
	Network *network,
	String s_data
) {
	Assert(network);

	if (!Network_IsSocketValid(network)) {
		String_Overwrite(&network->s_error, S("Invalid network socket [send].\n\tForgot to connect to a host?"));
		return false;
	}

	int result = send(network->socket, s_data.value, s_data.length, 0);

	if (result == SOCKET_ERROR) {
		String_Overwrite(&network->s_error, S("Socket error after data was send. [send]"));
	}

	return (result != SOCKET_ERROR);
}

instant s32
Network_Receive(
	Network *network,
	String  *s_buffer_out
) {
	Assert(network);
	Assert(s_buffer_out);
	Assert(s_buffer_out->length);

	if (!Network_IsSocketValid(network)) {
		String_Overwrite(&network->s_error, S("Invalid network socket [recv].\n\tForgot to connect to a host?"));
		return 0;
	}

	return recv(network->socket, s_buffer_out->value, s_buffer_out->length, 0);
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

instant bool
Network_HTTP_Request(
	Network *network,
	String   s_ip,
	String   s_path
) {
	Assert(network);

	if (!Network_IsSocketValid(network)) {
		*network = Network_Connect(s_ip, 80);

		if (Network_HasError(network))
			return false;
	}

	if (   network->HTTP.stage == NETWORK_HTTP_STAGE_RESPONSED_HEADER
		OR network->HTTP.stage == NETWORK_HTTP_STAGE_RESPONSED_DATA
	) {
		return false;
	}

	network->HTTP.stage = NETWORK_HTTP_STAGE_REQUESTED;
	String_Clear(&network->s_error);

	String s_request;
	String_Append(&s_request, S("GET /"));
	String_Append(&s_request, s_path);
	String_Append(&s_request, S(" HTTP/1.1\r\n"));
	String_Append(&s_request, S("Host: "));
	String_Append(&s_request, s_ip);
	String_Append(&s_request, S("\r\n"));

	String_Append(&s_request, S("\r\n\r\n"));

	return Network_Send(network, s_request);
}

instant bool
Network_HTTP_GetResponseRef(
	Network *network,
	String  *s_response_out
) {
	Assert(network);
	Assert(s_response_out);

	if (Network_HasError(network))
		return false;

	if (!(   network->HTTP.stage == NETWORK_HTTP_STAGE_REQUESTED
		  OR network->HTTP.stage == NETWORK_HTTP_STAGE_RESPONSED_HEADER
		  OR network->HTTP.stage == NETWORK_HTTP_STAGE_RESPONSED_DATA)
	) {
		return false;
	}

	if (network->HTTP.s_buffer_chunk.length < network->HTTP.packet_size)
		String_CreateBuffer(&network->HTTP.s_buffer_chunk, network->HTTP.packet_size, false);

	/// header
	/// =======================================================================
	if (!network->HTTP.is_receiving) {
		bool result = false;

		/// get http response header
		network->HTTP.bytes_received = Network_Receive(network, &network->HTTP.s_buffer_chunk);

		if (network->HTTP.bytes_received < 0) {
			network->HTTP.stage = NETWORK_HTTP_STAGE_ERROR;
			String_Overwrite(&network->s_error, S("No response recieved."));
			return false;
		}

		String s_header_end_id = S("\r\n\r\n");
		network->HTTP.header_size = String_IndexOf(&network->HTTP.s_buffer_chunk, s_header_end_id, 0, true);

		/// in case it was not byRef and had existing data
		String_Destroy(s_response_out);

		if (network->HTTP.header_size < 0) {
			network->HTTP.stage = NETWORK_HTTP_STAGE_ERROR;
			String_Overwrite(&network->s_error, S("No http header recieved."));
			return false;
		}

		network->HTTP.header_size += s_header_end_id.length;

		/// return http header
		*s_response_out = S(network->HTTP.s_buffer_chunk, network->HTTP.header_size);

		String s_http_code = String_GetDelimiterSectionRef(s_response_out, S(" "), 1, true);
		network->HTTP.response_code = ToInt(s_http_code);

		switch (network->HTTP.response_code) {
			/// Moved Permanently
			case 301: {
				Parser parser_header = Parser_Load(*s_response_out, true);

				String s_data;
				while(Parser_IsRunning(&parser_header)) {
					Parser_GetStringRef(&parser_header, &s_data, PARSER_MODE_SEEK, false);

					if (s_data == "Location:") {
						/// return already parsed redirecting url instead of full header
						Parser_GetStringRef(&parser_header, &s_data, PARSER_MODE_SEEK, false);
						/// since both strings use a reference to network->HTTP.s_buffer_chunk,
						/// it can be overwritten without any memory leak,
						/// and the string buffer size also stays intact
						*s_response_out = s_data;
						break;
					}
				}
			} break;

			/// everything is awesome
			case 200: {
				result = true;
			} break;

			default: {
				network->HTTP.stage = NETWORK_HTTP_STAGE_ERROR;
				String_Overwrite(&network->s_error, S("HTTP response code was not 200."));
				return false;
			} break;
		}

		network->HTTP.stage = NETWORK_HTTP_STAGE_RESPONSED_HEADER;
		network->HTTP.is_receiving = true;

		return result;
	}

	/// response data / content chunks
	/// =======================================================================
	/// error checking
	if (network->HTTP.response_code != 200) {
		network->HTTP.stage = NETWORK_HTTP_STAGE_ERROR;
		String_Overwrite(&network->s_error, S("HTTP response code was not 200."));
		return false;
	}

	/// get content chunk, which was sent with the header,
	/// instead of requesting more (needed) chunks prematurely
	/// -----------------------------------------------------------------------
	if (    network->HTTP.stage == NETWORK_HTTP_STAGE_RESPONSED_HEADER
		AND network->HTTP.bytes_received > network->HTTP.header_size
	) {
		String s_remaining_chunk = S(network->HTTP.s_buffer_chunk);
		String_AddOffset(&s_remaining_chunk, network->HTTP.header_size);

		String_Destroy(s_response_out);
		*s_response_out = s_remaining_chunk;

		network->HTTP.stage = NETWORK_HTTP_STAGE_RESPONSED_DATA;

		return true;
	}

	/// receive remaining chunks
	/// -----------------------------------------------------------------------
	if (network->HTTP.stage != NETWORK_HTTP_STAGE_RESPONSED_HEADER) {
		/// received last chunk before, if buffer was not fully filled
		network->HTTP.is_receiving = ((u64)network->HTTP.bytes_received == network->HTTP.s_buffer_chunk.length);
	}

	if (network->HTTP.is_receiving) {
		network->HTTP.stage = NETWORK_HTTP_STAGE_RESPONSED_DATA;

		network->HTTP.bytes_received = Network_Receive(network, &network->HTTP.s_buffer_chunk);

		if (network->HTTP.bytes_received <= 0) {
			network->HTTP.stage = NETWORK_HTTP_STAGE_ERROR;
			String_Overwrite(&network->s_error, S("Receiving remaining data chunks failed."));
			return false;
		}

		String_Destroy(s_response_out);
		*s_response_out = S(network->HTTP.s_buffer_chunk, network->HTTP.bytes_received);

		return true;
	}

	network->HTTP.stage = NETWORK_HTTP_STAGE_IDLE;
	return false;
}
