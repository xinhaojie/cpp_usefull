#include "tiny_message_queue.hpp"

namespace tiny_message_queue {

template<typename T>
void MessageQueue<T>::enqueue(T message) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(std::move(message));
    lock.unlock();
    cv_.notify_one();
}

template<typename T>
T MessageQueue<T>::dequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty(); });
    T message = std::move(queue_.front());
    queue_.pop();
    return message;
}

template<typename T>
std::optional<T> MessageQueue<T>::try_dequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return std::nullopt;
    }
    T message = std::move(queue_.front());
    queue_.pop();
    return message;
}

template<typename T>
size_t MessageQueue<T>::size() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.size();
}

template<typename T>
bool MessageQueue<T>::empty() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
}

// 显式实例化，这里以 int 类型为例，若要支持其他类型，需要添加相应的显式实例化
template class MessageQueue<int>;

} // namespace MessageQueueLib