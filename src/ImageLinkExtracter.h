#ifndef __IMAGELINKEXTRACTER_H__
#define __IMAGELINKEXTRACTER_H__

#include <map>
#include <string>
#include "Config.h"

//从post页面源码中提取出若干个原图链接
class ImageLinkExtracter
{
  public:
    //返回一个<文件名,url>集
    static std::map<std::string, std::string> Extract(const std::string &search_page_url,
                                                      const Config &config);

  private:
    //返回链接头位置
    static size_t __GetLinkPosition(size_t startpos, const std::string &src);

    //从startpos开始提取直到下一个双引号
    static std::string __ExtractLink(size_t link_start_pos, const std::string &src);

    //从url中提取出文件名
    static std::string __ExtractFilename(const std::string &url);
};

#endif  // __IMAGELINKEXTRACTER_H__