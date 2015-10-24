#include <exception>

#include "work_provider.h"

function<void()> WorkProvider::getWork() {
    // if there is no work on the queue
    // then return an empty function
    if (_work_queue.empty()) {
        return [] {};
    }

    // if the queue becomes empty
    // then it will throw an exception
    try {
        return _work_queue.pop();
    } catch (std::exception& e) {
        return [] {};
    }
}

void WorkProvider::postWork(function<void()> func) {
    _work_queue.push(std::move(func));
}

bool WorkProvider::hasWork() {
    return !_work_queue.empty();
}
