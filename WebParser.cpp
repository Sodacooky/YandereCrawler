#include "WebParser.h"

using namespace std;

WebParser::WebParser(std::vector<std::string>& tags, int page) {
	Downloader::DownloadPageToString(GenerateURL(tags, page), m_strWebSource);
	while (true) {
		//get pos
		auto pos = __SeekNextDownloadLinkPos();
		if (pos == -1) break;
		m_nWebSourcePos = pos;
		//get and store link
		auto link = __ExtractLink();
		vstrLinks.push_back(link);
	}
	if (vstrLinks.size() > 0) bAvaliable = true;
}

int WebParser::__SeekNextDownloadLinkPos() {
	auto pos = m_strWebSource.find(R"(<a class="directlink largeimg")", m_nWebSourcePos);
	if (pos == string::npos) {
		//not found any
		return -1;
	}
	return m_strWebSource.find(R"(href=")", pos) + 6;
}

std::string WebParser::__ExtractLink() {
	//链接以引号结束
	auto pos = m_strWebSource.find("\"", m_nWebSourcePos);
	return std::string(m_strWebSource.begin() + m_nWebSourcePos, m_strWebSource.begin() + pos);
}

std::string WebParser::GenerateURL(const std::vector<std::string>& tags, int page) {
	/*	https://yande.re/post?page={page}&tags={tags1}+{tags2}+... */
	string url = "https://yande.re/post?page=" + to_string(page);
	url.append("&tags=");
	for (auto& tag : tags) {
		url.append(tag);
		url.append("+");
	}
	return url;
}