#include "src/SLib.h"
#include "test/core.h"

int main() {
	Network network;
	String s_header;
	String s_response;

/// (Extended) usage example(s)
/// ===========================================================================
///	String s_url_example = S("http://user:pass@192.168.0.1:1234/demo.txt");
	String s_url         = S("http://www.example.com");

	Network_HTTP_Request(&network, s_url);
	Network_HTTP_GetResponseRef(&network, &s_header);

	if (   network.HTTP.response_code == 301
		OR network.HTTP.response_code == 302
	) {
		LOG_DEBUG("New URL:");
		String_PrintLine(s_header);

		Network_Destroy(&network);
		return 0;
	}

	/// will make prev. reference data in s_header invalid,
	/// since its storage will be overwritten
	/// -> create a copy of the header in case the data is needed
	///    during this operation
	while (Network_HTTP_GetResponseRef(&network, &s_response)) {
		String_Print(s_response);
	}

	if (Network_HasError(&network)) {
		String_Print(S("[Error] "));
		String_PrintLine(network.s_error);

		LOG_DEBUG("Code: " << network.HTTP.response_code);
	}

	Network_Destroy(&network);

	return 0;
}
