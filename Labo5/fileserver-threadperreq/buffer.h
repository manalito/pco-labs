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
        requests.push(item);
    }
    Buffer get(){
        return requests.size() - 1;
    }
};

#endif // BUFFER_H
