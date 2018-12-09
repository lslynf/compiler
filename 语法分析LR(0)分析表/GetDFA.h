#ifndef  GETDFA_H
#define GETDFA_H

#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<map>
#include<set>
#include<vector>
#include<queue>
#include<algorithm>
#include<iostream>
using namespace std;
class production
{
    public:
        char left;
        string right;
        int  id;//产生式在图中的位置，方便构造DFA,项目的编号
        int  before;//记录项目对应的产生式
        int  end;
        production(){}//无参构造函数，不然map会出错
        production(char a,string b)
        {
            left=a;
            right=b;
            id=0;
        }
        production(char a,string b,int c)
        {
            left=a;
            right=b;
            id=c;
        }
       production(char a,string b,int c,int d)
        {
            left=a;
            right=b;
            id=c;
            before=d;
            end=0;
        }
        production(char a,string b,int c,int d,int e)
        {
            left=a;
            right=b;
            id=c;
            before=d;
            end=e;
        }
};
struct dfanode
{
    int id;//存放状态号
    vector<int>choices;//存放项目的编号
    dfanode(){}
    dfanode(int a)
    {
        id=a;
    }
    void add(int b)
    {
        choices.push_back(b);
    }
};
struct edge
{
    int nextid;
    char  weight;
    edge(int a,char b)
    {
         nextid=a;
         weight=b;
    }
};
//得到一个项目的闭包
class closure
{
   public:
        vector<production> psets;
        closure(){};
        //闭包项中包括自己
        closure(production a)
        {
            psets.push_back(a);
        }
        void add(production b)
        {
            psets.push_back(b);
        }
};
bool operator<(edge a,edge b)
{
    //return a.nextstate<b.nextstate;
    if(a.nextid!=b.nextid)
         return a.nextid<b.nextid;
    else
        return a.weight<b.weight;

}
class getDFA
{
    public:
        map<int,production> input;//得到输入的产生式
        map<int,production> items;//得到所有的项目
        map<int,set<edge> >dfagraph;
        set<char>nonterminal;
        set<char>terminal;
        set<char>all;
        queue<dfanode>state;
        int n;
        char ch='.';
        char start;//开始符号
        int num=0;//状态个数
        vector<dfanode>dfa;
        vector<closure>csets;
    void init()
    {
        cout<<"请输入产生式的个数:"<<endl;
        cin>>n;
        cin.ignore();
        for(int k=0;k<n;k++)
        {
            string temp;
            getline(cin,temp);
            char buff=temp[0];
            nonterminal.insert(buff);//非终结符
            string b=temp.substr(3);
            production  anew(buff,b);
            input[k]=anew;
            for(int i=0;i<b.size();i++)
                all.insert(b[i]);
        }
        set_difference(all.begin(),all.end(),nonterminal.begin(),nonterminal.end(),inserter(terminal,terminal.begin()));
        terminal.insert('$');
        start=input[0].left;
    }
    //得到所有的项目
    void getItems()
    {
         int cnt=0;
         start=input[0].left;
         for(int i=0;i<input.size();i++)
         {
              char s=input[i].left;
              for(int j=0;j<=input[i].right.size();j++)
              {
                  string q=input[i].right;
                  q.insert(q.begin()+j,ch);
                  if(i==0&&j==input[i].right.size())
                  {
                      production p(s,q,cnt,i,1);
                      items[cnt]=p;
                      cnt++;
                  }
                  else
                  {
                        production p(s,q,cnt,i);
                        items[cnt]=p;
                        cnt++;
                  }
              }
         }
    }
    //判断是否是终结符
    bool IsTerminal(char a)
    {
        set<char>::iterator p2;
        for(p2=terminal.begin();p2!=terminal.end();p2++)
        {
            if(*p2==a)
                return true;
        }
        return false;
    }
    vector<int> getindex(char a)
    {
        vector<int> b;
        int i;
        for(i=0;i<items.size();i++)
        {
             if(items[i].left==a)
                 b.push_back(i);
        }
        return b;
    }
    //得到所有项目的闭包集
    //还存在一些问题
    void getclosure()
    {
        for(int i=0;i<items.size();i++)
        {
            closure temp(items[i]);
            string str=items[i].right;
            for(int j=0;j<str.size()-1;j++)
            {
                if(str[j]=='.'&&!IsTerminal(str[j+1]))
                {
                     //得到所有产生式左部为该非终结符的产生式
                     vector<int >ans=getindex(str[j+1]);
                     //进行排除，得到开始项目加入闭包
                     //cout<<"test";
                     /*cout<<"右部非终结符:"<<str[j+1]<<endl;
                     for(int p=0;p<ans.size();p++)
                        cout<<ans[p]<<" ";*/
                     for(int k=0;k<ans.size();k++)
                     {
                            production  one=items[ans[k]];
                            string s=one.right;
                            if(s[0]=='.'&&one.left==str[j+1])
                            {
                                 temp.add(one);
                            }
                     }
                }
            }
            csets.push_back(temp);
        }
    }
    //返回项目进度的位置
    int  getrate(production a)
    {
        string str=a.right;
        for(int i=0;i<str.size();i++)
        {
            if(str[i]=='.')
                return i;
        }
        return -1;
    }
    //返回经过一个字符转换后的产生式的位置
    int getnext(char a,char b)
    {
        int i;
        for(i=0;i<items.size();i++)
        {
             if(items[i].left==a)
             {
                  string  temp=items[i].right;
                  for(int j=0;j<temp.size()-1;j++)
                  {
                      if(temp[j]==b&&temp[j+1]=='.')
                          return  i;
                  }
             }
        }
    }
    //判断该结点是否存在
    bool hasExist(dfanode a)
    {
         vector<int >com=a.choices;
         for(int i=0;i<dfa.size();i++)
         {
               if(dfa[i].choices==com)
                    return true;
         }
         return false;
    }
    int getdfa(dfanode a)
    {
        vector<int>com=a.choices;
        for(int i=0;i<dfa.size();i++)
         {
               if(dfa[i].choices==com)//两个vector可以直接比较
                    return i;
         }
    }
    //利用初始项目得到DFA
    void getresult()
    {
         closure a=csets[0];
         dfanode b(num);
         vector<production>temp=a.psets;
         for(int i=0;i<temp.size();i++)
         {
             b.add(temp[i].id);
         }
         dfa.push_back(b);
         state.push(b);
         //当队列非空时
         while(!state.empty())
         {
              dfanode t=state.front();
              state.pop();
              vector<int >q=t.choices;
              if(q.size()>1)
              {
                    for(int i=0;i<q.size();i++)
                      {
                           production one=items[q[i]];
                           int  it=getrate(one);
                           char oleft=one.left;
                           char ch=one.right[it+1];
                           dfanode next;
                           int clocal=getnext(oleft,ch);
                           vector<production> ctemp=csets[clocal].psets;
                           for(int k=0;k<ctemp.size();k++)
                           {
                                next.add(ctemp[k].id);
                           }
                           //如果该节点不存在，为新的节点
                           if(!hasExist(next))
                           {
                               num++;
                               next.id=num;
                               edge enode(num,ch);
                               dfagraph[t.id].insert(enode);
                               state.push(next);
                               dfa.push_back(next);
                           }
                           //如果该节点存在
                           else
                           {
                                edge enode(getdfa(next),ch);
                                dfagraph[t.id].insert(enode);
                           }
                      }
              }
              else  if(q.size()==1)
              {
                   int len;
                   production  one=items[q[0]];
                   len=one.right.size();
                   //如果项目走到最后
                   int it=getrate(one);
                   if(it==len-1)
                   {
                        dfagraph[t.id].clear();
                   }
                   else
                   {
                           char oleft=one.left;
                           char ch=one.right[it+1];
                           dfanode next;
                           int clocal=getnext(oleft,ch);
                           vector<production> ctemp=csets[clocal].psets;
                           for(int k=0;k<ctemp.size();k++)
                           {
                                next.add(ctemp[k].id);
                           }
                           //如果该节点不存在，为新的节点
                           if(!hasExist(next))
                           {
                               num++;
                               next.id=num;
                               edge enode(num,ch);
                               dfagraph[t.id].insert(enode);
                               state.push(next);
                               dfa.push_back(next);
                           }
                           //如果该节点存在
                           else
                           {
                                edge enode(getdfa(next),ch);
                                dfagraph[t.id].insert(enode);
                           }
                   }
              }
         }
    }
};
#endif
