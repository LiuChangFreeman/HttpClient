#include <vector>
#include <map>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <regex>
using namespace std;
class HttpClient {
public:
	string version;
	int code;
	string status;
	HttpClient() {};
	string GetUrl(string Url);
	string PostUrl(string Url, map<string, string> Data);
	void AddRequestHeader(string key,string value);
	void AddResponseHeader(string key, string value);
private:
	map<string,string> RequestHeaders;
	map<string, string> ResponseHeaders;
	string ResponseBody;
};

