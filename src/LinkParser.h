#ifndef __LINKPARSER_H__
#define __LINKPARSER_H__

#include <string>
#include <vector>

//从网页源码中提取所有原图链接
//如果网页中没有原图那么vec_strLinks为空
class LinkParser {
 public:
  LinkParser(const std::string &webSource) : m_rstrWebSource(webSource) {
    __Do();
  }

 public:
  std::vector<std::string> vec_strLinks;

 private:
  // main work
  void __Do();

 private:
  const std::string &m_rstrWebSource;
};
#endif  // __LINKPARSER_H__