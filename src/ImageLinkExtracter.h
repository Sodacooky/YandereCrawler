#ifndef __IMAGELINKEXTRACTER_H__
#define __IMAGELINKEXTRACTER_H__

#include <list>
#include <string>

//从post页面源码中提取出若干个原图链接
class ImageLinkExtracter
{
public:
  static std::list<std::string> Extract(const std::string &web_content);

private:
  //返回链接头位置
  static size_t __SeekLinkPosition(size_t startpos, const std::string &web_content);

  //从startpos直到遇到"\\"
  static std::string __ExtractLink(size_t startpos, const std::string &web_content);
};

#endif // __IMAGELINKEXTRACTER_H__