#ifndef   GETFIRSTFOLLOW_H
#define   GETFIRSTFOLLOW_H

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
        int  id;//产生式在图中的位置，方便构造DFA，求项目时项目的编号
        int  before;//记录项目对应的产生式
        int  end;//标记项目的类型，1和2表示规约项目，同时2表示含有开始符号的规约项目
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
        vector<int> psets;
        closure(){};
        //闭包项中包括自己
        closure(production a)
        {
            psets.push_back(a.id);
        }
        void add(production b)
        {
            psets.push_back(b.id);
        }
};
bool operator<(edge a,edge b)
{
    if(a.nextid!=b.nextid)
         return a.nextid<b.nextid;
    else
        return a.weight<b.weight;
}
class GetFirstFollow
{
    public:
        map<int,production> input;//得到输入的产生式
        map<int,production> items;//得到所有的项目
        vector<char>index;
        map<char,bool>visit;
        map<char,set<char> >first;//first集
        map<char,set<char> >follow;//follow集
        set<char>nonterminal;
        set<char>terminal;
        set<char>all;
        int n;
        char ch='.';
        char start;//开始符号
        int num=0;//状态个数
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
                index.push_back(anew.left);
                for(int i=0;i<b.size();i++)
                    all.insert(b[i]);
            }
            set_difference(all.begin(),all.end(),nonterminal.begin(),nonterminal.end(),inserter(terminal,terminal.begin()));
            terminal.insert('$');
            start=input[0].left;
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
         vector<int> getindex1(char a)
        {
             vector<int>b;
             for(int i=0;i<n;i++)
             {
                 if(index[i]==a)
                     b.push_back(i);
             }
             return b;
        }
         void getFirst(int position)
        {
            int flag=0;
            int cnt=0;
            char tleft=input[position].left;
            visit[tleft]=true;
            string tright=input[position].right;
            if(IsTerminal(tright[0]))
            {
                first[tleft].insert(tright[0]);
            }
            else
            {
                for(int j=0;j<tright.size();j++)
                {
                    if(IsTerminal(tright[j]))
                   {
                         first[tleft].insert(tright[j]);
                         break;
                    }
                    //遇到了非终结符，递归查找first集
                    if(!visit[tright[j]])
                    {
                        vector<int >temp=getindex1(tright[j]);
                        for(int i=0;i<temp.size();i++)
                            getFirst(temp[i]);
                    }
                        set<char>::iterator it;
                        //对遇到的非终结符的first集进行处理
                        for(it=first[tright[j]].begin();it!=first[tright[j]].end();it++)
                        {
                            if(*it=='#')
                                flag=1;
                            else
                                first[tleft].insert(*it);
                        }
                        if(flag==0)
                        {
                            break;
                        }
                        else
                        {
                            cnt+=flag;
                            flag=0;
                        }
                    }
                        if(cnt==tright.size())
                            first[tleft].insert('#');
                }
            }
         void  FirstResult()
            {
                visit.clear();
                for(int i=0;i<n;i++)
                    getFirst(i);
            }
            void getFollow(char character)
            {
                 for(int i=0;i<n;i++)
                {
                     int position=-1;
                     char  tleft=input[i].left;
                     string tright=input[i].right;
                     for(int j=0;j<tright.size();j++)
                      {
                            if(tright[j]==character)
                            {
                                position=j;
                                break;
                            }
                     }
                                //如果后面还有符号
                      if(position!=-1&&position<tright.size()-1)
                       {
                             char next=tright[position+1];
                             if(IsTerminal(next))
                              {
                                   follow[character].insert(next);
                              }
                                //不是终结符，则加入其first集
                             else
                                {
                                    int flag=0;
                                    set<char>::iterator  it;
                                    for(it=first[next].begin();it!=first[next].end();it++)
                                    {
                                        if(*it=='#')
                                            flag=1;
                                        else
                                            follow[character].insert(*it);
                                    }
                                    if(flag==1&&tleft!=character)
                                    {
                                            getFollow(tleft);
                                            set<char>::iterator it;
                                            for(it=follow[tleft].begin();it!=follow[tleft].end();it++)
                                                follow[character].insert(*it);
                                    }
                                }
                            }
                            else if(position!=-1&&position==tright.size()-1&&tleft!=character)
                            {
                                getFollow(tleft);
                                set<char>::iterator it;
                                for(it=follow[tleft].begin();it!=follow[tleft].end();it++)
                                    follow[character].insert(*it);
                            }
                      }
            }
            void FollowResult()
            {
                char start=input[0].left;
                follow[start].insert('$');
                set<char>::iterator it;
                for(it=nonterminal.begin();it!=nonterminal.end();it++)
                    getFollow(*it);
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
                      if(j==input[i].right.size())
                      {
                          if(s==start)
                          {
                             production p(s,q,cnt,i,2);//含有开始符号的规约项
                             items[cnt]=p;
                             cnt++;
                          }
                          else
                          {
                              production p(s,q,cnt,i,1);//规约项
                              items[cnt]=p;
                              cnt++;
                          }
                      }
                      else
                      {
                            if(q[0]=='.')
                            {
                                production p(s,q,cnt,i,-1);
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
        }
        //返回左部为a的产生式
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
       void dfs(char a,vector<int>&result)
        {
               vector<int>ans=getindex(a);
               int cnt=0;
               for(int i=0;i<ans.size();i++)
               {
                   production temp=items[ans[i]];
                   if(temp.end==-1)//刚要开始的项目
                        cnt++;
               }
               if(cnt==0)
                    return;
               for(int k=0;k<ans.size();k++)
                {
                    production  one=items[ans[k]];
                    string s=one.right;
                    if(s[0]=='.'&&one.left==a)
                    {
                            result.push_back(one.id);
                            if(!IsTerminal(one.right[1])&&one.right[1]!=one.left)
                                dfs(one.right[1],result);
                    }
                }
        }
        //注意一个问题，会有重复，就是会把自己再放入一次闭包项
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
                             dfs(str[j+1],temp.psets);
                        }
                  }
                 csets.push_back(temp);
             }
        }
        void show()
        {
                init();
                FirstResult();
                FollowResult();
                getItems();
                getclosure();
                cout<<"--------------项目为---------------"<<endl;
                for(int i=0;i<items.size();i++)
                {
                    cout<<items[i].left<<"->";
                    cout<<items[i].right<<" ";
                    cout<<items[i].id<<" ";
                    cout<<items[i].before<<" ";
                    cout<<items[i].end;
                    cout<<endl;
                }
                cout<<"各项目的闭包集为:" <<endl;
                for(int i=0;i<csets.size();i++)
                {
                     cout<<i<<"  ";
                     vector<int>temp=csets[i].psets;
                     for(int j=0;j<temp.size();j++)
                     {
                         cout<<temp[j]<<" ";
                     }
                     cout<<endl;
                }
            cout<<"************************"<<endl;
            cout<<"first集是:"<<endl;
            set<char>::iterator it,p;
            for(it=nonterminal.begin();it!=nonterminal.end();it++)
            {
                cout<<*it<<": ";
                set<char>temp=first[*it];
                for(p=temp.begin();p!=temp.end();p++)
                    cout<<*p<<" ";
                cout<<endl;
            }
            cout<<"************************"<<endl;
            cout<<"follow集是:"<<endl;
            set<char>::iterator it1,p3;
            for(it1=nonterminal.begin();it1!=nonterminal.end();it1++)
            {
                cout<<*it1<<": ";
                set<char>temp=follow[*it1];
                for(p3=temp.begin();p3!=temp.end();p3++)
                    cout<<*p3<<" ";
                cout<<endl;
            }
        }
};
#endif
