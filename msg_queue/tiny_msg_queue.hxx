#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

namespace tiny_message_queue {

template<typename T>
class MessageQueue {
public:
    // 向队列中添加消息
    void enqueue(T message);
    // 从队列中取出消息，如果队列为空则阻塞
    T dequeue();
    // 尝试从队列中取出消息，如果队列为空则返回 std::nullopt
    std::optional<T> try_dequeue();
    // 获取队列的大小
    size_t size() const;
    // 检查队列是否为空
    bool empty() const;

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
};

} // namespace MessageQueueLib