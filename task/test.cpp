#include "tiny_task.hxx"
#include <iostream>
#include <thread>

namespace tiny_task {
void example_task() {
    std::cout << "Task executed at: " << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << std::endl;
}

void example_task1() {
    std::cout << "Task executed at: xxx" << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << std::endl;
}
} // namespace tiny_task

int main() {
    tiny_task::ScheduledTask scheduler;

    // 每分钟执行一次任务
    scheduler.add_minutely_task(tiny_task::example_task);

    // 每天 12:00:00 执行任务
    scheduler.add_daily_task(tiny_task::example_task, 12, 0, 0);

    // 每周周一 14:30:00 执行任务
    scheduler.add_weekly_task(tiny_task::example_task, 1, 14, 30, 0);

    // 每月 31 号 10:00:00 执行任务
    scheduler.add_monthly_task(tiny_task::example_task, 31, 10, 0, 0);

    // 定时 5 秒后执行一次任务
    scheduler.add_delayed_task(tiny_task::example_task1, std::chrono::seconds(5));

    scheduler.add_task(tiny_task::example_task1, std::chrono::system_clock::now(), std::chrono::seconds(1));

    // 等待一段时间
    std::this_thread::sleep_for(std::chrono::minutes(2));

    // 停止所有任务
    scheduler.stop_all_tasks();

    return 0;
}