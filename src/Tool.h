#ifndef __TOOL_H__
#define __TOOL_H__

#include <string>

//将HTML转义后的字符，转换回其原始
//目前仅支持空格
std::string HTMLSymbolToRaw(const std::string& in_string);

//将字符转换为HTML转义字符
//目前仅支持符号 +
std::string RawToHTMLSymbol(const std::string& in_string);

#endif  // __TOOL_H__