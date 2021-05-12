#include <bits/stdc++.h>
using namespace std;
int main()
{
    string s;
    cin>>s;
    int n=s.size();
    int pos=ceil(log2((double)n+ceil(log2((double)n)+(double)1)));
    pos+=n;
    
    int tosend[pos+1];
    int i=0;
    int j=0;
    for(int k=1;k<=pos;k++)
    {
        if(k==pow(2,j))
        {
            j++;
            tosend[k]=0;
        }
        else{
            tosend[k]=s[i++]-'0';
        }
    }
    for(int i=0;i<j;i++)
    {
        int x=1<<i;
        for(int j=1;j<=pos;j++)
        {
            if(j!=x && j&x)
            {
                tosend[x]+=(tosend[j]==1)?1:0;
            }
        }
        tosend[x]=tosend[x]%2;
    }
  
    //creating error
    int x=rand()%pos+1;
    tosend[x]=1-tosend[x];
    cout<<"error bit created is"<<x<<endl;
    //error correction
    int errbit=0;
    for(int i=0;i<j;i++)
    {
        int x=1<<i;
        int cnt=0;
        for(int k=1;k<=pos;k++)
        {
            if(k&x)
            {
                cnt+=(tosend[k]==1)?1:0;
            }
        }
        cnt=cnt%2;
        if(cnt!=0)
            errbit+=x;
    }
    cout<<"error bit is: "<<errbit<<endl;
}