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
	void clear();
private:
	map<string,string> RequestHeaders;
	map<string, string> ResponseHeaders;
	string ResponseBody;
};
vector<string> split(const string& source, string seperator) {
	vector<string> results;
	int SeparateCharacterLen = seperator.size();
	int LastPosition = 0, index = -1;
	while ((index = source.find(seperator, LastPosition)) != -1) {
		results.push_back(source.substr(LastPosition, index - LastPosition));
		LastPosition = index + SeparateCharacterLen;
	}
	string LastString = source.substr(LastPosition);
	if (!LastString.empty()) {
		results.push_back(LastString);
	}
	return results;
}
string HttpClient::GetUrl(string Url) {
	vector<string> IPCollection;
	regex patten("http://.+?/");
	smatch results;
	if (regex_search(Url, results, patten)) {
		for (auto x : results) {
			string domain = regex_replace(string(x), regex("(http://)(.+?)(/)"), "$2");
			AddRequestHeader("Host", domain);
			struct hostent *DomainToIP = NULL;
			DomainToIP = gethostbyname(domain.data());
			if (DomainToIP != NULL) {
				for (int i = 0;; i++) {
					if (DomainToIP->h_addr_list[i] != NULL) {
						IPCollection.push_back(inet_ntoa((in_addr &)*DomainToIP->h_addr_list[i]));
					}
					else {
						break;
					}
				}
			}

		}
	}
	int port = 80;
	int socket_client = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(IPCollection[0].data());
	string HttpRequest = "GET ";
	char buffer[5000];
	connect(socket_client, (struct sockaddr *)&server_addr, sizeof(server_addr));
	HttpRequest += regex_replace(Url, regex("(http://)(.+?)(/.*)"), "$3 ");
	HttpRequest += "HTTP/1.1\r\n";
	map<string, string>::iterator i;
	for (i = RequestHeaders.begin(); i != RequestHeaders.end(); i++) {
		HttpRequest += i->first;
		HttpRequest += ": ";
		HttpRequest += i->second;
		HttpRequest += "\r\n";
	}
	HttpRequest += "\r\n";
	strcpy(buffer, HttpRequest.data());
	if (send(socket_client, buffer, strlen(buffer), 0) == -1) {
		perror("error in sending");
	}
	memset(buffer, 0, sizeof(buffer));
	if (recv(socket_client, buffer, 4096, 0) == -1) {
		perror("receive error");
	}
	string HttpResponse = buffer;
	const char *sep = "\r\n\r\n";
	vector<string> splitup = split(HttpResponse, sep);
	vector<string>::iterator body = ++splitup.begin();
	while (body != splitup.end()) {
		ResponseBody += *body;
		body++;
	}
	sep = "\r\n";
	splitup = split(splitup[0], sep);
	string temp = splitup[0].data();
	version = regex_replace(temp, regex("(HTTP/)(1\.[0-9]) ([0-9]{3}) (.*)"), "$2").data();
	code = atoi(regex_replace(temp, regex("(HTTP/)(1\.[0-9]) ([0-9]{3}) (.*)"), "$3").data());
	status = regex_replace(temp, regex("(HTTP/)(1\.[0-9]) ([0-9]{3}) (.*)"), "$4").data();
	vector<string>::iterator j = ++splitup.begin();
	while (j != splitup.end()) {
		smatch MatchResults;
		if (regex_search(*j, MatchResults, regex("(.*): (.*)"))) {
			for (auto x : MatchResults) {
				string key = regex_replace(*j, regex("(.*): (.*)"), "$1");
				string value = regex_replace(*j, regex("(.*): (.*)"), "$2");
				AddResponseHeader(key, value);
				ResponseHeaders;
			}
		}
		else {
			break;
		}
		j++;
	}
	int length = atoi(ResponseHeaders["Content-Length"].data()) - 4096;
	while (length>0) {
		memset(buffer, 0, sizeof(buffer));
		if (recv(socket_client, buffer, 4096, 0) == -1) {
			perror("receive error");
		}
		ResponseBody += string(buffer);
		length -= 4096;
	}
	return ResponseBody;
}
string HttpClient::PostUrl(string Url, map<string, string> Data) {
	vector<string> IPCollection;
	regex patten("http://.+?/");
	smatch results;
	if (regex_search(Url, results, patten)) {
		for (auto x : results) {
			string domain = regex_replace(string(x), regex("(http://)(.+?)(/)"), "$2");
			AddRequestHeader("Host", domain);
			struct hostent *DomainToIP = NULL;
			DomainToIP = gethostbyname(domain.data());
			if (DomainToIP != NULL) {
				for (int i = 0;; i++) {
					if (DomainToIP->h_addr_list[i] != NULL) {
						IPCollection.push_back(inet_ntoa((in_addr &)*DomainToIP->h_addr_list[i]));
					}
					else {
						break;
					}
				}
			}

		}
	}
	int port = 80;
	int socket_client = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(IPCollection[0].data());
	string HttpRequest = "POST ";
	char buffer[5000];
	connect(socket_client, (struct sockaddr *)&server_addr, sizeof(server_addr));
	HttpRequest += regex_replace(Url, regex("(http://)(.+?)(/.*)"), "$3 ");
	HttpRequest += "HTTP/1.1\r\n";
	map<string, string>::iterator i;
	string PostData;
	int LengthCount = 0;
	for (i = Data.begin(); i != Data.end(); i++) {
		PostData += i->first;
		PostData += "=";
		PostData += i->second;
		PostData += "&";
	}
	PostData.pop_back();
	char DataLength[16];
	sprintf(DataLength, "%d", strlen(PostData.data()));
	AddRequestHeader("Content-Length", DataLength);
	for (i = RequestHeaders.begin(); i != RequestHeaders.end(); i++) {
		HttpRequest += i->first;
		HttpRequest += ": ";
		HttpRequest += i->second;
		HttpRequest += "\r\n";
	}
	HttpRequest += "\r\n";
	HttpRequest += PostData;
	strcpy(buffer, HttpRequest.data());
	if (send(socket_client, buffer, strlen(buffer), MSG_NOSIGNAL) == -1) {
		perror("error in sending");
	}
	memset(buffer, 0, sizeof(buffer));
	if (recv(socket_client, buffer, 4096, 0) == -1) {
		perror("receive error");
	}
	string HttpResponse = buffer;
	const char *sep = "\r\n\r\n";
	vector<string> splitup = split(HttpResponse, sep);
	vector<string>::iterator body = ++splitup.begin();
	while (body != splitup.end()) {
		ResponseBody += *body;
		body++;
	}
	sep = "\r\n";
	splitup = split(splitup[0], sep);
	string temp = splitup[0].data();
	version = regex_replace(temp, regex("(HTTP/)(1\.[0-9]) ([0-9]{3}) (.*)"), "$2").data();
	code = atoi(regex_replace(temp, regex("(HTTP/)(1\.[0-9]) ([0-9]{3}) (.*)"), "$3").data());
	status = regex_replace(temp, regex("(HTTP/)(1\.[0-9]) ([0-9]{3}) (.*)"), "$4").data();
	vector<string>::iterator j = ++splitup.begin();
	while (j != splitup.end()) {
		smatch MatchResults;
		if (regex_search(*j, MatchResults, regex("(.*): (.*)"))) {
			for (auto x : MatchResults) {
				string key = regex_replace(*j, regex("(.*): (.*)"), "$1");
				string value = regex_replace(*j, regex("(.*): (.*)"), "$2");
				AddResponseHeader(key, value);
				ResponseHeaders;
			}
		}
		else {
			break;
		}
		j++;
	}
	int length = atoi(ResponseHeaders["Content-Length"].data()) - 4096;
	while (length > 0) {
		memset(buffer, 0, sizeof(buffer));
		if (recv(socket_client, buffer, 4096, 0) == -1) {
			perror("receive error");
		}
		ResponseBody += string(buffer);
		length -= 4096;
	}
	return ResponseBody;
}
void HttpClient::AddRequestHeader(string key, string value) {
	RequestHeaders[key] = value;
}
void HttpClient::AddResponseHeader(string key, string value) {
	ResponseHeaders[key] = value;
}
void HttpClient::clear() {
	version = "";
	code = 0;
	status = "";
	ResponseBody = "";
	map<string, string>::iterator i;
	for (i = RequestHeaders.begin();i!= RequestHeaders.end();) {
		i=RequestHeaders.erase(i);
	}
	for (i = ResponseHeaders.begin(); i != ResponseHeaders.end();) {
		i = ResponseHeaders.erase(i);
	}
}
