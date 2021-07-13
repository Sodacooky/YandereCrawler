#include "Application.h"
#include "ConfigReader.h"
#include "ImageLinkExtracter.h"
#include "MultiFileDownloader.h"
#include "PageAmountExtracter.h"
#include "PageDownloader.h"
#include "Tool.h"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>
#include <thread>

Application::Application()
	: m_bConfigLoaded(false),
	  m_nStartPage(-1), m_nEndPage(-1)
{
}

void Application::Main()
{
	__LoadConfig();
	__TaskInput();
	__ExecuteTask();
}

void Application::__LoadConfig()
{
	if (ConfigReader::IsExist() == false)
	{
		spdlog::warn("找不到config.json，将创建");
		ConfigReader::WriteDefault();
	}

	if (ConfigReader::IsExist() == false)
	{
		spdlog::warn("无法创建config.json，将使用默认Config");
		//构造函数中，调用了Config的默认构造函数，即为默认Config
		return;
	}

	m_Config = ConfigReader::Read();
	//fix thread amount
	auto thread_amount = m_Config.DownloadThreadAmount();
	if (thread_amount <= 0 || thread_amount > 32)
	{
		m_Config.DownloadThreadAmount(1);
		spdlog::critical("config.json中设定的线程数 {} 不合理，已将其设置为 1 !!", thread_amount);
	}
}

void Application::__TaskInput()
{
	//tags
	std::list<std::string> list_tags = __InputTags();
	if (list_tags.size() == 0)
	{
		spdlog::critical("Tags输入无效");
		exit(-100);
	}
	if (list_tags.size() > 6)
	{
		spdlog::critical("最多支持6个Tag");
		exit(-101);
	}

	//set path
	m_strPath = "./" + *list_tags.begin();

	//set link generator
	for (auto &tag : list_tags)
	{
		m_LinkGenerator.AddTag(tag);
	}
	m_LinkGenerator.ChangePage(1);

	//download page amount info
	int page_amount = PageAmountExtracter::Extract(
		PageDownloader::Download(m_LinkGenerator.Generate(), m_Config));
	spdlog::info("所输入的Tags共有 {} 页数据", page_amount);
	if (page_amount < 0)
	{
		spdlog::critical("请检查网络连接和代理");
		exit(-102);
	}
	if (page_amount == 0)
	{
		spdlog::critical("请检查输入的Tags");
		exit(-103);
	}

	//page range
	__InputPageRange();
	if (m_nEndPage > page_amount)
	{
		m_nEndPage = page_amount;
		spdlog::warn("已将终止页设置为 {}", m_nEndPage);
	}
	spdlog::info("将下载从{}到{}共 {} 页", m_nStartPage, m_nEndPage, m_nEndPage - m_nStartPage + 1);
}

void Application::__ExecuteTask()
{
	__CreateOrReuseDirectory();
	for (int now_page = m_nStartPage; now_page <= m_nEndPage; now_page++)
	{
		//download and extract links
		spdlog::info("正在下载第 {} 页", now_page);
		m_LinkGenerator.ChangePage(now_page);
		auto page_content = PageDownloader::Download(m_LinkGenerator.Generate(), m_Config);
		std::list<std::string> page_links = ImageLinkExtracter::Extract(page_content);
		//download and report
		MultiFileDownloader::Download(page_links, m_strPath, m_Config);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		spdlog::info("第 {} 页下载完成", now_page);
	}
	spdlog::info("    ---  下载完成  ---");
}

std::list<std::string> Application::__InputTags()
{
	std::string input_line;
	spdlog::info("请输入Tags，最多6个>(如: miko no_bra):");
	std::getline(std::cin, input_line);
	std::list<std::string> m_listTags = SplitWord(input_line);
	spdlog::info("输入了 {} 个Tags", m_listTags.size());
	return m_listTags;
}

void Application::__InputPageRange()
{
	spdlog::info("请以空格为分割输入起始页终止页(如: 2 999):");
	std::cin >> m_nStartPage >> m_nEndPage;
	if (m_nStartPage > m_nEndPage)
	{
		spdlog::critical("输入不正确");
		exit(-104);
	}
	if (m_nStartPage <= 0 || m_nEndPage <= 0)
	{
		spdlog::critical("输入不正确");
		exit(-105);
	}
}

void Application::__CreateOrReuseDirectory()
{
	std::filesystem::directory_entry entry(m_strPath);

	//is exist
	if (entry.exists())
	{
		//is directory
		if (entry.is_directory())
		{
			spdlog::warn("文件夹 {} 已存在，重用它", m_strPath);
		}
		else
		{
			spdlog::warn("非文件夹文件 {} 将保存文件到程序目录", m_strPath);
			m_strPath = ".";
		}
	}
	else
	{
		//create
		if (std::filesystem::create_directory(m_strPath))
		{
			spdlog::info("创建了文件夹 {}", m_strPath);
		}
		else
		{
			spdlog::warn("无法创建文件夹 {}，将保存文件到程序目录", m_strPath);
			m_strPath = ".";
		}
	}
}
