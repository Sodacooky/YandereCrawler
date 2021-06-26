#include "MultiFileDownloader.h"

int MultiFileDownloader::Download(const std::list<std::string> &list,
                                  const std::string &path,
                                  const Config &config)
{
  auto thread_amount = config.DownloadThreadAmount();

  std::list<std::future<size_t>> list_future;
  auto list_iter = list.begin();
  int done_amount = 0;

  while (list_iter != list.end())
  {
    // 当线程数量未达到时分配新任务
    if (list_future.size() < thread_amount)
    {
      list_future.push_back(std::async(std::launch::async,
                                       FileDownloader::Download,
                                       path, *list_iter, config));
      list_iter++;
    }
    // 回收任务
    auto future_iter = list_future.begin();
    while (future_iter != list_future.end())
    {
      //完成
      if (future_iter->wait_for(std::chrono::milliseconds(250)) == std::future_status::ready)
      {
        if (future_iter->get() != 0)
        {
          done_amount++;
        }
        spdlog::info(" 已完成 {}/{}", done_amount, list.size());
        future_iter = list_future.erase(future_iter);
      }
      else
      {
        future_iter++;
      }
    } // end while (future_iter != list_future.end())
  }   // end while (list_iter != list.end())
  return done_amount;
}
