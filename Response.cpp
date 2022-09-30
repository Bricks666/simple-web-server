#include "Response.h"


string Response::get_document(string path) {
	ifstream document;
	string stringifyDocument = "";
	document.open(path, ios::in);
	if (document.is_open()) {
		string line = "";
		while (getline(document, line))
		{
			stringifyDocument += line;
		}
	}

	document.close();

	return stringifyDocument;
}

string Response::get_answer(string body) {
	ostringstream strStream;
	strStream <<
		"HTTP/1.1 200 OK\r\n"
		<< "Content-Type: text/html; charset=utf-8\r\n"
		<< "Content-Length: " << body.length() << "\r\n" <<
		"\r\n" <<
		body.c_str();

	return strStream.str();
}  

