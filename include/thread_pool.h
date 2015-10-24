#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#define RESULT_TYPE(T) typename result_of<T()>::type

#include <future>
#include <type_traits>
#include <vector>

#include "work_provider.h"
#include "worker.h"

using std::result_of;
using std::promise;
using std::future;
using std::vector;
using std::shared_ptr;

class ThreadPool {
    static constexpr int NUM_WORKERS = 5;

    public:
        ThreadPool() : _work_provider{} {
            for (int i = 0; i < NUM_WORKERS; ++i) {
                unique_ptr<Worker> worker(new Worker(&_work_provider));
                worker->start();
                _workers.push_back(std::move(worker));
            }
        }
        ~ThreadPool() {
            for (auto& worker : _workers) {
                worker->stop();
            }
        }

        template <typename F>
        future<RESULT_TYPE(F)> post(F func) {
            using prom_t = promise<RESULT_TYPE(F)>;
            shared_ptr<prom_t> prom(new prom_t);
            auto fut = prom->get_future();

            auto task = [prom, &func] {
                prom->set_value(std::move(func()));
            };

            _work_provider.postWork(std::move(task));
            return std::move(fut);
        }
        
    private:
        WorkProvider _work_provider;
        vector<unique_ptr<Worker>> _workers;
};

#endif
