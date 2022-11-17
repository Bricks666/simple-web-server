#include "Request.h"

using namespace server;

string Request::GetURL() {
	const int start_index = content.find("/");
	const int end_index = content.find("HTTP") - 1;

	return content.substr(start_index, end_index - start_index);
 }
