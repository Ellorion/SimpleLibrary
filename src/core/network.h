#pragma once

/// Server: socket() -> bind() -> listen() -> accept() |-> send()/recv() -> close()
/// Client: socket() -> connect()                      |-> send()/recv() -> close()

enum SOCKET_TYPE {
	SOCKET_TCP,
	SOCKET_UDP
};

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
		u16    packet_size     = 1024;
		u16    response_code   = 0;
		s32    header_size     = 0;
		s32    bytes_receiving = 0;
		s64    bytes_received  = 0;
		s64    content_length  = 0;
		String s_buffer_chunk;
		String s_credentials;

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

struct Network_HTTP_URI {
	String s_protocol;
	String s_credentials_plain; /// not base64 encoded
	String s_domain;
	String s_path;
	String s_error;

	u16 port;
};

instant void
Network_HTTP_DestroyURI(
	Network_HTTP_URI *uri
) {
    Assert(uri);

    Assert(!uri->s_error.value OR uri->s_error.is_reference);

    String_Destroy(&uri->s_credentials_plain);
    String_Destroy(&uri->s_domain);
    String_Destroy(&uri->s_path);
    String_Destroy(&uri->s_protocol);

    *uri = {};
}

instant Network_HTTP_URI
Network_HTTP_ParseURL(
    String s_url
) {
	String ts_url = S(s_url);

	String s_seperator_protocol    = S("://");
	String s_seperator_credentials = S("@");
	String s_seperator_domain      = S("/");
	String s_seperator_port        = S(":");

	s64 index_find;
	Network_HTTP_URI uri;

	/// check availability
	/// -----------------------------------------------------------------------
	if (String_IsEmpty(&ts_url, true)) {
		uri.s_error = S("No URL was assigned.");
		return uri;
	}

	/// check protocol
	/// -----------------------------------------------------------------------
	index_find = String_IndexOf(&ts_url, s_seperator_protocol, 0, true);

	if (index_find > 0) {
		if (!String_StartWith(&ts_url, S("http"), false)) {
			uri.s_error = S("Only http protocol is supported.");
			return uri;
		}

		uri.s_protocol = S(ts_url, index_find);

		index_find += s_seperator_protocol.length;

		String_AddOffset(&ts_url, index_find);
	}

	/// check credentials
	/// -----------------------------------------------------------------------
	index_find = String_IndexOf(&ts_url, s_seperator_credentials, 0, true);

	if (index_find > 0) {
		uri.s_credentials_plain = S(ts_url, index_find);
		String_AddOffset(&ts_url, index_find + s_seperator_credentials.length);
	}

	/// check domain
	/// -----------------------------------------------------------------------
	index_find = String_IndexOf(&ts_url, s_seperator_domain, 0, true);

	if (index_find < 0)
		index_find = ts_url.length;

	uri.s_domain = S(ts_url, index_find);
	String_AddOffset(&ts_url, index_find);

	if (String_IsEmpty(&uri.s_domain, true)) {
		uri.s_error = S("Invalid URL used for parsing into URI.");
		return uri;
	}

	/// check port
	/// -----------------------------------------------------------------------
	index_find = String_IndexOf(&uri.s_domain, s_seperator_port, 0, true);

	if (index_find > 0) {
		String s_port = S(uri.s_domain);
		String_AddOffset(&s_port, index_find + s_seperator_port.length);
		uri.port = ToInt(s_port);

		uri.s_domain.length = index_find;
	}
	else {
		/// default port for http
		uri.port = 80;
	}

	/// remaining path
	/// -----------------------------------------------------------------------
	uri.s_path = ts_url;

	return uri;
}

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
		network_out->s_error = S("closesocket error.");
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
	info->s_ip.has_changed = true;
}

instant void
Network_Destroy(
	Network *network_out
) {
	Assert(network_out);
	Assert(!network_out->s_error.value OR network_out->s_error.is_reference);

	Network_Close(network_out);
	network_out->HTTP = {};
}

instant Network
Network_Create(
	SOCKET_TYPE type
) {
	Network network = {};

	if (!Network_Init())
		return network;

	switch (type) {
		case SOCKET_TCP: {
			network.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		} break;

		case SOCKET_UDP: {
			network.socket = socket(AF_INET, SOCK_DGRAM, 0);
		} break;

		default: {
			AssertMessage(false, "Unknown socket type.");
		}
	}

	if (network.socket == INVALID_SOCKET)
		network.s_error = S("Network socket is invalid.");

	return network;
}

instant Network
Network_Listen(
	u16 port
) {
	Network network = Network_Create(SOCKET_TCP);

	sockaddr_in socket_addr;
	socket_addr.sin_family      = AF_INET;
	socket_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socket_addr.sin_port        = htons(port);

	s32 result = bind(network.socket, (SOCKADDR *)&socket_addr, sizeof(socket_addr));

	if (result == SOCKET_ERROR) {
		network.s_error = S("Network binding failed.");
		return network;
	}

	if (listen(network.socket, SOMAXCONN) == SOCKET_ERROR) {
		network.s_error = S("Network socket listening failed.");
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

	String ts_name;
	String_Append(&ts_name, s_name);
	String_Append(&ts_name, S("\0", 1));

	hostent *host = gethostbyname(ts_name.value);

	String_Destroy(&ts_name);

	if (!host)
		return s_ip;

	char *c_ip = inet_ntoa(*(in_addr *)host->h_addr);

	/// not by reference
	/// the return struct takes ownership
	/// also includes 0-terminator
	s_ip.value   = c_ip;
	s_ip.length  = String_GetLength(c_ip);
	s_ip.has_changed = true;

	return s_ip;
}

instant Network
Network_Connect(
	SOCKET_TYPE type,
	String s_host_adress,
	u16 port
) {
	Network network = Network_Create(type);

	if (Network_HasError(&network)) {
		return network;
	}

 	String s_ip_address = Network_GetIPByName(s_host_adress);

	if (String_IsEmpty(&s_ip_address)) {
		network.s_error = S("IP loopup failed.");
		return network;
	}

	sockaddr_in socket_addr;
	socket_addr.sin_family      = AF_INET;
	socket_addr.sin_addr.s_addr = inet_addr(s_ip_address.value);
	socket_addr.sin_port        = htons(port);

	LOG_STATUS("Network [" << s_host_adress.value << ":" << port << "] connecting...");

	s32 result = connect(network.socket, (SOCKADDR *)&socket_addr, sizeof(socket_addr));

	if (result == SOCKET_ERROR) {
		network.s_error = S("Network socket connection failed.");
	}
	else {
		LOG_STATUS("Network connected to: " << s_host_adress.value);
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

/// true, if data was sent
instant s32
Network_HTTP_WaitForData(
	Network *network,
	s64 seconds,
	s64 useconds
) {
	Assert(network);

	timeval timeout;
	timeout.tv_sec  = seconds;
	timeout.tv_usec = useconds;

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(network->socket, &fds);

	return select(0, &fds, 0, 0, &timeout);
}

instant bool
Network_Send(
	Network *network,
	String s_data
) {
	Assert(network);

	if (Network_HasError(network))
		return false;

	if (!Network_IsSocketValid(network)) {
		network->s_error = S("Invalid network socket [send].\n\tForgot to connect to a host?");
		return false;
	}

	int result = send(network->socket, s_data.value, s_data.length, 0);

	if (result == SOCKET_ERROR)
		network->s_error = S("Socket error after data was send. [send]");

	return (result != SOCKET_ERROR);
}

instant s32
Network_Receive(
	Network *network,
	String  *s_buffer_out,
	bool is_content_length_known
) {
	Assert(network);
	Assert(s_buffer_out);
	Assert(s_buffer_out->length);

	if (Network_HasError(network))
		return false;

	if (!Network_IsSocketValid(network)) {
		network->s_error = S("Invalid network socket [recv].\n\tForgot to connect to a host?");
		return -1;
	}

	if (!is_content_length_known) {
		/// make sure there is something to read,
		/// if the content length is not known beforehand,
		/// otherwise recv will block the app when no more data arrives
		s32 buffer_data_count = Network_HTTP_WaitForData(network, 1, 0);

		if (buffer_data_count <= 0)
			return buffer_data_count;
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

    ulong mac_address[2];
    ulong mac_address_len = 6;

    IPAddr ip_address = inet_addr(s_ip_address.value);

    auto request = SendARP(ip_address, 0, mac_address, &mac_address_len);

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

	ulong len_buffer;
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
Network_HTTP_HasCredentials(
	Network *network
) {
	Assert(network);

	return (network->HTTP.s_credentials.length > 0);
}

instant bool
Network_HTTP_HasCredentials(
	Network_HTTP_URI *uri
) {
	Assert(uri);

	return (uri->s_credentials_plain.length > 0);
}

instant void
Network_HTTP_ClearCredentials(
	Network *network
) {
	Assert(network);

	String_Clear(&network->HTTP.s_credentials);
}

instant bool
Network_HTTP_SetCredentials(
	Network *network,
	String s_user,
	String s_pass
) {
	Assert(network);

	if (String_IsEmpty(&s_user))
		return false;

	String_Overwrite(&network->HTTP.s_credentials, s_user);
	String_Append(&network->HTTP.s_credentials, S(":", 1));
	String_Append(&network->HTTP.s_credentials, s_pass);

	String s_encoded = Base64_Encode(network->HTTP.s_credentials);
	String_Destroy(&network->HTTP.s_credentials);
	network->HTTP.s_credentials = s_encoded;

	return true;
}

instant bool
Network_HTTP_SetCredentials(
	Network *network,
	String s_user_and_pass
) {
	Assert(network);

	if (String_IsEmpty(&s_user_and_pass))
		return false;

	String s_encoded = Base64_Encode(s_user_and_pass);
	String_Destroy(&network->HTTP.s_credentials);
	network->HTTP.s_credentials = s_encoded;

	return true;
}

/// @Depricated
instant bool
Network_HTTP_Request(
	Network *network,
	String   s_ip,
	String   s_path
) {
	Assert(network);

	if (!Network_IsSocketValid(network)) {
		*network = Network_Connect(SOCKET_TCP, s_ip, 80);

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
	String_Append(&s_request, S(" HTTP/1.1"));
	String_Append(&s_request, S("\r\nHost: "));
	String_Append(&s_request, s_ip);

	if (Network_HTTP_HasCredentials(network)) {
		String_Append(&s_request, S("\r\nAuthorization: Basic "));
		String_Append(&s_request, network->HTTP.s_credentials);
	}

	String_Append(&s_request, S("\r\n\r\n"));

	bool success = Network_Send(network, s_request);

	String_Destroy(&s_request);

	return success;
}

instant bool
Network_HTTP_Request(
	Network *network,
	String   s_url
) {
	Assert(network);

	String_Clear(&network->HTTP.s_credentials);

	/// in case the header file does not deliver that information
	network->HTTP.content_length = 0;
	network->HTTP.bytes_received = 0;

	Network_HTTP_URI uri = Network_HTTP_ParseURL(s_url);

	if (uri.s_error.length) {
		/// since every error message is at a contant memory location,
		/// it is enouth to simply put a reference here
		network->s_error = S(uri.s_error);
		Network_HTTP_DestroyURI(&uri);
		return false;
	}

	if (!Network_IsSocketValid(network)) {
		*network = Network_Connect(SOCKET_TCP, uri.s_domain, uri.port);

		if (Network_HasError(network)) {
			Network_HTTP_DestroyURI(&uri);
			return false;
		}
	}

	if (   network->HTTP.stage == NETWORK_HTTP_STAGE_RESPONSED_HEADER
		OR network->HTTP.stage == NETWORK_HTTP_STAGE_RESPONSED_DATA
	) {
		Network_HTTP_DestroyURI(&uri);
		return false;
	}

	network->HTTP.stage = NETWORK_HTTP_STAGE_REQUESTED;
	network->HTTP.is_receiving = false;
	String_Clear(&network->s_error);

	String s_request;
	String_Append(&s_request, S("GET /"));
	String_Append(&s_request, uri.s_path);
	String_Append(&s_request, S(" HTTP/1.1"));
	String_Append(&s_request, S("\r\nHost: "));
	String_Append(&s_request, uri.s_domain);

	if (Network_HTTP_HasCredentials(&uri)) {
		Network_HTTP_SetCredentials(network, uri.s_credentials_plain);

		String_Append(&s_request, S("\r\nAuthorization: Basic "));
		String_Append(&s_request, network->HTTP.s_credentials);
	}

	String_Append(&s_request, S("\r\n\r\n"));

	Network_HTTP_DestroyURI(&uri);

	bool success = Network_Send(network, s_request);

	String_Destroy(&s_request);

	return success;
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
		network->HTTP.bytes_receiving = Network_Receive(network,
														&network->HTTP.s_buffer_chunk,
														network->HTTP.content_length > 0);

		if (network->HTTP.bytes_receiving <= 0) {
			network->HTTP.stage = NETWORK_HTTP_STAGE_ERROR;
			network->s_error = S("No response recieved.");
			return false;
		}

		String s_header_end_id = S("\r\n\r\n");
		network->HTTP.header_size = String_IndexOf(&network->HTTP.s_buffer_chunk, s_header_end_id, 0, true);

		/// in case it was not byRef and had existing data
		String_Destroy(s_response_out);

		if (network->HTTP.header_size < 0) {
			network->HTTP.stage = NETWORK_HTTP_STAGE_ERROR;
			network->s_error = S("No http header recieved.");
			return false;
		}

		network->HTTP.header_size += s_header_end_id.length;

		/// return http header
		*s_response_out = S(network->HTTP.s_buffer_chunk, network->HTTP.header_size);

		String s_http_code = String_GetDelimiterSectionRef(s_response_out, S(" "), 1, true);
		network->HTTP.response_code = ToInt(s_http_code);

		switch (network->HTTP.response_code) {
			case 200: {		/// everything is awesome
				Parser parser_header = Parser_Load(*s_response_out, S("#"));

				String s_data;
				while(Parser_IsRunning(&parser_header)) {
					Parser_GetStringRef(&parser_header, &s_data, PARSER_MODE_SEEK, false);

					if (s_data == "Content-Length:") {
						/// return already parsed redirecting url instead of full header
						Parser_GetStringRef(&parser_header, &s_data, PARSER_MODE_SEEK, false);
						/// since both strings use a reference to network->HTTP.s_buffer_chunk,
						/// it can be overwritten without any memory leak,
						/// and the string buffer size also stays intact
						network->HTTP.content_length = ToInt(s_data);
						break;
					}
				}

				Parser_Destroy(&parser_header);

				result = true;
			} break;

			case 301:		/// Moved Permanently
			case 302: {		/// Moved Temporarily
				Parser parser_header = Parser_Load(*s_response_out, S("#"));

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

				Parser_Destroy(&parser_header);
			} break;

			case 401: {		/// Unauthorized
				network->HTTP.stage = NETWORK_HTTP_STAGE_ERROR;
				network->s_error = S("Unauthorized access. User/pass is required.");
				return false;
			} break;

			case 404: {		/// Not Found
				network->HTTP.stage = NETWORK_HTTP_STAGE_ERROR;
				network->s_error = S("Requested content is not available on the host.");
				return false;
			} break;
		}

		network->HTTP.stage = NETWORK_HTTP_STAGE_RESPONSED_HEADER;
		network->HTTP.is_receiving = true;

		return result;
	}

	/// response data / content chunks
	/// =======================================================================
	/// for (manually) unhandled response code information
	if (network->HTTP.response_code != 200) {
		switch (network->HTTP.response_code) {
			case 301:
			case 302: {
				network->s_error = S("URL is redirecting to another location.");
			} break;

			default: {
				network->s_error = S("HTTP response code was not 200. Check HTTP header for details.");
			} break;
		}

		network->HTTP.stage = NETWORK_HTTP_STAGE_ERROR;
		return false;
	}

	/// get content chunk, which was sent with the header,
	/// instead of requesting more (needed) chunks prematurely
	/// -----------------------------------------------------------------------
	if (    network->HTTP.stage == NETWORK_HTTP_STAGE_RESPONSED_HEADER
		AND network->HTTP.bytes_receiving > network->HTTP.header_size
	) {
		String s_remaining_chunk = S(network->HTTP.s_buffer_chunk);
		String_AddOffset(&s_remaining_chunk, network->HTTP.header_size);

		String_Destroy(s_response_out);
		*s_response_out = s_remaining_chunk;

		network->HTTP.stage = NETWORK_HTTP_STAGE_RESPONSED_DATA;
		network->HTTP.bytes_received += s_remaining_chunk.length;

		return true;
	}

	/// receive remaining chunks
	/// -----------------------------------------------------------------------
	if (network->HTTP.stage != NETWORK_HTTP_STAGE_RESPONSED_HEADER) {
		if (network->HTTP.content_length)
			network->HTTP.is_receiving = (network->HTTP.bytes_received != network->HTTP.content_length);
	}

	if (network->HTTP.is_receiving) {
		network->HTTP.stage = NETWORK_HTTP_STAGE_RESPONSED_DATA;

		network->HTTP.bytes_receiving  = Network_Receive(network,
														&network->HTTP.s_buffer_chunk,
														network->HTTP.content_length > 0);

		network->HTTP.bytes_received  += network->HTTP.bytes_receiving;

		if (network->HTTP.bytes_receiving <= 0) {
			if (network->HTTP.bytes_receiving < 0) {
				network->HTTP.stage = NETWORK_HTTP_STAGE_ERROR;
				network->s_error = S("Receiving remaining data chunks failed.");
				return false;
			}

			/// at this point, there was nothing more to receive (or the timeout kicked in)
			/// and the transfer is done, but only in case the content-length was not known
		}
		else {
			String_Destroy(s_response_out);
			*s_response_out = S(network->HTTP.s_buffer_chunk, network->HTTP.bytes_receiving);

			return true;
		}

	}

	network->HTTP.stage = NETWORK_HTTP_STAGE_IDLE;
	return false;
}

instant String
Network_HTTP_DownloadData(
	String s_url,
	bool *has_error
) {
	#define OnErrorReturn()	\
		if (Network_HasError(&network)) {	\
			Network_Destroy(&network); \
			*has_error = true;	\
			String_Destroy(&s_result); \
			s_result = S(network.s_error);	\
			return s_result;	\
		}

	Assert(has_error);

	Network network;
	String s_header;
	String s_response;
	bool success;

	String s_result;
	bool is_url_reloacating;

	/// to make sure it does not end up in an endless loop
	u16 relocation_count_limit = 5;

	do {
		is_url_reloacating = false;

		Network_HTTP_Request(&network, s_url);
		OnErrorReturn();

		success = Network_HTTP_GetResponseRef(&network, &s_header);

		is_url_reloacating = (   network.HTTP.response_code == 301
							  OR network.HTTP.response_code == 302);

		if (is_url_reloacating) {
			if (s_url.is_reference) {
				s_url = s_header;
			}
			else{
				String_Overwrite(&s_url, s_header);
			}

			--relocation_count_limit;
		}

	} while(is_url_reloacating AND relocation_count_limit);

	OnErrorReturn();

	while (Network_HTTP_GetResponseRef(&network, &s_response)) {
		String_Append(&s_result, s_response);
	}

	OnErrorReturn();

	Network_Destroy(&network);

	*has_error = false;
	return s_result;
}

/// - returns unix timestamp
/// - on error -> returns 0
instant u64
Network_NTP_GetTimestamp(
	String s_ntp_server
) {
	const s32 NTP_PACKET_SIZE = 48;

	Network network = Network_Connect(SOCKET_UDP, s_ntp_server, 123);

	static String s_ntp_packet = String_CreateBuffer(NTP_PACKET_SIZE);

	Memory_Set(s_ntp_packet.value, 0, NTP_PACKET_SIZE);
	s_ntp_packet.value[00] = 0b11100011;
	s_ntp_packet.value[01] = 0;
	s_ntp_packet.value[02] = 6;
	s_ntp_packet.value[03] = 0xEC;
	s_ntp_packet.value[12] = 49;
	s_ntp_packet.value[13] = 0x4E;
	s_ntp_packet.value[14] = 49;
	s_ntp_packet.value[15] = 52;

    Network_Send(&network, s_ntp_packet);

    s32 recv = Network_Receive(&network, &s_ntp_packet, false);

    u64 timestamp_unix_sec = 0;

	if (    !Network_HasError(&network)
		AND (recv == NTP_PACKET_SIZE)
	) {
		u64 word_high = (s_ntp_packet.value[40] << 8 | s_ntp_packet.value[41]);
		u64 word_low  = (s_ntp_packet.value[42] << 8 | s_ntp_packet.value[43]);

		/// a ntp server can send bogus data...
		if (word_high > word_low) {
			/// number of seconds since January 1, 1970 at 00:00:00 GMT
			timestamp_unix_sec  = (word_high << 16) | word_low;
			timestamp_unix_sec -= 2208988800;
		}
	}

	Network_Close(&network);

	return timestamp_unix_sec;
}
