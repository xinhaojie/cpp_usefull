#pragma once

#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace tiny_task {
class ScheduledTask {
public:
    ScheduledTask();
    ~ScheduledTask();

    // 添加定时任务
    void add_task(const std::function<void()>& task, const std::chrono::system_clock::time_point& startTime, const std::chrono::milliseconds& interval = std::chrono::milliseconds(0));
    // 添加每天执行的任务
    void add_daily_task(const std::function<void()>& task, int hour, int minute, int second);
    // 添加每周执行的任务
    void add_weekly_task(const std::function<void()>& task, int dayOfWeek, int hour, int minute, int second);
    // 添加每月执行的任务
    void add_monthly_task(const std::function<void()>& task, int dayOfMonth, int hour, int minute, int second);
    // 添加每分钟执行的任务
    void add_minutely_task(const std::function<void()>& task);
    // 增加定时多少时间后执行一次的任务
    void add_delayed_task(const std::function<void()>& task, const std::chrono::milliseconds& delay);
    // 停止所有任务
    void stop_all_tasks();

private:
    struct Task {
        std::function<void()> task;
        std::chrono::system_clock::time_point nextExecution;
        std::chrono::milliseconds interval;
        bool isStopped = false;

        bool operator>(const Task& other) const {
            return nextExecution > other.nextExecution;
        }
    };

    std::priority_queue<Task, std::vector<Task>, std::greater<Task>> taskQueue;
    std::mutex taskMutex;
    std::condition_variable taskCV;
    std::thread workerThread;
    bool running = true;

    void worker();
    std::chrono::system_clock::time_point get_next_execution_time(int dayOfWeek, int dayOfMonth, int hour, int minute, int second);
    int get_days_in_month(int year, int month);
};
} // namespace tiny_task