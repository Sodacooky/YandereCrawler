#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <future>
#include <chrono>
#include <curl/curl.h>
/*	windows.h 用于创建文件夹
		如果使用其他系统或不想用winapi，可以用 stdio.h 代替，但是比较麻烦 */
#include <Windows.h>

/*
		提供网页和图片的下载功能
*/
class Downloader {
public:
	//下载网页到字符串
	//返回是否成功
	static bool DownloadPageToString(const std::string& url, std::string& src_out);

	//下载一个图片到文件
	//返回是否成功
	static bool DownloadFile(const std::string& url);

	//多线程下载一组图片到文件
	static void AsyncDownloadFiles(const std::vector<std::string>& links);

	//创建download文件夹
	static void CreatePath();

private:
	//curl write callback
	static size_t __WriteToString(char* buff, size_t block_size, size_t block_amount, void* str_ptr);

	//设置一些相同的参数
	static void __SetCurlDefaultOpt(CURL* handle);

	//将字符串内的%20替换为空格
	static void __TransSymbol(std::string& str);

	//将链接的最后一段（通常是文件名）提取出来
	static std::string __ExtractFilename(const std::string& src_str);
};
