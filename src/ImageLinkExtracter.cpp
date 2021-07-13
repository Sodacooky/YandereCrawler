#include "ImageLinkExtracter.h"

std::list<std::string> ImageLinkExtracter::Extract(
	const std::string &web_content)
{
	//
	std::list<std::string> result;
	//当前查找位置
	size_t nowFindingPos = 0;
	//提取整个网页的所有图片链接
	while (true)
	{
		// 找到链接位置
		auto link_pos = __SeekLinkPosition(nowFindingPos, web_content);
		if (link_pos == -1)
			break; // "eof"

		// 提取出链接
		auto link = __ExtractLink(link_pos, web_content);
		result.push_back(link);

		// 当前查找位置移动到链接后
		nowFindingPos = link_pos + link.size() + 1;
	}
	return result;
}

size_t ImageLinkExtracter::__SeekLinkPosition(size_t startpos, const std::string &web_content)
{
	//找到链接类头
	auto pos = web_content.find(R"(<a class="directlink largeimg")", startpos);
	if (pos == std::string::npos)
		return -1;
	//从链接类头移动到链接位置
	return web_content.find(R"(href=")", pos) + 6;
}

std::string ImageLinkExtracter::__ExtractLink(size_t startpos, const std::string &web_content)
{
	//链接以引号结束
	auto endPos = web_content.find("\"", startpos);
	//将链接头到引号前拷贝
	return std::string(web_content.cbegin() + startpos,
					   web_content.cbegin() + endPos);
}
