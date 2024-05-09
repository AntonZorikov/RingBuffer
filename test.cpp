#include "googletest-release-1.12.1/googletest/include/gtest/gtest.h"
#include "headers/ringbuffer.h"
#include "headers/ringbuffer_client.h"
#include <thread>
#include <ctime>

using namespace std;

TEST(init_test, positive_test_1){
    RingBuffer<int> rb(5, 3);

    EXPECT_EQ(rb.getSize(), 5);
}

TEST(init_test, negative_test_1){
    EXPECT_ANY_THROW(RingBuffer<int> rb(0, 3));
}

TEST(add_tests, positive_test_1){
    RingBuffer<int> rb(5, 3);
    rb.add(1);
    rb.add(2);
    rb.add(3);

    EXPECT_EQ(rb.getItemCount(), 3);
}

TEST(add_tests, positive_test_2){
    RingBuffer<int> rb(5, 3);

    EXPECT_EQ(rb.getItemCount(), 0);
}

TEST(add_tests, positive_test_3){
    RingBuffer<int> rb(5, 3);

    rb.add(1);
    rb.add(2);
    rb.add(3);
    rb.add(4);
    rb.add(5);

    EXPECT_EQ(rb.getItemCount(), 5);
}

TEST(add_tests, positive_test_4){
    RingBuffer<int> rb(5, 3);

    rb.add(1);
    rb.add(2);
    rb.add(3);
    rb.add(4);
    rb.add(5);
    rb.add(6);

    EXPECT_EQ(rb.getItemCount(), 5);
}

TEST(remove_tests, positive_test_1){
    RingBuffer<int> rb(5, 3);

    rb.add(1);
    rb.add(2);
    rb.add(3);
    rb.add(4);
    rb.add(5);

    rb.get();

    EXPECT_EQ(rb.getItemCount(), 4);
}

TEST(remove_tests, positive_test_2){
    RingBuffer<int> rb(5, 3);

    rb.get();

    EXPECT_EQ(rb.getItemCount(), 0);
}

TEST(remove_tests, positive_test_3){
    RingBuffer<int> rb(5, 3);

    rb.add(1);
    rb.add(2);
    rb.add(3);
    rb.add(4);
    rb.add(5);

    rb.get();
    rb.get();
    rb.get();
    rb.get();
    rb.get();

    EXPECT_EQ(rb.getItemCount(), 0);
}

TEST(get_tests, positive_test_1){
    RingBuffer<int> rb(5, 3);

    EXPECT_EQ(rb.get(), NULL);
}

TEST(get_tests, positive_test_2){
    RingBuffer<int> rb(5, 3);

    EXPECT_EQ(rb.get(), NULL);
}

TEST(get_tests, positive_test_3){
    RingBuffer<int> rb(5, 3);

    rb.add(1);
    rb.add(2);
    rb.add(3);
    rb.add(4);
    rb.add(5);

    EXPECT_EQ(rb.get(), 1);
}

TEST(get_tests, positive_test_4){
    RingBuffer<int> rb(5, 3);

    rb.add(1);
    rb.add(2);
    rb.add(3);
    rb.add(4);
    rb.add(5);

    rb.get();

    EXPECT_EQ(rb.get(), 2);
}

TEST(is_full_tests, positive_test_1){
    RingBuffer<int> rb(5, 3);

    rb.add(1);
    rb.add(2);
    rb.add(3);
    rb.add(4);
    rb.add(5);

    EXPECT_TRUE(rb.isFull());
}

TEST(is_full_tests, positive_test_2){
    RingBuffer<int> rb(5, 3);

    EXPECT_FALSE(rb.isFull());
}

TEST(is_full_tests, positive_test_3){
    RingBuffer<int> rb(5, 3);

    rb.add(1);
    rb.add(2);
    rb.add(3);

    EXPECT_FALSE(rb.isFull());
}

TEST(is_full_tests, positive_test_4){
    RingBuffer<int> rb(5, 3);

    rb.add(1);
    rb.add(2);
    rb.add(3);
    rb.add(4);
    rb.add(5);

    rb.get();

    EXPECT_FALSE(rb.isFull());
}

TEST(is_empty_tests, positive_test_1){
    RingBuffer<int> rb(5, 3);

    EXPECT_TRUE(rb.isEmpty());
}

TEST(is_empty_tests, positive_test_2){
    RingBuffer<int> rb(5, 3);

    rb.add(1);

    EXPECT_FALSE(rb.isEmpty());
}

void add_help_func(int n, RingBuffer<int> *rb){
    for(int i = 0; i < n; i++){
        rb->add(i);
    }
}

void get_help_func(int n, RingBuffer<int> *rb){
    for(int i = 0; i < n; i++){
        rb->get();
    }
}

TEST(mt_tests, positive_test_1){
    RingBuffer<int> rb(500, 3);

    add_help_func(500, &rb);

    thread th2(get_help_func, 250, &rb);
    thread th3(get_help_func, 250, &rb);

    th2.join();
    th3.join();

    EXPECT_TRUE(rb.isEmpty());
}

TEST(rb_client_writer_tests, positive_test_1){
    RingBuffer<int> rb(10, 3);

    RingBufferClientWriter<int> rb_client_writer(&rb, 0, 10, -100, 100);
    rb_client_writer.write();

    EXPECT_TRUE(rb.isFull());
}

TEST(rb_client_writer_tests, negative_test_1){
    RingBuffer<int> rb(10, 3);

    EXPECT_ANY_THROW(RingBufferClientWriter<int> rb_client_writer(&rb, 0, 10, 1000, 100););
}

TEST(rb_client_writer_tests, negative_test_2){
    RingBuffer<int> rb(10, 3);

    EXPECT_ANY_THROW(RingBufferClientWriter<int> rb_client_writer(&rb, 0, 0, -100, 100););
}

TEST(rb_client_reader_tests, positive_test_1){
    RingBuffer<int> rb(10, 3);

    RingBufferClientWriter<int> rb_client_writer(&rb, 0, 10, -100, 100);
    rb_client_writer.write();

    RingBufferClientReader<int> rb_client_reader(&rb, 0, 10);
    rb_client_reader.read();

    EXPECT_TRUE(rb.isEmpty());
}

TEST(rb_client_reader_tests, negative_test_1){
    RingBuffer<int> rb(10, 3);

    EXPECT_ANY_THROW(RingBufferClientReader<int> rb_client_reader(&rb, 0, 0););
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

