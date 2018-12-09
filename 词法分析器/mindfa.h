#ifndef  MINDFA_H
#define  MINDFA_H

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
using namespace std;
//1.第一次划分终态和非终态
//2.每次有新状态再划分
//3.记录每个状态所在的区
//4.记录每个区中有哪些状态
class MinDfa
{
    public:
    //判断两个边集经过状态后是否还在同一个集合中
    bool InSameSet(set<edge_node> s1,set<edge_node> s2,map<int ,int >stateinblock)
    {
         //把边集中下个边的状态用区号来代替
        set<edge_node> temp1,temp2;
        set<edge_node>::iterator t1,t2;
        for(t1=s1.begin();t1!=s1.end();t1++)
        {
            edge_node  temp((*t1).weight,stateinblock[(*t1).nextstate]);
            temp1.insert(temp);
        }
        for(t2=s2.begin();t2!=s2.end();t2++)
        {
            edge_node  temp((*t2).weight,stateinblock[(*t2).nextstate]);
            temp2.insert(temp);
        }
        set<edge_node>::iterator p1=temp1.begin(),p2=temp2.begin();
        while(p1!=temp1.end()&&p2!=temp2.end())
        {
            if(p1->weight==p2->weight&&p1->nextstate==p2->nextstate)
            {
                p1++;
                p2++;
            }
            else
                return false;
        }
        if(p1==temp1.end()&&p2==temp2.end())
            return true;
        else
            return false;
    }
    //状态所在的区号
    map<int,int >stateinblock(map<int ,set<int > >stateGraph,int blocknum)
    {
          map<int,int>stateinblockgraph;
          for(int i=1;i<=blocknum;i++)
          {
               for(set<int >::iterator it=stateGraph[i].begin();it!=stateGraph[i].end();it++)
               {
                    stateinblockgraph[*it]=i;
               }
          }
          return stateinblockgraph;
    }
    //求已有的两个状态集合中不同的元素的集合
    set<int >difference(set<int>set1,set<int>set2)
    {
         set<int>differset;
         set<int >::iterator it;
         for(it=set1.begin();it!=set1.end();it++)
         {
             if(!set2.count(*it))
                differset.insert(*it);
         }
         return differset;
    }
    //对一个区中的状态进行判断是否在同一个区
    set<int >newdivide(Graph dfa,set<int >oneblock,map<int ,int >stateinblock)
    {
         set<int >newstate;
         newstate.insert(*oneblock.begin());
         set<int >::iterator it=oneblock.begin();
         it++;
         while(it!=oneblock.end())
         {
              if(InSameSet(dfa.ResultGraph[*oneblock.begin()],dfa.ResultGraph[*it],stateinblock))
              {
                  newstate.insert(*it);
              }
              it++;
         }
         return newstate;
    }
    map<int,set<int > >FirstDivide(Graph dfa,set<int >dfastates,set<int >terminates,int &blocknum)
    {
        map<int,set<int > >stateGraph;
        //如果都是终止状态,只有一个分区
        set<int >::iterator it;
        if(dfastates.size()==terminates.size())
        {
             blocknum=1;
             for(it=dfastates.begin();it!=dfastates.end();it++)
                stateGraph[blocknum].insert(*it);
        }
        else
        {
             blocknum=2;
             for(it=dfastates.begin();it!=dfastates.end();it++)
               {
                 if(terminates.count(*it))
                    stateGraph[2].insert(*it);
                 else
                    stateGraph[1].insert(*it);
               }
            /*for(it=stateGraph[1].begin();it!=stateGraph[1].end();it++)
                cout<<*it<<" ";
            cout<<endl;
            for(it=stateGraph[2].begin();it!=stateGraph[2].end();it++)
                cout<<*it<<" ";
            cout<<endl;*/
        }
        return stateGraph;
    }
    map<int,set<int > >OtherDivide(Graph dfa,map<int,set<int> >stateGraph,int &blocknum)
    {
          map<int,set<int > >dividegraph;
          map<int,int >stateinblockgraph=stateinblock(stateGraph,blocknum);
         /* cout<<"所对应的状态号是"<<endl;
          for(int i=1;i<=stateinblockgraph.size();i++)
             cout<<i<<""<<stateinblockgraph[i]<<endl;*/
          int num=0;
          int i;
          for(i=1;i<=blocknum;i++)
          {
              set<int >temp=stateGraph[i];
              set<int >result=newdivide(dfa,temp,stateinblockgraph);
              if(result.size()!=temp.size())
              {
                    set<int >newblock=difference(temp,result);
                    num++;
                    dividegraph[num]=result;
                    num++;
                    dividegraph[num]=newblock;
                    break;
              }
              else
              {
                    num++;
                    dividegraph[num]=temp;
              }
          }
          while(i<blocknum)
          {
              num++;
              i++;
              dividegraph[num]=stateGraph[i];
          }
          blocknum=num;
          return dividegraph;
    }
    map<int ,set<int> >GetDivide(Graph dfa,set<int >dfastates,set<int >terminates,int&blocknum)
    {
        map<int,set<int> >initgraph;
        map<int ,set<int> >blockgraph;
        int cnt=1;
        initgraph=FirstDivide(dfa,dfastates,terminates,blocknum);
        blockgraph=initgraph;
        while(blocknum!=cnt)
        {
            cnt=blocknum;
            map<int,set<int> >temp=OtherDivide(dfa,blockgraph,blocknum);
            blockgraph=temp;
        }
       /*cout<<"一共划分了"<<blocknum<<endl;
        for(int i=1;i<=blocknum;i++)
        {
            cout<<i<<"  ";
            set<int >::iterator it;
            for(it=blockgraph[i].begin();it!=blockgraph[i].end();it++)
                cout<<*it<<" ";
            cout<<endl;
        }*/
        return blockgraph;
    }
    //把得到的区号划分状态图转化成最小化图
    void  GetMinDfa(Graph dfa,set<int>dfastates,set<int>terminates,Graph &MinDfaGraph)
    {
        int blocknum=1;
        map<int,set<int > >blockgraph=GetDivide(dfa,dfastates,terminates,blocknum);
        map<int,int>stateinblockgraph=stateinblock(blockgraph,blocknum);
        /*cout<<"所对应的状态号是"<<endl;
          for(int i=1;i<=stateinblockgraph.size();i++)
             cout<<i<<""<<stateinblockgraph[i]<<endl;*/
        set<edge_node>::iterator it;
        //插入时出现了一个问题，当两个状态号一样时，默认覆盖了字符，对于重载函数要进行修改
        for(int i=1;i<=blocknum;i++)
        {
             int x=*blockgraph[i].begin();
             set<edge_node>temp=dfa.ResultGraph[x];
             for(it=temp.begin();it!=temp.end();it++)
             {
                  int next=stateinblockgraph[(*it).nextstate];
                  MinDfaGraph.add(i,next,(*it).weight);
             }
        }
    }
};
#endif
