#include "Response.h"


string Response::get_header(const string name, const string value) {
	return name + " " + value + ";\r\n";
}

string Response::make_answer() {
	string stringify_answer;
	stringify_answer.append("HTTP/1.1 200").append(" OK\r\n")
		.append(Response::get_header("Content-Type:", "text/html")) 
		.append(Response::get_header("charset=", "utf-8")) 
		.append(Response::get_header("Content-Length:", to_string(body.length())))
		.append(Response::get_header("\r\n", body));

	answer = stringify_answer;

	return answer;
}  

