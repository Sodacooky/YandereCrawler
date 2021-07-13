#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>

//程序运行的一些设置
/*
  是否默认下载全部页面
  线程数量
  是否弃用HTTP代理
  HTTP代理地址
 */
class Config
{
public:
	// default config
	Config();
	// assign
	Config(bool all_page, bool http_proxy, int thread_amount, const std::string &proxy_addr);

public:
	//
	bool DefaultDownloadAllPage() const;
	void DefaultDownloadAllPage(bool all_page);
	//
	bool EnableHttpProxy() const;
	void EnableHttpProxy(bool http_proxy);
	//
	int DownloadThreadAmount() const;
	void DownloadThreadAmount(int thread_amount);
	//
	std::string ProxyAddress() const;
	void ProxyAddress(const std::string &proxy_addr);

private:
	bool m_bAllPage;
	bool m_bHttpProxy;
	int m_nThreadAmount;
	std::string m_strProxyAddr;
};
#endif // __CONFIG_H__