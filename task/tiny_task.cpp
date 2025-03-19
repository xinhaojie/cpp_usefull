/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-19 09:14:47
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-19 09:20:57
* @FilePath     : /cpp_usefull/task/tiny_task.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_task.hxx"
#include <ctime>

namespace tiny_task {
ScheduledTask::ScheduledTask() : workerThread(&ScheduledTask::worker, this) {}

ScheduledTask::~ScheduledTask() {
    {
        std::unique_lock<std::mutex> lock(taskMutex);
        running = false;
    }
    taskCV.notify_all();
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void ScheduledTask::add_task(const std::function<void()>& task, const std::chrono::system_clock::time_point& startTime, const std::chrono::milliseconds& interval) {
    std::unique_lock<std::mutex> lock(taskMutex);
    taskQueue.push({task, startTime, interval});
    taskCV.notify_all();
}

void ScheduledTask::add_daily_task(const std::function<void()>& task, int hour, int minute, int second) {
    auto nextTime = get_next_execution_time(-1, -1, hour, minute, second);
    add_task(task, nextTime, std::chrono::hours(24));
}

void ScheduledTask::add_weekly_task(const std::function<void()>& task, int dayOfWeek, int hour, int minute, int second) {
    auto nextTime = get_next_execution_time(dayOfWeek, -1, hour, minute, second);
    add_task(task, nextTime, std::chrono::hours(24 * 7));
}

void ScheduledTask::add_monthly_task(const std::function<void()>& task, int dayOfMonth, int hour, int minute, int second) {
    auto nextTime = get_next_execution_time(-1, dayOfMonth, hour, minute, second);
    add_task(task, nextTime, std::chrono::milliseconds(0));
    // 重新计算下一次执行时间，考虑每月天数不同
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);
    int year = now_tm->tm_year + 1900;
    int month = now_tm->tm_mon + 1;
    int daysInMonth = get_days_in_month(year, month);
    if (dayOfMonth > daysInMonth) {
        dayOfMonth = daysInMonth;
    }
    now_tm->tm_mon++;
    if (now_tm->tm_mon >= 12) {
        now_tm->tm_mon = 0;
        now_tm->tm_year++;
    }
    now_tm->tm_mday = dayOfMonth;
    now_tm->tm_hour = hour;
    now_tm->tm_min = minute;
    now_tm->tm_sec = second;
    std::time_t next_c = std::mktime(now_tm);
    auto newNextTime = std::chrono::system_clock::from_time_t(next_c);
    add_task(task, newNextTime, std::chrono::milliseconds(0));
}

void ScheduledTask::add_minutely_task(const std::function<void()>& task) {
    auto now = std::chrono::system_clock::now();
    auto nextTime = std::chrono::time_point_cast<std::chrono::minutes>(now) + std::chrono::minutes(1);
    add_task(task, nextTime, std::chrono::minutes(1));
}

// 增加定时多少时间后执行一次的任务的实现
void ScheduledTask::add_delayed_task(const std::function<void()>& task, const std::chrono::milliseconds& delay) {
    auto startTime = std::chrono::system_clock::now() + delay;
    add_task(task, startTime, std::chrono::milliseconds(0));
}

void ScheduledTask::stop_all_tasks() {
    std::unique_lock<std::mutex> lock(taskMutex);
    while (!taskQueue.empty()) {
        auto task = taskQueue.top();
        task.isStopped = true;
        taskQueue.pop();
    }
}

void ScheduledTask::worker() {
    while (running) {
        std::unique_lock<std::mutex> lock(taskMutex);
        if (taskQueue.empty()) {
            taskCV.wait(lock, [this] { return!taskQueue.empty() ||!running; });
        }
        if (!running) {
            break;
        }
        auto nextTask = taskQueue.top();
        if (std::chrono::system_clock::now() < nextTask.nextExecution) {
            taskCV.wait_until(lock, nextTask.nextExecution, [this] { return!running; });
        }
        if (!running) {
            break;
        }
        taskQueue.pop();
        if (!nextTask.isStopped) {
            std::thread([nextTask]() { nextTask.task(); }).detach();
            if (nextTask.interval.count() > 0) {
                nextTask.nextExecution += nextTask.interval;
                taskQueue.push(nextTask);
            }
        }
    }
}

std::chrono::system_clock::time_point ScheduledTask::get_next_execution_time(int dayOfWeek, int dayOfMonth, int hour, int minute, int second) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    now_tm->tm_hour = hour;
    now_tm->tm_min = minute;
    now_tm->tm_sec = second;

    if (dayOfWeek != -1) {
        int currentDay = now_tm->tm_wday;
        int daysToAdd = (dayOfWeek - currentDay + 7) % 7;
        if (daysToAdd == 0 && std::mktime(now_tm) < now_c) {
            daysToAdd = 7;
        }
        now_tm->tm_mday += daysToAdd;
    } else if (dayOfMonth != -1) {
        if (dayOfMonth < now_tm->tm_mday || (dayOfMonth == now_tm->tm_mday && std::mktime(now_tm) < now_c)) {
            now_tm->tm_mon++;
        }
        now_tm->tm_mday = dayOfMonth;
    }

    std::time_t next_c = std::mktime(now_tm);
    return std::chrono::system_clock::from_time_t(next_c);
}

int ScheduledTask::get_days_in_month(int year, int month) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        return 29;
    }
    return days[month - 1];
}
} // namespace tiny_task