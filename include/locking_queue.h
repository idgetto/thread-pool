#ifndef LOCKING_QUEUE_H
#define LOCKING_QUEUE_H

#include <mutex>
#include <queue>
#include <deque>
#include <iostream>
#include <exception>
#include <vector>
#include <string>

using std::mutex;
using std::lock_guard;
using std::queue;
using std::deque;
using std::vector;
using std::string;

template <typename T>
class LockingQueue {
    public:
        LockingQueue() : _q{deque<T>()} {
        }

        LockingQueue(const LockingQueue& other) :
            _q{other._q} { }

        void push(T value) {
            lock_guard<mutex> lck(m);
            _q.push(value);
        }

        T pop() {
            lock_guard<mutex> lck(m);

            if (_q.empty()) {
                throw std::exception();
            }

            T value = _q.front();
            _q.pop();

            return value;
        }

        bool empty() {
            lock_guard<mutex> lck(m);
            return _q.empty();
        }

        unsigned size() {
            lock_guard<mutex> lck(m);
            return _q.size();
        }
    private:
        queue<T> _q; 
        mutex m;
};

#endif
