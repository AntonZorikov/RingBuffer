#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include <mutex>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
class RingBuffer
{
private:
    template<typename T1>
    class Node{
    public:
        T1 value = T1();
        Node<T1> *next = nullptr;
    };

    Node<T> *start, *end, *current, *oldest;
    unsigned int size;
    unsigned int item_count;
    unsigned short reader_count;
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

        T res = oldest->value;


        if(find(now_read.begin(), now_read.end(), oldest) != now_read.end()){
            now_read.erase(find(now_read.begin(), now_read.end(), oldest));
        }

        if(item_count > 0){
            lock_guard<mutex> loock(mtx);

            oldest->value = T();
            oldest = oldest->next;
            item_count--;
        }

        return res;
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

    RingBuffer(unsigned int size_, unsigned short reader_count){
        if(size_ == 0 || reader_count == 0){
            throw invalid_argument("zero size is not allowed");
        }

        if(reader_count >= size_){
            throw invalid_argument("readerCount must be less than size");
        }

        this->size = size_;
        this->reader_count = reader_count;

        Node<T> *newNode = new Node<T>();

        start = newNode;
        for(unsigned int i = 0; i < size - 1; i++){
            newNode->next = new Node<T>();
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
