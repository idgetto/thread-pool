#ifndef WORKER_H
#define WORKER_H

#include <memory>
#include <thread>

#include "iwork_provider.h"

using std::unique_ptr;
using std::thread;

class Worker {
    public:
        Worker(IWorkProvider *wp) : _wp{wp}, _working{false} {}
        Worker(const Worker& worker) = delete;
        void start();
        void stop();

    private:
        void work();
        
        IWorkProvider *_wp;
        unique_ptr<thread> _thread;
        bool _working;
};

#endif
