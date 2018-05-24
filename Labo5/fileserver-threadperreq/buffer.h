#ifndef BUFFER_H
#define BUFFER_H

#include"abstractbuffer.h"


class Buffer;

template<typename Buffer>
class Buffer: public AbstractBuffer{
public:
    Buffer();

    void put(Buffer item){

    }
    Buffer get(){
        T hello = "fef";
        return hello;
    }
};

#endif // BUFFER_H
