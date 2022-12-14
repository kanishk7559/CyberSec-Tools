#include<iostream>
#include<windows.h>
#include<cstdlib>
#include<time.h>
#include<chrono>
using namespace std;

bool basic()
{
    return IsDebuggerPresent();         //windows library function
}


bool advanced(){                    //checks time between two consecutive time instructions,
                                    //ideally gap should be 0 microseconds, else its using a debugger

    auto start = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::high_resolution_clock::now() - start;

    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
    if(microseconds)
        return true;
    else
        return false;
}


int main(){
    if(advanced())
        cout<<"DEBUG"<<endl;
    else
        cout<<"NOPE"<<endl;
}
