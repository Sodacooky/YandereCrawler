#ifndef __DOWNLOADER_H__
#define __DOWNLOADER_H__

#include <string>
#include <vector>
#include "Config.h"

//将指定url下载到string中（二进制或字符
//若为二进制注意用string的data方法而不是c_str来提取内容
class Downlaoder
{
  public:
    //返回char串
    static std::vector<char> Download(const std::string &url, const Config &config);

  private:
    static size_t __CurlWriteFunc(char *ptrBuffer, size_t size, size_t nmemb, void *ptrVecChar);
};

#endif  // __DOWNLOADER_H__