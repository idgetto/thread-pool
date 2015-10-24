#ifndef WORK_PROVIDER_H
#define WORK_PROVIDER_H

#include <thread>
#include <functional>

#include "locking_queue.h"
#include "iwork_provider.h"

using std::function;

class WorkProvider : public IWorkProvider {
    public:
        virtual function<void()> getWork();
        void postWork(function<void()> func);
        virtual bool hasWork();
    private:
        LockingQueue<function<void()>> _work_queue;
};

#endif
