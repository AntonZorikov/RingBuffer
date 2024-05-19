#include "ringbuffer.hp"
#include <thread>
#include <vector>
#include <array>
#include <ctime>
#include <chrono>
#include <math.h>
#include <random>
#include <fstream>
#include <map>

#ifndef RINGBUFFER_CLIENT_H
#define RINGBUFFER_CLIENT_H

template<typename T>
class Client{
protected:
    RingBuffer<T> *rb;
    unsigned int sleep_time;

    Client(RingBuffer<T> *rb_, unsigned int step_time_){
        this->rb = rb_;
        this->sleep_time = step_time_;
    }

    Client(RingBuffer<T> *rb_){
        this->rb = rb_;
    }
};

template<typename T>
class RingBufferClientWriter : public Client<T>{
private:
    unsigned int size;
    T matrix;
    int min_generated_value, max_generated_value;

public:
    void generateNewValues(){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(min_generated_value, max_generated_value);
        matrix = T(size);
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                matrix.set(dist(gen), i, j);
            }
        }
    }

    void write() {
        for(size_t i = 0; i < size + 1; i++){
            double start = clock();
            Client<T>::rb->add(matrix);
            generateNewValues();
            this_thread::sleep_for(chrono::milliseconds(Client<T>::sleep_time));
            double end = clock();
            cout << "Write time: " + to_string(end - start) << endl;
        }
    }

    RingBufferClientWriter(RingBuffer<T> *rb_, unsigned int step_time_, unsigned int size_,
                     int min_generated_value_, int max_generated_value_) : Client<T>(rb_, step_time_){
        if(max_generated_value_ <= min_generated_value_){
            throw invalid_argument("max_generated_value must be more than min_generated_value");
        }
        if(size_ == 0){
            throw invalid_argument("values_quantity cannot be zero");
        }

        this->size = size_;
        this->min_generated_value = min_generated_value_;
        this->max_generated_value = max_generated_value_;

        generateNewValues();
    }
};

template<typename T>
class RingBufferClientReader : public Client<T>{
private:
    unsigned int values_quantity;
    static inline unsigned int x{};

public:
    void read() {
        for(size_t i = 0; i < values_quantity; i++){
            fstream file;
            map<int, int> values;
            double start = clock();
            T data = Client<T>::rb->get();
        }
    }

    RingBufferClientReader(RingBuffer<T> *rb_, unsigned int values_quantity_) : Client<T>(rb_){
        if(values_quantity_ == 0){
            throw invalid_argument("values_quantity cannot be zero");
        }

        this->values_quantity = values_quantity_;
    }
};

template<typename T>
class ReaderFactory{
protected:
    virtual RingBufferClientReader<T>* createReader(RingBuffer<T> *rb_);
};

template<typename T>
class ReaderFactory_WithoutDelay_100Values : ReaderFactory<T>{
public:
    RingBufferClientReader<T>* createReader(RingBuffer<T> *rb_) override{
        return new RingBufferClientReader<T>(rb_, 0, 100);
    }
};

template<typename T>
class ReaderFactory_1000000Values : ReaderFactory<T>{
public:
    RingBufferClientReader<T>* createReader(RingBuffer<T> *rb_) override{
        return new RingBufferClientReader<T>(rb_, 10000000);
    }
};

template<typename T>
class ReaderFactory_100Values : ReaderFactory<T>{
public:
    RingBufferClientReader<T>* createReader(RingBuffer<T> *rb_) override{
        return new RingBufferClientReader<T>(rb_, 100);
    }
};

template<typename T>
class WriterFactory {
public:
    static RingBufferClientWriter<T>* createWriter(RingBuffer<T> *rb_);
};

template<typename T>
class WriterFactory_WithoutDelay_100Size_Min0Max100 : WriterFactory<T> {
public:
    static RingBufferClientWriter<T>* createWriter(RingBuffer<T> *rb_){
        return new RingBufferClientWriter<T>(rb_, 0, 100, 0, 100);
    }
};

template<typename T>
class WriterFactory_WithoutDelay_1000000Size_Min0Max100 : WriterFactory<T> {
public:
    static RingBufferClientWriter<T>* createWriter(RingBuffer<T> *rb_){
        return new RingBufferClientWriter<T>(rb_, 0, 1000000, 0, 100);
    }
};

template<typename T>
class WriterFactory_WithoutDelay_100Size_Min0Max10 : WriterFactory<T> {
public:
    static RingBufferClientWriter<T>* createWriter(RingBuffer<T> *rb_){
        return new RingBufferClientWriter<T>(rb_, 0, 100, 0, 10);
    }
};


template<typename T>
class WriterFactory_100Delay_100Size_Min0Max100 : WriterFactory<T> {
public:
    static RingBufferClientWriter<T>* createWriter(RingBuffer<T> *rb_){
        return new RingBufferClientWriter<T>(rb_, 100, 100, 0, 100);
    }
};

template<typename T>
class WriterFactory_10000Delay_100Size_Min0Max100 : WriterFactory<T> {
public:
    static RingBufferClientWriter<T>* createWriter(RingBuffer<T> *rb_){
        return new RingBufferClientWriter<T>(rb_, 10000, 100, 0, 100);
    }
};


#endif
