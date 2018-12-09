#include<stdio.h>
#include<stdlib.h>
#include<map>
#include<vector>
#include<set>
#include<stack>
#include"firstandfollow.h"
#include<algorithm>
#include<iostream>
using namespace std;
//存放终结符下的产生式
struct node
{
    string symbol;
    int choice;
    node(string a,int b)
    {
        symbol=a;
        choice=b;
    }
    //终结符遇到某些非终结符后没有产生式对应
    node(string a)
    {
        symbol=a;
        choice=-1;
    }
};
//继承要利用到first集和follow集
class  Analy:public GetSet
{
    public:
        map<string,vector<node> >LLtable;//预测分析表
        stack<string>expstack;//存放表达式
        stack<string>analystack;//分析栈
        string start;//开始符号
        //看的是产生后面跟的一个终结符或非终结符
        void getTable()
        {
             int i;//当前产生式
             for(i=0;i<n;i++)
             {
                 int flag=0;
                 string tleft=myinit[i].left;
                 string tright=myinit[i].right[0];
                 if(IsTerminal(tright))
                 {
                      if(tright=="#")
                      {
                          set<string>tfollow=follow[tleft];
                          set<string>::iterator p;
                          for(p=tfollow.begin();p!=tfollow.end();p++)
                          {
                                node temp(*p,i);
                                LLtable[tleft].push_back(temp);
                          }
                      }
                      else
                      {
                          node temp(tright,i);
                          LLtable[tleft].push_back(temp);
                      }
                 }
                 set<string>tfirst=first[tright];
                 set<string>::iterator it;
                 for(it=tfirst.begin();it!=tfirst.end();it++)
                 {
                      if(*it=="#")
                         flag=1;
                      else
                      {
                            node temp(*it,i);
                            LLtable[tleft].push_back(temp);
                      }
                 }
                 if(flag==1)
                 {
                     set<string>tfollow=follow[tleft];
                     set<string>::iterator p;
                     for(p=tfollow.begin();p!=tfollow.end();p++)
                     {
                            node temp(*p,i);
                            LLtable[tleft].push_back(temp);
                     }
                 }
             }
        }
        //利用分析表进行分析
        void  analyProcedure()
        {
             start=myinit[0].left;
             expstack.push("$");
             analystack.push("$");
             analystack.push(start);
             string  token;
             cout<<"请输入表达式:"<<endl;
             getline(cin,token);
             int i=0;//记得初始化
             vector<string>temp;
             while(i!=token.size())
             {
                 while(i!=token.size()&&token[i]==' ')
                    i++;
                 int j=i;
                 while(j!=token.size()&&token[j]!=' ')
                    j++;
                 if(j!=i)
                 {
                     temp.push_back(token.substr(i,j-i));
                     i=j;
                 }
             }
             //把表达式反向压入表达式栈
             vector<string>::reverse_iterator  it;
             for(it=temp.rbegin();it!=temp.rend();it++)
             {
                  expstack.push(*it);
             }
             string  atop,etop;
             atop=analystack.top();
             etop=expstack.top();
             cout<<"当前栈顶"<<atop<<" "<<etop<<endl;
             while(!analystack.empty()&&!expstack.empty())
             {
                 //如果是终结符
                  if(IsTerminal(atop))
                  {
                      if(atop!=etop)
                      {
                          cout<<"语法错误"<<endl;
                          break;
                      }
                      else
                      {
                                cout<<atop<<"和"<<etop<<"匹配"<<endl;
                                analystack.pop();
                                atop=analystack.top();
                                expstack.pop();
                                etop=expstack.top();
                      }
                  }
                  //如果是非终结符,查分析表
                  else
                  {
                        analystack.pop();
                        int flag=0;
                        //int flag1=0;
                        vector<node>query=LLtable[atop];
                        for(int i=0;i<query.size();i++)
                        {
                            //如果找到了非终结符，找到相应的产生式
                            if(query[i].symbol==etop)
                            {
                                 flag=1;
                                 int ans=query[i].choice;
                                 cout<<"选择产生式:"<<ans<<endl;
                                 production temp=myinit[ans];
                                 vector<string>::reverse_iterator  it;
                                 //非终结符未进行处理出现错误
                                 for(it=temp.right.rbegin();it!=temp.right.rend();it++)
                                 {
                                        if(*it!="#")
                                          analystack.push(*it);
                                 }
                                 atop=analystack.top();
                            }
                        }
                        if(flag!=1)
                        {
                            cout<<"语法错误"<<endl;
                            break;
                        }
                  }
                  if(analystack.top()=="$"&&analystack.top()=="$")
                  {
                     cout<<"此token序列符合该文法的规则"<<endl;
                     break;
                  }
                  cout<<"当前栈顶"<<atop<<" "<<etop<<endl;
             }
        }
        void showtable()
        {
            input();
            FirstResult();
            FollowResult();
            show();
            getTable();
            cout<<"************************"<<endl;
            cout<<"LL1分析表"<<endl;
            set<string>::iterator it1;
            for(it1=nonterminal.begin();it1!=nonterminal.end();it1++)
            {
                cout<<*it1<<": ";
                vector<node>temp=LLtable[*it1];
                vector<node>::iterator p3;
                for(p3=temp.begin();p3!=temp.end();p3++)
                    cout<<(*p3).symbol<<" "<<(*p3).choice<<"  ";
                cout<<endl;
            }
            analyProcedure();
        }
};
int main()
{
    Analy test;
    test.showtable();
}
