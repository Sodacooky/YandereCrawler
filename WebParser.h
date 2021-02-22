#pragma once

#include <string>
#include <map>
#include <vector>
#include "Downloader.h"

/*
*		从Page网页源码中提取Tags和链接
*/
class WebParser {
public:
	//给予标签列表和页数，内部生成url并开始解析
	WebParser(std::vector<std::string>& tags, int page);

public:
	//此页内的原图链接
	std::vector<std::string> vstrLinks;

	//可用性
	bool bAvaliable = false;
private:

	//从当前位置找到一个 链接头
	//返回链接开头位置，若无则-1
	int __SeekNextDownloadLinkPos();

	//从当前位置提取下载链接，前提是到达了链接的第一个字母
	std::string __ExtractLink();

	//生成链接
	std::string GenerateURL(const std::vector<std::string>& tags, int page);

private:
	//网页源码
	std::string m_strWebSource;

	//当前字符串查找位置
	int m_nWebSourcePos = 0;
};

/*
	网页结构

	图片部分由若干组以下构成：

	tag头		<a class="thumb" ....

	Tags		...title="Rating:...Score:...Tags:..."

	链接头	<a class="directlink largeimg" ...

	链接		....href="..."

*/