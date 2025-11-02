#include"manyqueue.h"
QueueManager::QueueManager(unsigned int m){
    //TODO
    queues.resize(m + 1);
    size = m;
}
QueueManager::~QueueManager(){
    //TODO
    
}
void QueueManager::push(unsigned int k, unsigned int x){
    //TODO
    queues[k].qpush(x);
}
void QueueManager::pop(unsigned int k){
    //TODO
    queues[k].qpop();
}
unsigned int QueueManager::query(unsigned int k, unsigned int i){
    //TODO
    return queues[k].query(i);
}