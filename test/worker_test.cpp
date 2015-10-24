#include "gtest/gtest.h"
#include "worker.h"

#include <functional>
#include <chrono>

using std::function;

TEST(Worker, work) {
    bool changed = false;

    class FakeWorkProvider : public IWorkProvider {
        public:
            FakeWorkProvider(bool *changed) : _changed{changed} {}

            function<void()> getWork() {
                return [this] {
                    *_changed = true;
                };
            }

            bool hasWork() {
                return !*_changed;
            }

        private:
            bool *_changed;
    };

    FakeWorkProvider wp(&changed);
    Worker worker(&wp);

    EXPECT_FALSE(changed);
    worker.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    worker.stop();
    EXPECT_TRUE(changed);
}
