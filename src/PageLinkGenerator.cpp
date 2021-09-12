#include "PageLinkGenerator.h"

PageLinkGenerator::PageLinkGenerator()
{
	m_nPage = -1;
	m_pCurl = curl_easy_init();
}

PageLinkGenerator::~PageLinkGenerator() { curl_easy_cleanup(m_pCurl); }

void PageLinkGenerator::AddTag(const std::string &tag)
{
	m_listTags.push_back(tag);
}

void PageLinkGenerator::ChangePage(int page)
{
	if (page <= 0 || page >= 99999)
	{
		throw "invalid page.";
	}
	m_nPage = page;
}

std::string PageLinkGenerator::Generate()
{
	std::string result = "https://yande.re/post?page=";
	result.append(std::to_string(m_nPage));
	result.append("&tags=");
	for (auto &tag : m_listTags)
	{
		char *_tag = curl_easy_escape(m_pCurl, tag.c_str(), 0);
		result.append(_tag);
		result.append("+");
		curl_free(_tag);
	}
	return result;
}
