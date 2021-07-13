#ifndef __PAGEAMOUNTEXTRACTER_H__
#define __PAGEAMOUNTEXTRACTER_H__

#include <string>

//通过网页源代码，
//分析页面底部的翻页条，
//以此确定总共页数
class PageAmountExtracter
{
public:
	//-1 on 404 or other failed
	static int Extract(const std::string &webSource);
};

#endif // __PAGEAMOUNTEXTRACTER_H__