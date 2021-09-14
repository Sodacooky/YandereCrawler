#ifndef __PAGELINKGENERATOR_H__
#define __PAGELINKGENERATOR_H__

#include <string>

//生成指定页指定tag的链接
//并可改变页
class PageLinkGenerator
{
  public:
    //空格间隔的tags行
    explicit PageLinkGenerator(const std::string &tags_line);

    // generate and copy
    std::string Generate(int page);

  private:
    //链接的tags部分
    std::string m_strTagsPart;
    //链接的开头
    static const std::string sm_strHeader;
};

#endif  // __PAGELINKGENERATOR_H__