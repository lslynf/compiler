#ifndef   GETDFA_H
#define  GETDFA_H

#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<map>
#include<set>
#include<vector>
#include<queue>
#include"GetFirstFollow.h"
#include<algorithm>
#include<iostream>
using namespace std;
class GetDfa:public GetFirstFollow
{
    public:
        map<int,set<edge> >dfagraph;
        queue<dfanode>state;
        vector<dfanode>dfa;
        char  next(int i)
        {
             production temp=items[i];
             string tright=temp.right;
             for(int i=0;i<tright.size()-1;i++)
                {
                    if(tright[i]=='.')
                       return tright[i+1];
                }
             return '@';//如果是规约项目
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
        //得到是该闭包项中哪些产生式.后所跟终结符为b
        vector<int> getp(vector<int>a,char b)
        {
             vector<int>ans;
             for(int i=0;i<a.size();i++)
             {
                   production temp=items[a[i]];
                   string tright=temp.right;
                   for(int j=0;j<tright.size()-1;j++)
                        if(tright[j]=='.'&&tright[j+1]==b)
                            ans.push_back(a[i]);
             }
             return ans;
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
        //返回经过一个字符转换后的产生式的位置
        int getnext(char a,char b,int c)
        {
            int i;
            for(i=0;i<items.size();i++)
            {
                 if(items[i].left==a)
                 {
                      string  temp=items[i].right;
                      for(int j=0;j<temp.size()-1;j++)
                      {
                          if(temp[j]==b&&temp[j+1]=='.'&&temp.size()==c)
                              return  i;
                      }
                 }
            }
        }
        void getgraph()
        {
            closure a=csets[0];
            dfanode b(num);
            vector<int>temp=a.psets;
            //去掉重复元素
            sort(temp.begin(),temp.end());
            temp.erase(unique(temp.begin(), temp.end()), temp.end());
            for(int i=0;i<temp.size();i++)
            {
                b.add(temp[i]);
            }
            dfa.push_back(b);
            state.push(b);
            while(!state.empty())
            {
                 dfanode t=state.front();
                 state.pop();
                 vector<int >q=t.choices;
                 vector<char>change;//转换的字符记录下来
                 for(int i=0;i<q.size();i++)
                 {
                       char ch=next(q[i]);
                       change.push_back(ch);
                 }
                 //删减重复的元素
                 vector<char>::iterator iter=change.begin();
                 while(iter != change.end())
                 {
                        if(iter != find(change.begin(),iter,*iter))
                        {
                            iter = change.erase(iter);
                            continue;
                        }
                        iter++;
                 }
                 vector<char>::iterator it;
                 for(it=change.begin();it!=change.end();it++)
                 {
                        if(*it!='@')
                        {
                            vector<int >ans=getp(q,*it);//返回转换字符为*it的产生式
                            cout<<*it<<" ";
                            for(int m=0;m<ans.size();m++)
                                cout<<ans[m]<<" ";
                            cout<<endl;
                            dfanode next;
                            for(int j=0;j<ans.size();j++)
                            {
                                production one=items[ans[j]];
                                char oleft=one.left;
                                int  rsize=one.right.size();
                                int clocal=getnext(oleft,*it,rsize);
                                vector<int> ctemp=csets[clocal].psets;
                                sort(ctemp.begin(),ctemp.end());
                                ctemp.erase(unique(ctemp.begin(),ctemp.end()), ctemp.end());
                                for(int k=0;k<ctemp.size();k++)
                                    next.add(ctemp[k]);
                            }
                            if(!hasExist(next))
                           {
                               num++;
                               next.id=num;
                               edge enode(num,*it);
                               dfagraph[t.id].insert(enode);
                               state.push(next);
                               dfa.push_back(next);
                           }
                           //如果该节点存在
                           else
                           {
                                edge enode(getdfa(next),*it);
                                dfagraph[t.id].insert(enode);
                           }
                        }
                 }
                 if(change.size()==1)
                 {
                       dfagraph[t.id].clear();
                 }
            }
        }
};
#endif // GETDFA_H
