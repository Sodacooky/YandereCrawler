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
    //尝试从文件中读取
    void TryLoad();

    //将当前配置写出到文件
    void TryWriteOut();

  public:
    bool isAllPage        = false;
    bool isHttpProxy      = false;
    int threadAmount      = 1;
    std::string proxyAddr = "null";

  private:
    //默认路径的设置文件是否存在
    static bool IsFileExist();

  private:
    // default "config.json"
    static const char *sm_defaultPath;
};
#endif  // __CONFIG_H__