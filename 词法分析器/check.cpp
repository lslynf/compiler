#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<string.h>
#include<set>
#include<vector>
#include<map>
#include<stack>
#include"regtonfa.h"
#include"nfatodfa.h"
#include"mindfa.h"
using namespace std;
bool isletter(char ch)
{
    if(ch>='A'&&ch<='Z'||ch>='a'&&ch<='z')
         return true;
    else
        return false;
}
bool isdigits(char ch)
{
    int a=ch-'0';
    if(a>=0&&a<=9)
        return true;
    else
        return false;
}
void check(Graph mindfa,string str)
{
    int i;
    int position=1;
    for(i=0;i<str.length();i++)
    {
        char ch=str[i];
        set<edge_node>::iterator it;
        set<edge_node>temp;
        temp=mindfa.ResultGraph[position];
        if(isletter(ch))
        {
            int cnt=0;
            for(it=temp.begin();it!=temp.end();it++)
            {
                if((*it).weight=="letter")
                    position=(*it).nextstate;
                else
                    cnt++;
            }
            if(cnt==temp.size())
            {
                cout<<str<<"是不合法的标识符"<<endl;
                break;
            }
        }
        if(isdigits(ch))
        {
             int cnt=0;
             for(it=temp.begin();it!=temp.end();it++)
             {
                 if((*it).weight=="digit")
                    position=(*it).nextstate;
                else
                    cnt++;
             }
             if(cnt==temp.size())
             {
                 cout<<str<<"是不合法的标识符"<<endl;
                 break;
             }
        }
        if(!isdigits(ch)&&!isletter(ch))
        {
              cout<<str<<"是不合法的标识符"<<endl;
              break;
        }
    }
    if(i==str.length())
        cout<<str<<"是一个合法的标识符"<<endl;
}
int main()
{
    RegToNfa  a("(@a@@b@)*(@a@*|@b@*)(@b@@a@)*");
    Graph nfa;
    int num=0;
    set<string>character;
    nfa_node ans=a.ScanReg(nfa,num,character);
    set<edge_node>::iterator it;
    cout<<"所得到的NFA图是:"<<endl;
        for(int i=1;i<=num;i++)
        {
            nfa.show(i);
        }
    cout<<endl;
    Graph dfa;
    set<int >dfastates;
    set<int >terminates;
    NfaToDfa  b(ans.start,num,ans.end);
    int sum=b.DfaGraph(dfa,nfa.ResultGraph,character,dfastates,terminates);
    cout<<"所得到的DFA图是:"<<endl;
    for(int i=1;i<=sum;i++)
    {
        dfa.show(i);
    }
    cout<<endl;
    Graph MinDfaGraph;
    int blocknum=1;
    MinDfa  test;
    test.GetDivide(dfa,dfastates,terminates,blocknum);
    test.GetMinDfa(dfa,dfastates,terminates,MinDfaGraph);
     cout<<"所得到的最小化DFA图是:"<<endl;
    for(int i=1;i<=blocknum;i++)
       {
             MinDfaGraph.show(i);
       }
    cout<<endl;
    string input;
    cin>>input;
    //check(MinDfaGraph,input);
}
