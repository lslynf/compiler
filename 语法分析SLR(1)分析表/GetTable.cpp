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
    int flag;//���ñ�־����Ϊ1���ʾ���룬��Ϊ2���ʾ��Լ,��Ϊ3���ʾ����
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
         //���ع�Լ��ı��
         int isreduce(vector<int>a)
         {
                for(int i=0;i<a.size();i++)
                {
                    if(items[a[i]].end==1||items[a[i]].end==2)
                        return a[i];
                }
                return -2;
         }
         //Ҫ�����ж��Ƿ��й�Լ��
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
                                  //���ͨ���ս��ת����һ��״̬
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
                         if(position>0)//������й�Լ��,��follow����д��Լ
                         {
                              if(items[position].end==2)//����ǰ�����ʼ����
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
                   else//���ڹ�Լ���Ҳ����п�ʼ��Ŀ,��Ҳ��ֻ��follow���²Ž��й�Լ
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
            cout<<"��������ʽ:"<<endl;
            cin>>token;
            expstack.push('$');
            analystack.push('$');
            analystack.push(0);
            for(int i=token.size()-1;i>=0;i--)
                expstack.push(token[i]);
            int atop=analystack.top();
            char etop=expstack.top();//�����ս��
            cout<<"------------������������---------------"<<endl;
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
                            //�����shift������ʽջ��һ��Ԫ��ѹ�뵽����ջ��ͬʱѹ����״̬
                            if(temp[i].flag==1)
                            {
                                  cout<<"shift"<<endl;
                                  analystack.push(etop);
                                  analystack.push(temp[i].nextstate);
                                  expstack.pop();
                                  atop=analystack.top();
                                  etop=expstack.top();
                            }
                            //����ǹ�Լ����������ʽ�Ҳ����ȳ�ջ��ͬʱѹ���󲿼���ǰջ�������󲿲����ĺ��״̬
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
                                 cout<<"��token���з����ķ���"<<endl;
                                 jump=1;
                                 break;
                            }
                       }
                  }
                  if(exist==0)
                        {
                              jump=1;
                              cout<<"�﷨����"<<endl;
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
            cout<<"--------------��ĿΪ---------------"<<endl;
            for(int i=0;i<items.size();i++)
            {
                cout<<items[i].left<<"->";
                cout<<items[i].right<<" ";
                cout<<items[i].id<<" ";
               // cout<<items[i].before<<" ";
               // cout<<items[i].end;
                cout<<endl;
            }
           /* cout<<"����Ŀ�ıհ���Ϊ:" <<endl;
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
            cout<<"------------first����---------------"<<endl;
            set<char>::iterator it,p;
            for(it=nonterminal.begin();it!=nonterminal.end();it++)
            {
                cout<<*it<<": ";
                set<char>temp=first[*it];
                for(p=temp.begin();p!=temp.end();p++)
                    cout<<*p<<" ";
                cout<<endl;
            }
            cout<<"-----------follow����----------------"<<endl;
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
            cout<<"-----------�õ���DFAͼ��-------------"<<endl;
            for(int i=0;i<dfagraph.size();i++)
            {
                cout<<i<<" ";
                for(it3=dfagraph[i].begin();it3!=dfagraph[i].end();it3++)
                    cout<<"����"<<(*it3).weight<<"����"<<(*it3).nextid<<"   ";
                cout<<endl;
            }
            cout<<"-----------action������---------"<<endl;
             for(int i=0;i<action.size();i++)
             {
                 cout<<i<<"  ";
                 for(int j=0;j<action[i].size();j++)
                    cout<<action[i][j].weight<<action[i][j].nextstate<<action[i][j].flag<<" ";
                cout<<endl;
             }
             cout<<"-----------go������----------"<<endl;
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
