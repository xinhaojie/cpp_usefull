/****************************************************************************************************
* @Author       : xinhaojie xinhaojie@qq.com
* @Date         : 2025-03-17 11:17:36
* @LastEditors  : xinhaojie xinhaojie@qq.com
* @LastEditTime : 2025-03-17 11:17:41
* @FilePath     : /cpp_usefull/msg_queue/test.cpp
* @Description  :
* @Copyright (c) 2025 by xinhaojie@qq.com, All Rights Reserved.
****************************************************************************************************/
#include "tiny_message_queue.hpp"
#include <thread>
#include <iostream>
#include <vector>

namespace tiny_message_queue {

void producer(MessageQueue<int>& queue) {
    for (int i = 0; i < 10; ++i) {
        queue.enqueue(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(MessageQueue<int>& queue) {
    for (int i = 0; i < 10; ++i) {
        int message = queue.dequeue();
        std::cout << "Consumed: " << message << std::endl;
    }
}

} // namespace tiny_message_queue

int main() {
    tiny_message_queue::MessageQueue<int> queue;

    std::thread producerThread(tiny_message_queue::producer, std::ref(queue));
    std::thread consumerThread(tiny_message_queue::consumer, std::ref(queue));

    producerThread.join();
    consumerThread.join();

    return 0;
}