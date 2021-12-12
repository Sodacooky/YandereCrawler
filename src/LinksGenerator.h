#ifndef YANDERECRAWLER_LINKSGENERATOR_H
#define YANDERECRAWLER_LINKSGENERATOR_H

#include <string>
#include <vector>
#include "Config.h"

/*  通过给定的tags，访问网站提取出原图链接，并放入队列中
 * */
class LinksGenerator
{
  public:
    //开始下载网页，提取链接，返回所有图片的下载链接
    std::vector<std::string> Start();

  public:
    //给定输入的tags行，和下载范围
    LinksGenerator(Config &config, const std::string &tagsLine, int start, int end);

  private:
    //在构造函数中，将tagsLine这一控制台输入转换为转以后的m_linksTagsArg这一URL参数
    void ConvertTagsLineToArg(const std::string &tagsLine);

  private:
    // use tags and page number to generate page url
    std::string GeneratePageLink(int pageNumber);
    // use page link to get all picture downloading link
    std::vector<std::string> ExtractDownloadLinks(const std::string &pageLink);
    // download wbe source code via page link
    std::string DownloadPage(const std::string &pageLink) const;

  private:
    // escaped tags arg, (&tags=....+...+
    std::string m_linksTagsArg;
    //  the page range
    int m_startPage, m_endPage;
    // user settings
    Config &m_config;
};

#endif  // YANDERECRAWLER_LINKSGENERATOR_H
