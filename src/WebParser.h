#ifndef __WEBPARSER_H__
#define __WEBPARSER_H__

#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Downloader.h"
#include "LinkParser.h"

//从Page网页源码中提取Tags和链接
class WebParser {
 public:
  //给予标签列表和页数，内部生成url并开始解析
  WebParser(std::vector<std::string>& tags, int page);

 public:
  //此页内的原图链接
  //没有链接时为空
  std::vector<std::string> vec_strLinks;
};
#endif  // __WEBPARSER_H__