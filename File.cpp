#include "File.h"

using namespace server;

string File::GetDocument(string path) {
	ifstream document;
	string stringifyDocument = "";
	document.open(HOME + path, ios::in);
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
