#include "MultiFileDownloader.h"

#include <spdlog/spdlog.h>

#include <chrono>
#include <future>
#include <memory>
#include <thread>

#include "FileDownloader.h"

void MultiFileDownloader::Download(const std::list<std::string> &list,
								   const std::string &path,
								   const Config &config) {
	auto thread_amount = config.DownloadThreadAmount();

	std::list<std::future<size_t>> list_future;
	auto list_iter = list.begin();

	while (list_iter != list.end()) {
		// 当线程数量未达到时分配新任务
		if (list_future.size() < thread_amount) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			list_future.push_back(std::async(std::launch::async,
											 FileDownloader::Download, path,
											 *list_iter, config));
			list_iter++;
		}
		// 回收任务
		auto future_iter = list_future.begin();
		while (future_iter != list_future.end()) {
			//完成
			if (future_iter->wait_for(std::chrono::milliseconds(250)) ==
				std::future_status::ready) {
				future_iter = list_future.erase(future_iter);
			} else {
				future_iter++;
			}
		} // end while (future_iter != list_future.end())
	}   // end while (list_iter != list.end())

	// list_future析构时等待任务完成
	list_future.clear();
}