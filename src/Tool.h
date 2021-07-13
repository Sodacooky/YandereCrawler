#ifndef __TOOL_H__
#define __TOOL_H__

#include <list>
#include <string>

// copy from my browser
const std::string CURL_AGENT =
	"User-Agent,Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
	"(KHTML, like Gecko) Chrome/88.0.4324.182 Safari/537.36 Edg/88.0.705.74";

// atoi
int StringToInt(const std::string &src_str);

// 以空格分割行
std::list<std::string> SplitWord(const std::string &line);

#endif // __TOOL_H__