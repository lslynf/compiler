#ifndef  REGTONFA_H
#define REGTONFA_H

#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<set>
#include<map>
#include<stack>
using namespace std;
char op[]={'$','!','|','&','*','(',')'};
struct nfa_node
{
    int start;
    int end;
    string weight;
    nfa_node(int a,int b)
    {
        start=a;
        end=b;
        weight="#";
    }
    nfa_node(string c)//a,b状态在图中不存在
    {
        start=-1;
        end=-1;
        weight=c;
    }
};
struct edge_node//存权值和下一个结点的id号
{
    string weight;
    int nextstate;
    edge_node(string a,int id)
    {
        weight=a;
        nextstate=id;
    }
};
//set容器中放入结构体时需要运算符重载，定义排序规则
bool operator<(edge_node a,edge_node b)
{
    //return a.nextstate<b.nextstate;
    if(a.nextstate!=b.nextstate)
         return a.nextstate<b.nextstate;
    else
        return a.weight<b.weight;

}
//结果图，用来存结点和边的关系
class Graph
{
  public:
    map<int ,set<edge_node> >ResultGraph;
    void add(int start,int end,string weight)
    {
        edge_node  a(weight,end);
        ResultGraph[start].insert(a);
    }
    void add(int start,int end)
    {
        edge_node a("#",end);
        ResultGraph[start].insert(a);
    }
    void show(int i)
    {
        set<edge_node>::iterator it;
            cout<<i<<" ";
            for(it=ResultGraph[i].begin();it!=ResultGraph[i].end();it++)
            {
                cout<<(*it).nextstate<<" "<<(*it).weight<<" ";
            }
            cout<<endl;
    }
};
/*
思考有哪些步骤：
1.输入的正则表达式的修改，加入连接符号
2.正则表达式转后缀表达式
3.建立相应的操作，扫描转换后的正则表达式
*/
class  RegToNfa
{
    public:
        RegToNfa(string str)
        {
            Change(str);
            Suffixed(this->expression);
        }
        //扫描正则表达式
        nfa_node ScanReg(Graph&g,int&num,set<string>&character)
        {
            for(int i=0;i<this->expression.length();i++)
            {
                  char ch=expression[i];
                  if(isoperator(ch))
                  {
                      switch(ch)
                      {
                          case '|':Union(g,num);break;
                          case '&':Connect(g,num);break;
                          case '*':Closure(g,num);break;
                      }
                  }
                  //一个问题，要得到字符串,存在问题
                  else
                  {
                      string  astr=getstring(i,this->expression);
                      snode.push(nfa_node(astr));
                      character.insert(astr);
                  }
            }
            return snode.top();
        }
    public:
        string expression;//后缀正则表达式
        stack<nfa_node>snode;//存放节点的栈
        stack<char>operation;//存放操作符
        string getstring(int &i,string str)
        {
             string result="";
             int sum=0;
             while(!isoperator(str[i]))
             {
                 if(str[i]=='@')
                    {
                        sum++;
                        i++;
                    }
                 if(sum==2)
                    break;
                result+=str[i];
                i++;
             }
             i--;
             return result;
        }
        bool isoperator(char c)
        {
            bool flag=false;
            for(int i=0;i<7;i++)
            {
                if(c==op[i])
                {
                    flag=true;
                    break;
                }
            }
            return flag;
        }
        //添加连接符号
        void Change(string str)
        {
            int i=0;
            char front=str[i];
            char rear=str[i+1];
            //注意，定义正则表达式的输入时，用到了@符号，要考虑这种情况
            while(str[i]!='\0')
            {
                if(!isoperator(front)&&rear=='('||front==')'&&rear=='('||front=='*'&&rear=='('||front==')'&&!isoperator(rear)||front=='*'&&!isoperator(rear)||front=='@'&&rear=='@')
                           str.insert(i,"&");
                     front=str[i++];
                     rear=str[i];
            }
            str+='!';
            this->expression=str;
        }
        //定义优先级
        int priority(char ch)
            {
                  switch(ch)
                  {
                      case '$':return 0;
                      case '!':return 1;
                      case ')':return 2;
                      case '|':return 3;
                      case '&':return 4;
                      case '*':return 5;
                      case '(':return 6;
                      default:return 7;
                  }
            }
        bool compare(char op1,char op2)
        {
            if(priority(op1)<=priority(op2))
                return true;
            else
                return false;
        }
        //转后缀
        //遇到左括号直接压栈，遇到右括号，一直出栈，直到遇到左括号，左括号出栈，
        //遇到其他操作符，若优先级低于栈顶元素且栈顶不是右括号，一直出栈
        void Suffixed(string mid)
        {
            string buff="";
            operation.push('$');
            char ch;
            for(int i=0;i<mid.length();i++)
            {
                ch=mid[i];
                if(isoperator(ch))
                {
                    switch(ch)
                    {
                        case '(':operation.push(ch);
                                   break;
                        case ')':
                                  while(operation.top()!='(')
                                  {
                                       char pre=operation.top();
                                       buff+=pre;
                                       operation.pop();
                                  }
                                  operation.pop();
                                  break;
                        default:
                                  char pre=operation.top();
                                  while(compare(ch,pre)&&pre!='(')
                                  {
                                      buff+=pre;
                                      operation.pop();
                                      pre=operation.top();
                                  }
                                  operation.push(ch);
                                  break;
                    }
                }
                else
                {
                    buff+=ch;
                }
             }
                this->expression=buff;
        }
        //连接操作
        void Connect(Graph &g,int &num)
        {
             nfa_node  last=snode.top();//首先出栈的元素
             snode.pop();
             nfa_node  first=snode.top();//先对其操作
             snode.pop();
             int begin1,end1,begin2,end2;
             int begin,end;
             begin1=first.start;end1=first.end;begin2=last.start;end2=last.end;
             if(begin1==-1&&end1==-1&&begin2==-1&&end2==-1)
             {
                 num++;
                 begin1=num;
                 num++;
                 end1=num;
                 g.add(begin1,end1,first.weight);
                 begin2=end1;
                 num++;
                 end2=num;
                 g.add(begin2,end2,last.weight);
                 begin=begin1;
                 end=end2;
                 nfa_node  temp(begin,end);
                 snode.push(temp);
             }
             else if(begin1==-1&&end1==-1&&begin2!=-1&&end2!=-1)
             {
                 num++;
                 begin1=num;
                 end1=begin2;
                 g.add(begin1,end1,first.weight);
                 begin=begin1;
                 end=end2;
                 nfa_node temp(begin,end);
                 snode.push(temp);
             }
             else if(begin1!=-1&&end1!=-1&&begin2==-1&&end2==-1)
             {
                 begin2=end1;
                 num++;
                 end2=num;
                 g.add(begin2,end2,last.weight);
                 begin=begin1;
                 end=end2;
                 nfa_node temp(begin,end);
                 snode.push(temp);
             }
            else
            {
                 begin=begin1;
                 end=end2;
                 g.add(end1,begin2);
                 nfa_node temp(begin,end);
                 snode.push(temp);
            }
        }
        //或操作
        void Union(Graph&g,int &num)
        {
            nfa_node last=snode.top();
            snode.pop();
            nfa_node first=snode.top();
            snode.pop();
            int begin1,end1,begin2,end2;
            int begin,end;
            begin1=first.start;end1=first.end;begin2=last.start;end2=last.end;
            if(begin1==-1&&end1==-1&&begin2==-1&&end2==-1)
            {
                num++;
                begin1=num;
                num++;
                end1=num;
                g.add(begin1,end1,first.weight);
                num++;
                begin2=num;
                num++;
                end2=num;
                g.add(begin2,end2,last.weight);
                num++;
                begin=num;
                g.add(begin,begin1);
                g.add(begin,begin2);
                num++;
                end=num;
                g.add(end1,end);
                g.add(end2,end);
                nfa_node temp(begin,end);
                snode.push(temp);
            }
            else if(begin1==-1&&begin2!=-1)
            {
                num++;
                begin1=num;
                num++;
                end1=num;
                g.add(begin1,end1,first.weight);
                num++;
                begin=num;
                g.add(begin,begin2);
                g.add(begin,begin1);
                num++;
                end=num;
                g.add(end2,end);
                g.add(end1,end);
                nfa_node temp(begin,end);
                snode.push(temp);
            }
            else if(begin1!=-1&&end1!=-1&&begin2==-1&&end2==-1)
            {
                num++;
                begin2=num;
                num++;
                end2=num;
                g.add(begin2,end2,last.weight);
                num++;
                begin=num;
                g.add(begin,begin1);
                g.add(begin,begin2);
                num++;
                end=num;
                g.add(end1,end);
                g.add(end2,end);
                nfa_node temp(begin,end);
                snode.push(temp);
            }
            else
            {
                num++;
                begin=num;
                g.add(begin,begin1);
                g.add(begin,begin2);
                num++;
                end=num;
                g.add(end1,end);
                g.add(end2,end);
                nfa_node temp(begin,end);
                snode.push(temp);
            }
        }
        //闭包操作
        void Closure(Graph&g,int&num)
        {
            nfa_node first=snode.top();
            snode.pop();
            int begin1,end1;
            int begin,end;
            begin1=first.start;end1=first.end;
            if(begin1==-1&&end1==-1)
            {
                num++;
                begin1=num;
                num++;
                end1=num;
                g.add(begin1,end1,first.weight);
                g.add(end1,begin1);
                num++;
                begin=num;
                g.add(begin,begin1);
                num++;
                end=num;
                g.add(end1,end);
                g.add(begin,end);
                nfa_node temp(begin,end);
                snode.push(temp);
            }
            else
            {
                num++;
                begin=num;
                num++;
                end=num;
                g.add(begin,begin1);
                g.add(begin,end);
                g.add(end1,begin1);
                g.add(end1,end);
                nfa_node temp(begin,end);
                snode.push(temp);
            }
        }
};
#endif
