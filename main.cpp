#include "HttpClient.h" 
int main() {
	HttpClient httpclient;
	string Url = "http://tjservice.cn/campus";
	string result = httpclient.GetUrl(Url);
	cout << result<<'\r\n';
	httpclient.AddRequestHeader("Content-Type","application/x-www-form-urlencoded");
	Url = "http://tjservice.cn/build";
	map<string, string> data;
	data["campus"] = "1";
	result=httpclient.PostUrl(Url,data);
	cout << result<<'\r\n';
	return 0;
}

