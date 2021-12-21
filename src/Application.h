#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <atomic>
#include <map>
#include <mutex>
#include <string>
#include <vector>
#include "Config.h"

//应用程序
class Application
{
  public:
    Application();

  public:
    // main
    void Main();

  private:
    //用户交互
    void PromptInput();
    //创建文件夹
    // dir_name不以分隔符结尾
    static bool TryCreateDirectory(const std::string &dir_name);
    //提取文件名
    std::string ExtractFilename(const std::string &link);

  private:
    //配置文件
    Config m_config;
    //输入的Tags行
    std::string m_tagsLine;
    //页码范围
    int m_startPage, m_endPage;
    //保存文件的目录
    std::string m_directoryName;
    //进度条打印锁
    static std::mutex sm_mutexProgressBar;
};

#endif  // __APPLICATION_H__