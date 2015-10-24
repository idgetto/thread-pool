#ifndef IWORK_PROVIDER_H
#define IWORK_PROVIDER_H

#include <functional>

using std::function;

class IWorkProvider  {
    public:
        virtual function<void()> getWork() = 0;
        virtual bool hasWork() = 0;
};

#endif
