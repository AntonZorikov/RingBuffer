#include "headers/ringbuffer.hp"
#include "headers/ringbuffer_client.hp"
#include <thread>
#include <ctime>
#include <fstream>

int main(int argc, char **argv){
    RingBuffer<Data<int>> rb(100, 2, 100);
    WriterFactory_WithoutDelay_100Size_Min0Max100<Data<int>> writer_factory;
    RingBufferClientWriter<Data<int>> *writer = writer_factory.createWriter(&rb);

    ReaderFactory_100Values<Data<int>> reader_factory;
    RingBufferClientReader<Data<int>> *reader_1 = reader_factory.createReader(&rb);
    RingBufferClientReader<Data<int>> *reader_2 = reader_factory.createReader(&rb);

    writer->generateNewValues();
    thread writer_thread(&RingBufferClientWriter<Data<int>>::write, writer);
    thread reader_thread_1(&RingBufferClientReader<Data<int>>::read, reader_1);
    thread reader_thread_2(&RingBufferClientReader<Data<int>>::read, reader_2);

    writer_thread.join();
    reader_thread_1.join();
    reader_thread_2.join();
}
