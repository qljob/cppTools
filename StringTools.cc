#include <string>
#include <vector>
#include "StringTools.h"
using namespace std;

namespace cppTools {

vector<string> split(const string &str, const string &sep) {
	vector<string> ret;
	size_t pre = 0, pos;
	while((pos = str.find(sep, pre)) != string::npos) {
		ret.push_back(str.substr(pre, pos-pre));
		pre = pos + sep.size();
	}
	ret.push_back(str.substr(pre));
	return ret;
}

string& trim(string &str) {
	size_t head = str.find_first_not_of(" \t\r\n");
	if(head == string::npos) {
		str = "";
		return str;
	}
	size_t tail = str.find_last_not_of(" \t\r\n");
	str = str.substr(head, tail - head + 1);
	return str;
}

string implode(const string &sep, const vector<string> &strs) {
	string ret = "";
	for(auto &str : strs) {
		ret += str + sep;
	}
	if(ret.size() != 0) {
		ret = ret.substr(0, ret.size() - sep.size());
	}
	return ret;
}

string& replace_all(const string& old_str, const string& new_str, string& source) {
	size_t pos = 0;
	while((pos = source.find(old_str, pos)) != string::npos) {
		source.replace(pos, old_str.size(), new_str);
		pos += new_str.size();
	}
	return source;
}

string base64_encode(const string& str) {
	char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	string base64 = "";
	size_t i = 2;
	while(i < str.size()) {
		unsigned char temp[3], buf[4];
		for(int j = 0; j < 3; j++) {
			temp[j] = (unsigned char)str[i-2+j];
		}
		buf[0] = temp[0] >> 2;
		buf[1] = ((temp[0] & 0x03) << 4) + (temp[1] >> 4);
		buf[2] = ((temp[1] & 0x0f) << 2) + (temp[2] >> 6);
		buf[3] = temp[2] & 0x3f;
		for(int j = 0; j < 4; j++) {
			base64 += table[buf[j]];
		}
		i += 3;
	}
	if(str.size() % 3 == 1) {
		unsigned char temp = (unsigned char)str[str.size()-1];
		base64 += table[temp >> 2];
		base64 += table[(temp & 0x03) << 4];
		base64 += "==";
	}
	if(str.size() % 3 == 2) {
		unsigned char temp[2], buf[3];
		temp[0] = (unsigned char)str[str.size()-2];
		temp[1] = (unsigned char)str[str.size()-1];
		buf[0] = temp[0] >> 2;
		buf[1] = ((temp[0] & 0x03) << 4) + (temp[1] >> 4);
		buf[2] = ((temp[1] & 0x0f)<< 2);
		for(int j = 0; j < 3; j++) {
			base64 += table[buf[j]];
		}
		base64 += "=";
	}
	return base64;
}

static inline constexpr uint8_t index_of(unsigned char c) {
	return (c >= 'A' && c <= 'Z') ? (c - 'A')
		: (c >= 'a' && c <='z') ? (c - 'a' + 26)
		: (c >= '0' && c <= '9') ? (c - '0' + 52)
		: (c == '+') ? 62
		: (c == '/') ? 63
		: 255;
}

string base64_decode(const string& str) {

	string decoded = "";
	unsigned char buf_4[4]; 
	char buf_3[4];
	int idx = 0;
	for(auto s : str) {
		if(s == '=')
			break;
		buf_4[idx++] = (unsigned char)s;
		if(idx == 4) {
			buf_3[0] = (index_of(buf_4[0]) << 2) + (index_of(buf_4[1]) >> 4);
			buf_3[1] = ((index_of(buf_4[1]) & 0x0f) << 4) + (index_of(buf_4[2]) >> 2);
			buf_3[2] = ((index_of(buf_4[2]) & 0x03) << 6) + index_of(buf_4[3]);
			buf_3[3] = 0;
			idx = 0;
			decoded += string(buf_3);
		}
	}
	if(2 == idx) {
		char temp = (index_of(buf_4[0]) << 2) + (index_of(buf_4[1]) >> 4);
		decoded += temp;
	}
	if(3 == idx) {
		char temp[3];
		temp[0] = (index_of(buf_4[0]) << 2) + (index_of(buf_4[1]) >> 4);
		temp[1] = ((index_of(buf_4[1]) & 0x0f) << 4) + (index_of(buf_4[2]) >> 2);
		temp[2] = 0;
		decoded += string(temp);
	}
	return decoded;
}

}
