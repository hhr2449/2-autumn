#include"manyqueue.h"
QueueManager::QueueManager(unsigned int m){
    //TODO
    queues = new Queue*[m + 1];
    for(int i = 1; i <= m; i++) {
        queues[i] = new Queue();
    }
    size = m;
}
QueueManager::~QueueManager(){
    //TODO
    for(int i = 1; i <= size; i++) {
        delete queues[i];
    }
    delete [] queues;
}
void QueueManager::push(unsigned int k, unsigned int x){
    //TODO
    queues[k]->qpush(x);
}
void QueueManager::pop(unsigned int k){
    //TODO
    queues[k]->qpop();
}
unsigned int QueueManager::query(unsigned int k, unsigned int i){
    //TODO
    return queues[k]->query(i);
}