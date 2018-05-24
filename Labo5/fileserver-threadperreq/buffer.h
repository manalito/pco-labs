#ifndef BUFFER_H
#define BUFFER_H

#include"abstractbuffer.h"


class Buffer;

template<typename T>
class Buffer: public AbstractBuffer{
private:
    QList<T> requests;
public:
    Buffer();

    void put(T item){
        requests.push_back(item);
    }
    Buffer get(){
        return requests.pop_front();
    }
};

#endif // BUFFER_H
