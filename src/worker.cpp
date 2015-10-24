#include "worker.h"
#include <iostream>

void Worker::start() {
    _working = true;
    thread *t = new thread([this] {
        work();
    });

    _thread = unique_ptr<thread>(t);
}

void Worker::stop() {
    _working = false;
    _thread->join();
}

void Worker::work() {
    while (_working) {
        if (_wp->hasWork()) {
            auto task = _wp->getWork();
            task();
        }
    }
}
