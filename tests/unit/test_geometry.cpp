// TexMol Unit Tests — Geometry::IntQueue
#include <gtest/gtest.h>
#include <Geometry/IntQueue.h>

TEST(QueueTest, EmptyOnConstruction) {
    Queue q;
    EXPECT_TRUE(q.isEmpty());
}

TEST(QueueTest, EnqueueDequeue) {
    Queue q;
    q.enQueue(42);
    EXPECT_FALSE(q.isEmpty());
    int v = q.deQueue();
    EXPECT_EQ(v, 42);
    EXPECT_TRUE(q.isEmpty());
}

TEST(QueueTest, FIFOOrder) {
    Queue q;
    q.enQueue(1);
    q.enQueue(2);
    q.enQueue(3);
    EXPECT_EQ(q.deQueue(), 1);
    EXPECT_EQ(q.deQueue(), 2);
    EXPECT_EQ(q.deQueue(), 3);
    EXPECT_TRUE(q.isEmpty());
}

TEST(QueueTest, ClearQueue) {
    Queue q;
    q.enQueue(1);
    q.enQueue(2);
    q.enQueue(3);
    q.clearQueue();
    EXPECT_TRUE(q.isEmpty());
}

TEST(QueueTest, InterleavedEnqueueDequeue) {
    Queue q;
    q.enQueue(10);
    q.enQueue(20);
    EXPECT_EQ(q.deQueue(), 10);
    q.enQueue(30);
    EXPECT_EQ(q.deQueue(), 20);
    EXPECT_EQ(q.deQueue(), 30);
    EXPECT_TRUE(q.isEmpty());
}

TEST(QueueTest, ManyElements) {
    Queue q;
    for (int i = 0; i < 100; i++) {
        q.enQueue(i);
    }
    for (int i = 0; i < 100; i++) {
        EXPECT_EQ(q.deQueue(), i);
    }
    EXPECT_TRUE(q.isEmpty());
}
