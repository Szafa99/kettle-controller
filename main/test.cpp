// #define cos
#ifdef cos

#include <iostream>
#include <cstring>
#include <string>

class Network{

public:

static Network& getInstance(){
    static Network instance;
    return instance;
}

void (Network::*update)()=NULL;

void operator=(Network const&) = delete;
Network(Network const&) = delete;
    
void updateDS8B20(){
    printf("cos");
}

void test(){
    printf("cos3");
}

private:
Network(){}

};

int main(){

if(Network::getInstance().update ==NULL)printf("is null");
Network::getInstance().update = &Network::test;
( Network::getInstance().*(Network::getInstance().update) )(); 

}


#endif