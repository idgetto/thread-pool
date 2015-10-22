#include <cstdlib>
#include <thread>

#include "gtest/gtest.h"
#include "locking_queue.h"

using std::thread;

TEST(LockingQueue, push) {
    LockingQueue<int> q;

    EXPECT_TRUE(q.empty());
    q.push(1);
    EXPECT_EQ(1, q.size()); 
    q.push(4);
    EXPECT_EQ(2, q.size()); 
}

TEST(LockingQueue, pop) {
    LockingQueue<int> q;

    EXPECT_TRUE(q.empty());
    q.push(1);
    EXPECT_EQ(1, q.size()); 
    q.push(4);
    EXPECT_EQ(2, q.size()); 

    EXPECT_EQ(1, q.pop());
    EXPECT_EQ(1, q.size()); 
    EXPECT_EQ(4, q.pop());
    EXPECT_TRUE(q.empty()); 
}

TEST(LockingQueue, MultiThreadPush) {
    LockingQueue<int> q;

    auto pushStuff = [&q] {
        for (int times = 0; times < 5; ++times) {
            q.push(rand() % 100);
        }
    };

    EXPECT_TRUE(q.empty());

    thread t1(pushStuff);
    thread t2(pushStuff);
    thread t3(pushStuff);
    thread t4(pushStuff);
    thread t5(pushStuff);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    EXPECT_EQ(25, q.size()); 
}

TEST(LockingQueue, MultiThreadPop) {
    LockingQueue<int> q;

    auto pushStuff = [&q] {
        for (int times = 0; times < 5; ++times) {
            q.push(rand() % 100);
        }
    };

    EXPECT_TRUE(q.empty());

    thread t1(pushStuff);
    thread t2(pushStuff);
    thread t3(pushStuff);
    thread t4(pushStuff);
    thread t5(pushStuff);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    EXPECT_EQ(25, q.size()); 

    auto popStuff = [&q] {
        for (int times = 0; times < 5; ++times) {
            q.pop();
        }
    };

    thread t6(popStuff);
    thread t7(popStuff);
    thread t8(popStuff);
    thread t9(popStuff);
    thread t10(popStuff);
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    t10.join();

    EXPECT_TRUE(q.empty());
}
