#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<map>
#include<set>
#include<stack>
#include<vector>
#include<queue>
#include"GetDfa.h"
#include<algorithm>
#include<iostream>
using namespace std;
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
        flag=0;
    }
};
class table:public GetDfa
{
    public:
         map<int,vector<node> >action;
         map<int,vector<node> >go;
         stack<char>expstack;
         stack<char>analystack;
         //返回规约项的编号
         int isreduce(vector<int>a)
         {
                for(int i=0;i<a.size();i++)
                {
                    if(items[a[i]].end==1||items[a[i]].end==2)
                        return a[i];
                }
                return -2;
         }
         //要进行判断是否含有规约项
         void BuildTable()
         {
             set<edge>::iterator it1;
             for(int i=0;i<=num;i++)
             {
                   vector<int>dtemp=dfa[i].choices;
                   int  position=isreduce(dtemp);
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
                         if(position>0)//如果含有规约项,在follow集下写规约
                         {
                              if(items[position].end==2)//如果是包含开始符号
                              {
                                    node a(-1,'$',3);
                                    action[i].push_back(a);
                              }
                              else
                              {
                                    char ch=items[position].left;
                                    int c=items[position].before;
                                    set<char>cf=follow[ch];
                                    set<char>::iterator it;
                                    for(it=cf.begin();it!=cf.end();it++)
                                    {
                                        node a(c,*it,2);
                                        action[i].push_back(a);
                                    }
                              }
                        }
                   }
                   else//对于规约项且不含有开始项目,但也是只在follow集下才进行规约
                   {
                        int b=dfa[i].choices[0];
                        int c=items[b].before;
                        char ch=items[b].left;
                        set<char>temp=follow[ch];
                        set<char>::iterator it2;
                        for(it2=temp.begin();it2!=temp.end();it2++)
                        {
                            node a(c,*it2,2);
                            action[i].push_back(a);
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
            char etop=expstack.top();//总是终结符
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
            init();
            FirstResult();
            FollowResult();
            getItems();
            getclosure();
            getgraph();
            BuildTable();
            cout<<"--------------项目为---------------"<<endl;
            for(int i=0;i<items.size();i++)
            {
                cout<<items[i].left<<"->";
                cout<<items[i].right<<" ";
                cout<<items[i].id<<" ";
               // cout<<items[i].before<<" ";
               // cout<<items[i].end;
                cout<<endl;
            }
           /* cout<<"各项目的闭包集为:" <<endl;
            for(int i=0;i<csets.size();i++)
            {
                cout<<i<<"  ";
                vector<int>temp=csets[i].psets;
                for(int j=0;j<temp.size();j++)
                {
                        cout<<temp[j]<<" ";
                }
                    cout<<endl;
            }*/
            cout<<"------------first集是---------------"<<endl;
            set<char>::iterator it,p;
            for(it=nonterminal.begin();it!=nonterminal.end();it++)
            {
                cout<<*it<<": ";
                set<char>temp=first[*it];
                for(p=temp.begin();p!=temp.end();p++)
                    cout<<*p<<" ";
                cout<<endl;
            }
            cout<<"-----------follow集是----------------"<<endl;
            set<char>::iterator it1,p3;
            for(it1=nonterminal.begin();it1!=nonterminal.end();it1++)
            {
                cout<<*it1<<": ";
                set<char>temp=follow[*it1];
                for(p3=temp.begin();p3!=temp.end();p3++)
                    cout<<*p3<<" ";
                cout<<endl;
            }
            set<edge>::iterator it3;
            cout<<"-----------得到的DFA图是-------------"<<endl;
            for(int i=0;i<dfagraph.size();i++)
            {
                cout<<i<<" ";
                for(it3=dfagraph[i].begin();it3!=dfagraph[i].end();it3++)
                    cout<<"经过"<<(*it3).weight<<"到达"<<(*it3).nextid<<"   ";
                cout<<endl;
            }
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
    table  test;
    test.show();
    test.analy();
}
