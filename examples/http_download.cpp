#include "src/SLib.h"
#include "test/core.h"

int main() {
	Network network;
	String s_header;
	String s_response;

	String s_host_adress = S("www.example.com");

///	Network_HTTP_Request will connect to port 80 by default to the same
/// host adress as the one requesting to.
/// @Note: do not call the host-string "s_host", or winsock.h will complain
/// because of a macro, that is used there. Thank you, microsoft.
//	Network network = Network_Connect(s_host_adress, 80);

	Network_HTTP_Request(&network, s_host_adress, S(""));
	Network_HTTP_GetResponse(&network, &s_header);

	while (Network_HTTP_GetResponse(&network, &s_response)) {
		String_Print(s_response);
	}

	if (Network_HasError(&network)) {
		String_Print(S("[Error] "));
		String_PrintLine(network.s_error);
	}

	Network_Destroy(&network);

	return 0;
}
