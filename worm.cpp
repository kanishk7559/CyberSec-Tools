#include<bits/stdc++.h>
#include<windows.h>
using namespace std;

int main(){
    DWORD bitmask=GetLogicalDrives();       //gets all used drive letters in a bitmask

    unordered_set<char> drives;
    cout<<"Enter filename: "<<endl;     //file to copy to each new drive
    string s;
    cin>>s;
    
    for(int i=0;i<26;i++)
    {
        if((1<<i)&bitmask)      //find if current drive exists
        {
            drives.insert('A'+i);
            cout<<char('A'+i)<<endl;
        }
    }


    while(1)
    {
        DWORD bitmask=GetLogicalDrives();
        //cout<<bitmask<<endl;

        for(int i=0;i<26;i++)
        {
            //copy if a new drive was added
            if((1<<i)&bitmask and drives.find('A'+i)==drives.end())
            {
                cout<<"New drive added "<<char('A'+i)<<endl;
                drives.insert('A'+i);
                string tmp;
                tmp+=('A'+i);
                tmp+=":\\";
                tmp+=s;

                CopyFile(s.c_str(),tmp.c_str(),FALSE);
            }

            //remove from list if a drive was removed
            else if(!((1<<i)&bitmask) and drives.find('A'+i)!=drives.end())
            {
                cout<<"drive removed "<<char('A'+i)<<endl;
                drives.erase('A'+i);
            }
        }
        
        Sleep(3000);
    }
}
