#pragma once
#include <functional>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <future>
class Task {
public:
	Task(std::function<void()> func) : m_func(func) {}
	void operator()() { m_func(); }
private:
	std::function<void()> m_func;
};

class ThreadPool2 {
public:
	ThreadPool2(size_t numThreads) {
		for (size_t i = 0; i < numThreads; ++i) {
			m_threads.emplace_back([this] {
				while (true) {
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(m_mutex);
						m_condition.wait(lock, [this] {
							return m_stop || !m_tasks.empty();
							});
						if (m_stop && m_tasks.empty()) {
							return;
						}
						task = std::move(m_tasks.front());
						m_tasks.pop();
					}
					task();
				}
				});
		}
	}

	~ThreadPool2() {
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_stop = true;
		}
		m_condition.notify_all();
		for (std::thread& thread : m_threads) {
			thread.join();
		}
	}

	template<typename F, typename... Args>
	auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {

		using return_type = decltype(f(args...));
		auto task = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);
		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			if (m_stop) {
				throw std::runtime_error("enqueue on stopped ThreadPool");
			}
			m_tasks.emplace([task]() { (*task)(); });
		}
		m_condition.notify_one();
		return res;
	}
private:
	std::vector<std::thread> m_threads;
	std::queue<std::function<void()>> m_tasks;
	std::mutex m_mutex;
	std::condition_variable m_condition;
	bool m_stop = false;
};