#pragma once
#include "general.h"

class Response
{
	public:
		int code = 200;
		string redicret_URL = "";
		string body = "0" ;
		string answer;
		string make_answer();
	private:
		static map<int, string> response_name;
		static string get_header(const string name, const string value);
};

