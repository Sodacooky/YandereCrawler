#ifndef __PAGEDOWNLOADER_H__
#define __PAGEDOWNLOADER_H__

#include "Config.h"
#include <string>

//网页源代码下载
class PageDownloader
{
public:
	static std::string Download(const std::string &url, const Config &config);

private:
	static size_t __WriteToString(char *buff, size_t block_size,
								  size_t block_amount, void *str_ptr);
};

#endif // __PAGEDOWNLOADER_H__