#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include <mutex>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
class Data{
protected:
    T** value = T();
    unsigned int size;

public:
    virtual T** get(){
        return value;
    }

    virtual void set(T** value_){
        for(unsigned int i = 0; i < size; i++){
            for(unsigned int j = 0; j < size; j++){
                value[i][j] = value_[i][j];
            }
        }
    }

    virtual void set(T value_, unsigned int i, unsigned int j){
        value[i][j] = value_;
    }

    virtual void clear(){
        for(unsigned int i = 0; i < size; i++){
            for(unsigned int j = 0; j < size; j++){
                value[i][j] = NULL;
            }
        }
    }

    Data(T** value_, unsigned int size_){
        this->size = size_;

        value = new T* [size];
        for(unsigned int i = 0; i < size; i++){
            value[i] = new T[size];
        }

        for(unsigned int i = 0; i < size; i++){
            for(unsigned int j = 0; j < size; j++){
                value[i][j] = value_[i][j];
            }
        }
    }

    Data(unsigned int size_){
        this->size = size_;

        value = new T* [size];
        for(unsigned int i = 0; i < size; i++){
            value[i] = new T[size];
        }
    }

    Data(){
        value = new T* [1];
        for(unsigned int i = 0; i < 1; i++){
            value[i] = new T[1];
        }
    }
};

//template<typename T>
//class Matrix : public Data<T> {
//protected:
//    T value = T();

//public:
//    T get() override{
//        return value;
//    }

//    void set(T value_) override{
//        this->value = value_;
//    }

//    Matrix(T value_){
//        this->value = value_;

//        matrix = new T* [size];
//        for(unsigned int i = 0; i < size; i++){
//            matrix[i] = new T[size];
//        }

//        for(unsigned int i = 0; i < size; i++){
//            for(unsigned int j = 0; j < size; j++){
//                matrix[i][j] = value_[i][j];
//            }
//        }    }

//    Matrix(){
//        this->value = value_;

//        matrix = new T* [rows_cols_quantity];
//        for(unsigned int i = 0; i < size; i++){
//            matrix[i] = new T[size];
//        }
//    }
//};

template<typename T>
class RingBuffer
{
private:
    template<typename T1>
    class Node{
    public:
        T1 value;
        Node<T1> *next = nullptr;

        Node(unsigned int size){
            value = T1(size);
        }
    };

    Node<T> *start, *end, *current, *oldest;
    unsigned int size;
    unsigned int item_count;
    unsigned short reader_count;
    unsigned int matrix_size;
    vector<Node<T>*> now_read;

    mutex mtx;

    void nextOldest(){
        //lock_guard<mutex> loock(mtx);
        while(find(now_read.begin(), now_read.end(), oldest) != now_read.end()){
            oldest = oldest->next;
        }
    }

public:
    void add(T value){
        while(find(now_read.begin(), now_read.end(), current) != now_read.end()){
            current = current->next;
        }

        if(current == oldest){
            nextOldest();
        }

        current->value = value;

        current = current->next;

        if(item_count < size){
            item_count++;
        }
    }

    T get(){
        if(now_read.size() < reader_count){
            nextOldest();
        }
        else{
            throw logic_error("Reader more than readerCount");
        }




        if(find(now_read.begin(), now_read.end(), oldest) != now_read.end()){
            now_read.erase(find(now_read.begin(), now_read.end(), oldest));
        }

        if(item_count > 0){
            lock_guard<mutex> loock(mtx);

            oldest->value.clear();
            oldest = oldest->next;
            item_count--;
        }

        T res = oldest->value;


//        int** val = res.get();

//        cout << "###" << endl;

//        for(unsigned int i = 0; i < getMatrixSize(); i++){
//            for(unsigned int j = 0; j < getMatrixSize(); j++){
//                cout << val[i][j] << " ";
//            }
//            cout << endl;
//        }

//        cout << "###" << endl;


        return res;
    }

    unsigned int getMatrixSize(){
        return matrix_size;
    }

    bool isFull(){
        return size == item_count;
    }

    bool isEmpty(){
        return item_count == 0;
    }

    unsigned int getSize(){
        return size;
    }

    unsigned int getItemCount(){
        return item_count;
    }

    RingBuffer(unsigned int size_, unsigned short reader_count, unsigned int matrix_size_){
        if(size_ == 0 || reader_count == 0 || matrix_size_ == 0){
            throw invalid_argument("zero size is not allowed");
        }

        if(reader_count >= size_){
            throw invalid_argument("readerCount must be less than size");
        }

        this->size = size_;
        this->reader_count = reader_count;
        this->matrix_size = matrix_size_;

        Node<T> *newNode = new Node<T>(matrix_size);

        start = newNode;
        for(unsigned int i = 0; i < size; i++){
            newNode->next = new Node<T>(matrix_size);
            newNode = newNode->next;
        }

        end = newNode;
        end->next = start;

        current = start;
        oldest = start;
        item_count = 0;
    }

    ~RingBuffer(){
        Node<T> *node = start;
        do{
            Node<T> *del = node;
            node = node->next;
            delete del;
        } while(node != start);
    }

//    //DELETE
//    void display(){
//        Node<T> *currentNode = start;
//        while(currentNode != end){
//            cout << currentNode->value << endl;
//            currentNode = currentNode->next;
//        }
//        cout << end->value << endl;
//    }
};

#endif // RINGBUFFER_H
