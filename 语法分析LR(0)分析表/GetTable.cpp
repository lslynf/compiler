#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<map>
#include<set>
#include<stack>
#include<vector>
#include<queue>
#include"GetDFA.h"
#include<algorithm>
#include<iostream>
using namespace std;
//建立分析表，有两张分析表，一张是列为终结符，一张是列为非终结符去掉开始符号
struct node
{
    int nextstate;
    char weight;
    int flag;//设置标志，若为1则表示移入，若为2则表示规约,若为3则表示接受
    node(){}
    node(int a,char b,int c)
    {
        nextstate=a;
        weight=b;
        flag=c;
    }
    node(int a,char b)
    {
        nextstate=a;
        weight=b;
        flag=0;//表示是非终结符
    }
};
class table:public getDFA
{
    public:
         map<int,vector<node> >action;
         map<int,vector<node> >go;
         stack<char>expstack;
         stack<char>analystack;
        void  DFAresult()
        {
            init();
            getItems();
            getclosure();
            getresult();
        }
         void BuildTable()
         {
              int i;
              set<edge>::iterator it1;
              for(i=0;i<=num;i++)//对于DFA中的每一个状态
              {
                  //不是规约项目
                  set<edge>temp=dfagraph[i];
                  if(temp.size()>0)
                  {
                       for(it1=temp.begin();it1!=temp.end();it1++)
                          {
                                  //如果通过终结符转到下一个状态
                                      if(IsTerminal((*it1).weight))
                                      {
                                            node a((*it1).nextid,(*it1).weight,1);
                                            action[i].push_back(a);
                                      }
                                      else
                                      {
                                            node a((*it1).nextid,(*it1).weight);
                                            go[i].push_back(a);
                                      }
                          }
                  }
                  else//对于归约项，还得找到是否是含有开始符号的归约项目
                  {
                        int b=dfa[i].choices[0];
                        int c=items[b].before;
                        if(items[b].end==1)
                        {
                            node a(-1,'$',3);
                            action[i].push_back(a);
                        }
                        else
                        {
                            set<char>::iterator it2;
                            for(it2=terminal.begin();it2!=terminal.end();it2++)
                           {
                              node a(c,*it2,2);
                              action[i].push_back(a);
                           }
                        }
                  }
            }
         }
        void analy()
        {
            string token;
            cout<<"请输入表达式:"<<endl;
            cin>>token;
            expstack.push('$');
            analystack.push('$');
            analystack.push(0);
            for(int i=token.size()-1;i>=0;i--)
                expstack.push(token[i]);
            int atop=analystack.top();
            char etop=expstack.top();//总是非终结符
            cout<<"------------分析过程如下---------------"<<endl;
            while(!expstack.empty()&&!analystack.empty())
            {
                  int jump=0;
                  vector<node>temp=action[atop];
                  int exist=0;
                  for(int i=0;i<temp.size();i++)
                  {
                       if(temp[i].weight==etop)
                       {
                            exist=1;
                            //如果是shift，产生式栈出一个元素压入到分析栈，同时压入后继状态
                            if(temp[i].flag==1)
                            {
                                  cout<<"shift"<<endl;
                                  analystack.push(etop);
                                  analystack.push(temp[i].nextstate);
                                  expstack.pop();
                                  atop=analystack.top();
                                  etop=expstack.top();
                            }
                            //如果是归约，两倍产生式右部长度出栈，同时压回左部及当前栈顶遇到左部产生的后继状态
                            else if(temp[i].flag==2)
                            {
                                   cout<<"reduce"<<" ";
                                   int  a=temp[i].nextstate;
                                   cout<<input[a].left<<"->";
                                   cout<<input[a].right;
                                   cout<<endl;
                                   int len=input[a].right.size();
                                   char tleft=input[a].left;
                                   for(int i=0;i<len*2;i++)
                                   {
                                       analystack.pop();
                                   }
                                   atop=analystack.top();
                                   analystack.push(tleft);
                                   int b;
                                   vector<node>gotemp=go[atop];
                                   for(int k=0;k<gotemp.size();k++)
                                   {
                                       if(gotemp[k].weight==tleft)
                                                b=gotemp[k].nextstate;
                                   }
                                   analystack.push(b);
                                   atop=analystack.top();
                            }
                            else if(temp[i].flag==3)
                            {
                                 cout<<"accept!"<<endl;
                                 cout<<"此token序列符合文法！"<<endl;
                                 jump=1;
                                 break;
                            }
                       }
                  }
                  if(exist==0)
                        {
                              jump=1;
                              cout<<"语法错误"<<endl;
                              break;
                        }
                  if(jump==1)
                        break;
            }
        }
        void show()
        {
                /*for(int i=0;i<input.size();i++)
                 {
                    cout<<input[i].left<<"->";
                    cout<<input[i].right;
                    cout<<endl;
                 }*/
                set<char>::iterator p1,p2;
                cout<<"非终结符是:"<<endl;
                for(p1=nonterminal.begin();p1!=nonterminal.end();p1++)
                    cout<<*p1<<" ";
                cout<<endl;
                cout<<"终结符是:"<<endl;
                for(p2=terminal.begin();p2!=terminal.end();p2++)
                    cout<<*p2<<" ";
                cout<<endl;
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
               // cout<<"开始符号"<<start<<" 个数"<<num<<endl;
                cout<<"各项目的闭包集为:" <<endl;
                for(int i=0;i<csets.size();i++)
                {
                     cout<<i<<"  ";
                     vector<production>temp=csets[i].psets;
                     for(int j=0;j<temp.size();j++)
                     {
                         cout<<temp[j].id<<" ";
                     }
                     cout<<endl;
                }
                set<edge>::iterator it;
                cout<<"-----------得到的DFA图是-------------"<<endl;
                for(int i=0;i<dfagraph.size();i++)
                {
                    cout<<i<<" ";
                    //cout<<dfagraph[i].size()<<" ";
                    for(it=dfagraph[i].begin();it!=dfagraph[i].end();it++)
                        cout<<"经过"<<(*it).weight<<"到达"<<(*it).nextid<<"   ";
                    cout<<endl;
                }
                /*cout<<"DFA所对应的产生式为:"<<endl;
                for(int i=0;i<dfa.size();i++)
                  {
                      cout<<dfa[i].id<<"产生式: ";
                      for(int j=0;j<dfa[i].choices.size();j++)
                        cout<<dfa[i].choices[j]<<" ";
                      cout<<endl;
                  }*/
          }
          void display()
         {
             BuildTable();
             //cout<<"1"<<endl;
             //cout<<num<<endl;
             cout<<"-----------action表如下---------"<<endl;
             for(int i=0;i<action.size();i++)
             {
                 cout<<i<<"  ";
                 for(int j=0;j<action[i].size();j++)
                    cout<<action[i][j].weight<<action[i][j].nextstate<<action[i][j].flag<<" ";
                cout<<endl;
             }
             cout<<"-----------go表如下----------"<<endl;
             for(int i=0;i<go.size();i++)
             {
                 cout<<i<<" ";
                 for(int j=0;j<go[i].size();j++ )
                    cout<<go[i][j].weight<<go[i][j].nextstate<<" ";
                 cout<<endl;
             }
         }
};
int main()
{
    table A;
    A.DFAresult();
    A.show();
    A.display();
    A.analy();
}
