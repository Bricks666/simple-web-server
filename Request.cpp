#include "Request.h"


string Request::get_URL() {
	const int start_index = content.find("/");
	const int end_index = content.find("HTTP") - 1;

	return content.substr(start_index, end_index - start_index);
 }