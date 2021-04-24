#ifndef __NAVIGATIONPARSER_H__
#define __NAVIGATIONPARSER_H__

#include <sstream>
#include <string>

//通过网页源代码，
//分析页面底部的翻页条，
//以此确定总共页数
class NavigationParser {
 public:
  NavigationParser(const std::string& webSource)
      : nPageAmount(-1), m_rstrWebSource(webSource) {
    __Do();
  };

 public:
  //-1 on unavaliable
  int nPageAmount;

 private:
  // actually work function
  void __Do();

 private:
  const std::string& m_rstrWebSource;
};
#endif  // __NAVIGATIONPARSER_H__