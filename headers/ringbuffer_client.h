#include "ringbuffer.h"
#include <thread>
#include <vector>
#include <array>
#include <ctime>
#include <chrono>

#ifndef RINGBUFFER_CLIENT_H
#define RINGBUFFER_CLIENT_H

template<typename T>
class RingBufferClientWriter{
private:
    RingBuffer<T> *rb;
    unsigned int sleep_time, rows_cols_quantity;
    int** matrix;
    int min_generated_value, max_generated_value;

public:
    void generateNewValues(){
        srand(time(0));

        for(int i = 0; i < rows_cols_quantity; i++){
            for(int j = 0; j < rows_cols_quantity; j++){
                matrix[i][j] = rand() % (max_generated_value - min_generated_value + 1) + min_generated_value;
            }
        }
    }

    void write(){
        for(size_t i = 0; i < rows_cols_quantity; i++){
            for(size_t j = 0; j < rows_cols_quantity; j++){
                rb->add(matrix[i][j]);
                this_thread::sleep_for(chrono::milliseconds(sleep_time));
            }
        }
    }

    RingBufferClientWriter(RingBuffer<int> *rb_, unsigned int step_time_, unsigned int rows_cols_quantity_,
                     int min_generated_value_, int max_generated_value_){
        if(max_generated_value_ <= min_generated_value_){
            throw invalid_argument("max_generated_value must be more than min_generated_value");
        }
        if(rows_cols_quantity_ == 0){
            throw invalid_argument("values_quantity cannot be zero");
        }

        this->rb = rb_;
        this->sleep_time = step_time_;
        this->rows_cols_quantity = rows_cols_quantity_;
        this->min_generated_value = min_generated_value_;
        this->max_generated_value = max_generated_value_;

        matrix = new int* [rows_cols_quantity];
        for(int i = 0; i < rows_cols_quantity_; i++){
            matrix[i] = new int[rows_cols_quantity];
        }

        generateNewValues();
    }
};

template<typename T>
class RingBufferClientReader{
private:
    RingBuffer<T> *rb;
    unsigned int sleep_time, values_quantity;

public:
    void read(){
        for(size_t i = 0; i < values_quantity; i++){
            rb->get();
            this_thread::sleep_for(chrono::milliseconds(sleep_time));
        }
    }

    RingBufferClientReader(RingBuffer<int> *rb_, unsigned int step_time_, unsigned int values_quantity_){
        if(values_quantity_ == 0){
            throw invalid_argument("values_quantity cannot be zero");
        }

        this->rb = rb_;
        this->sleep_time = step_time_;
        this->values_quantity = values_quantity_;
    }
};




#endif // RINGBUFFER_CLIENT_H
