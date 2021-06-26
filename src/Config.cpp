#include "Config.h"

Config::Config()
    : m_bAllPage(false),
      m_bHttpProxy(false),
      m_nThreadAmount(1),
      m_strProxyAddr("null")
{
}

Config::Config(bool all_page, bool http_proxy, int thread_amount, const std::string &proxy_addr)
    : m_bAllPage(all_page),
      m_bHttpProxy(http_proxy),
      m_nThreadAmount(thread_amount),
      m_strProxyAddr(proxy_addr)
{
}

bool Config::DefaultDownloadAllPage() const
{
  return m_bAllPage;
}

void Config::DefaultDownloadAllPage(bool all_page)
{
  m_bAllPage = all_page;
}

bool Config::EnableHttpProxy() const
{
  return m_bHttpProxy;
}

void Config::EnableHttpProxy(bool http_proxy)
{
  m_bHttpProxy = http_proxy;
}

int Config::DownloadThreadAmount() const
{
  return m_nThreadAmount;
}

void Config::DownloadThreadAmount(int thread_amount)
{
  m_nThreadAmount = thread_amount;
}

std::string Config::ProxyAddress() const
{
  return m_strProxyAddr;
}

void Config::ProxyAddress(const std::string &proxy_addr)
{
  m_strProxyAddr = proxy_addr;
}
