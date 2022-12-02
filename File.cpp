#include "File.h"

using namespace server;

string File::ReadFile(const string path) {
	ifstream fin(HOME + path, ios::binary);
	if (!fin.is_open()) {
		return "";
	}
	std::string data((std::istreambuf_iterator<char>(fin)),
		std::istreambuf_iterator<char>());
	return data;
}

string File::GetFileExtension(const string path) {
	const auto start = path.rfind(".");
	if (start == -1) {
		return "html";
	}
	return path.substr(start + 1);
}
