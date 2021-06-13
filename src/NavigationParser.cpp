#include "NavigationParser.h"

using namespace std;

void NavigationParser::__Do() {
  //是否有navigation
  auto startPos = m_rstrWebSource.find(R"(<div role="navigation")");
  if (startPos == string::npos) {
    nPageAmount = -1;
    return;
  }

  //循环找到最后一个 page
  auto endPos = m_rstrWebSource.find(R"(<a class="next_page")", startPos);
  string maxPageText = "0";
  while (true) {
    //带空格结尾的Page只在navitation中
    startPos = m_rstrWebSource.find("Page ", startPos);
    if (startPos == string::npos) {
      break;
    }
    startPos += 5;  // skip "Page "
    auto pageTextEnd = m_rstrWebSource.find("\"", startPos);
    // extract page
    maxPageText.assign(m_rstrWebSource.begin() + startPos,
                       m_rstrWebSource.begin() + pageTextEnd);
  }

  // string -> int
  stringstream ss(maxPageText);
  ss >> nPageAmount;
}
