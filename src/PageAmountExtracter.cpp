#include "PageAmountExtracter.h"

using namespace std;

int PageAmountExtracter::Extract(const std::string &webSource)
{
  //是否有navigation
  auto startPos = webSource.find(R"(<div role="navigation")");
  if (startPos != string::npos)
  {
    //循环找到最后一个 page
    auto endPos = webSource.find(R"(<a class="next_page")", startPos);
    string maxPageText = "0";
    while (true)
    {
      //带空格结尾的Page只在navitation中
      startPos = webSource.find("Page ", startPos);
      if (startPos == string::npos)
      {
        break;
      }
      startPos += 5; // skip "Page "
      auto pageTextEnd = webSource.find("\"", startPos);
      // extract page
      maxPageText.assign(webSource.begin() + startPos,
                         webSource.begin() + pageTextEnd);
    }
    return StringToInt(maxPageText);
  }

  //没有navigation可能只有1页
  //"Nobody here but us chickens!"是真的的一页都没有时存在的
  if (webSource.find("Nobody here but us chickens!") != string::npos)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}
