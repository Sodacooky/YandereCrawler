#ifndef __WEBPARSER_H__
#define __WEBPARSER_H__

#include <spdlog/spdlog.h>

#include <chrono>
#include <future>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "Downloader.h"
#include "LinkParser.h"
#include "Tool.h"

//从网页源码中提取所有原图链接
class WebParser {
 public:
  //给予标签列表和页数范围，内部生成url并开始解析
  WebParser(std::vector<std::string> &tags, int pageStart, int pageEnd,
            std::vector<std::string> &links_out)
      : m_nPageStart(pageStart),
        m_nPageEnd(pageEnd),
        m_rvec_strTags(tags),
        m_rvec_strLinks(links_out) {
    __Do();
  }

 private:
  // main work
  void __Do();

  // 分析链接线程函数
  // 返回pageLinksIndex对应m_vec_strPageLinks的原图链接
  std::vector<std::string> __GetLinks(int pageLinksIndex);

 private:
  // tags
  std::vector<std::string> &m_rvec_strTags;
  //所有预览网页的链接
  std::vector<std::string> m_vec_strPageLinks;
  // page range
  int m_nPageStart, m_nPageEnd;
  //
  std::vector<std::string> &m_rvec_strLinks;
};
#endif  // __WEBPARSER_H__