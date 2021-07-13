#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Config.h"
#include "PageLinkGenerator.h"
#include <list>
#include <string>

//应用程序
class Application
{
public:
	//init
	Application();
	//main
	void Main();

private:
	//加载或新建config.json
	void __LoadConfig();
	//输入任务
	void __TaskInput();
	std::list<std::string> __InputTags();
	void __InputPageRange();
	//启动
	void __ExecuteTask();
	void __CreateOrReuseDirectory();

private:
	//config
	Config m_Config;
	bool m_bConfigLoaded;
	//task
	int m_nStartPage, m_nEndPage;
	PageLinkGenerator m_LinkGenerator;
	std::string m_strPath;
};

#endif // __APPLICATION_H__