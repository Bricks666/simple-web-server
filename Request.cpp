#include "Request.h"


using namespace server;

string Request::get_URL() {
	const int start_index = request.find("/");
	const int end_index = request.find("HTTP") - 2;

	return request.substr(start_index, end_index - start_index);
 }