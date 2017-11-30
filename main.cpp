#include "HttpClient.h" 
int main() {
	HttpClient httpclient;
	httpclient.AddRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	string Url = "http://detectportal.firefox.com/success.txt";
	string result = httpclient.GetUrl(Url);
	if (result == "success\n") {
		cout <<"当前无需登录\n";
	}
	else {
		httpclient.clear();
		regex ipregex("(userip=)(.+?)(&)");
		smatch ipresults;
		string userip;
		if (regex_search(result,ipresults,ipregex)) {
			for (auto x:ipresults) {
				userip = regex_replace(string(x).data(), ipregex, "$2");
				break;
			}
		}
		string Referer = "http://192.168.192.18/index_10.html?userip="+userip+"&userurl=http://detectportal.firefox.com/success.txt";
		httpclient.AddRequestHeader("Referer",Referer);
		httpclient.AddRequestHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2486.0 Safari/537.36 Edge/13.11082");
		httpclient.AddRequestHeader("Accept", "*/*");
		httpclient.AddRequestHeader("Accept-Language", "zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2");
		httpclient.AddRequestHeader("Accept-Encoding", "gzip, deflate");
		httpclient.AddRequestHeader("Content-Type", "application/x-www-form-urlencoded");
		httpclient.AddRequestHeader("X-Requested-With", "XMLHttpRequest");
		httpclient.AddRequestHeader("Connection", "keep-alive");
		Url = "http://192.168.192.18/include/auth_action.php";
		map<string, string>data;
		data["action"] ="login";
		data["username"] = "";
		data["password"] = "";
		data["ac_id"] = "10";
		data["user_ip"] = userip;
		data["nas_ip"] = "";
		data["user_mac"] = "";
		data["save_m"] = "1";
		data["ajax"] = "1";
		string result = httpclient.PostUrl(Url,data);
		cout << result;
	}
	return 0;
}

