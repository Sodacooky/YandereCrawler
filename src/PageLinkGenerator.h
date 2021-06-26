#ifndef __PAGELINKGENERATOR_H__
#define __PAGELINKGENERATOR_H__

#include <curl/curl.h>

#include <list>
#include <string>

//生成指定页指定tag的链接
//并可改变页
class PageLinkGenerator {
 public:
  // with empty content
  PageLinkGenerator();
  ~PageLinkGenerator();

 public:
  // add tag
  void AddTag(const std::string& tag);

  // change page
  void ChangePage(int page);

  // generate and copy
  std::string Generate();

 private:
  // tags to generate
  std::list<std::string> m_listTags;
  // page to generate
  int m_nPage;
  // for curl_easy_escape only
  CURL* m_pCurl;
};

#endif  // __PAGELINKGENERATOR_H__