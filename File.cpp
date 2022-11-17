#include "File.h"

string File::get_document(string path) {
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
