#include<iostream>
#include<windows.h>
using namespace std;

int main()
{
    while(1)
    {
        for(int i=1;i<255;i++)
        {
            if(GetAsyncKeyState(i))
            {
                cout<<"keypressed 0x"<<hex<<i<<" "<<(char)i<<"\n";
                Sleep(100);
            }
        }
    }    
}