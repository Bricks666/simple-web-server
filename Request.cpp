#include "Request.h"

using namespace server;

void Request::ParseRequest(string _request) {
	const int start_index = _request.find("/");
	const int end_index = _request.find("HTTP") - 1;
	url = _request.substr(start_index, end_index - start_index);
	request = _request;
	_request.erase(0, _request.find("\r\n") + 2);
	headers = ParseHeaders(_request);
}
map<string, string> Request::ParseHeaders(string& _request) {
	vector<string> raw_headers;
	map<string, string> headers;
	while (_request.length() > 2) {
		raw_headers.push_back(_request.substr(0, _request.find("\r\n")));
		_request = _request.erase(0, _request.find("\r\n") + 2);
	}

	auto begin = raw_headers.begin();
	for (; begin != raw_headers.end(); begin++) {
		const auto& element = (*begin);
		const int separator_index = element.find(": ");
		const auto key = element.substr(0, separator_index);
		const auto value = element.substr(separator_index + 2);
		headers.insert(pair<string, string>(key, value));
	}
	
	return headers;
}



const string& Request::GetURL() {
	return url;
 }
