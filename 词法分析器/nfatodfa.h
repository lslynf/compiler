#ifndef NFATODFA_H
#define NFATODFA_H

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
using namespace std;
class NfaToDfa
{
   public:
     int start;//开始的状态
     int nfaend;//nfa中的终态，dfa的终态是含有nfa终态的状态
     int sum;//nfa中状态的集合
     int state_flag[200];
     int state_table[200][200];
     set<string>character;//转换字母表
     vector<int >temp;
     NfaToDfa(int begin,int cnt,int end)
     {
         start=begin;
         nfaend=end;
         sum=cnt;
         for(int i=0;i<sum;i++)
                for(int j=0;j<sum;j++)
                     state_table[i][j]=0;
        init();
     }
     void init()
     {
          memset(state_flag,0,sizeof(state_flag));
          this->temp.clear();
     }
     //如果Nfa中，开始状态后面的节点中没有空字符出现,那么Dfa开始状态即为Nfa开始状态
     bool  special(map<int,set<edge_node> >nfa)
     {
          set<edge_node>::iterator  it;
          for(it=nfa[start].begin();it!=nfa[start].end();it++)
          {
               string  str="#";
               if((*it).weight==str)
                return false;
          }
          return true;
     }
     //经过空字符后到达的状态
     void EpsilonClosure(int p,map<int,set<edge_node> >nfa)
     {
          state_flag[p]=1;
          state_flag[0]++;
          set<edge_node>::iterator  it;
          for(it=nfa[p].begin();it!=nfa[p].end();it++)
          {
               string  str=(*it).weight;
               int next=(*it).nextstate;
               if(str=="#"&&!state_flag[next])
               {
                   EpsilonClosure(next,nfa);
               }
          }
     }
     //经过一个字符再经过空字符后到达的状态
     void Closure(map<int,set<edge_node> >nfa)
     {
         for(int i=0;i<temp.size();i++)
         {
             EpsilonClosure(temp[i],nfa);
         }
     }
     int newstate()
     {
          if(state_flag[0]==0)
            return -2;
          for(int i=1;i<=state_table[0][0];i++)
          {
                if(state_flag[0]!=state_table[i][0])
                    continue;
                int j;
                for(j=1;j<=sum;j++)
                {
                    if(state_flag[j]!=state_table[i][j])
                        break;
                }
                if(j==(sum+1))//没有产生新状态
                    return i;
          }
          state_table[0][0]++;
          for(int i=0;i<=sum;i++)
            state_table[state_table[0][0]][i]=state_flag[i];
          return -1;
     }
     //对于state_flag数组中的每一个存在的状态经过该字符到达的状态
     void convert(string a,int position,map<int,set<edge_node> >nfa)
     {
          for(int i=1;i<=sum;i++)
          {
              if(state_table[position][i])
              {
                    set<edge_node>::iterator  it;
                    for(it=nfa[i].begin();it!=nfa[i].end();it++)
                    {
                         string str1=(*it).weight;
                         int next=(*it).nextstate;
                         if(str1==a)
                            temp.push_back(next);
                    }
              }
          }
     }
     public:
     int  DfaGraph(Graph &dfa,map<int,set<edge_node> >nfa,set<string>character,set<int>&dfastates,set<int>&terminates)
     {
            set<string>::iterator  it;
            int result;
            int num=1;
            int newid;
            //首先把开始状态放入状态表
            dfastates.insert(1);
            if(special(nfa))
            {
                state_table[0][0]++;
                state_table[1][0]++;
                state_table[1][start]=1;
                init();
            }
            else
            {
                EpsilonClosure(start,nfa);
                newstate();
                if(state_flag[nfaend]==1)
                    terminates.insert(1);
                init();
            }
            for(int i=1;i<=state_table[0][0];i++)
            {
                 for(it=character.begin();it!=character.end();it++)
                 {
                       convert(*it,i,nfa);
                       Closure(nfa);
                       result=newstate();
                       if(result==-1)//如果产生了新状态
                       {
                            num++;
                            newid=num;
                            dfa.add(i,newid,*it);
                            dfastates.insert(newid);
                            if(state_flag[nfaend]==1)
                                terminates.insert(newid);
                       }
                       else if(result>0&&temp.size()>0)
                       {
                           dfa.add(i,result,*it);
                       }
                    init();
                 }
            }
            return num;
     }
};
#endif
