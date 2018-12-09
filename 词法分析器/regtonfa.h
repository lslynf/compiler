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
    nfa_node(string c)//a,b״̬��ͼ�в�����
    {
        start=-1;
        end=-1;
        weight=c;
    }
};
struct edge_node//��Ȩֵ����һ������id��
{
    string weight;
    int nextstate;
    edge_node(string a,int id)
    {
        weight=a;
        nextstate=id;
    }
};
//set�����з���ṹ��ʱ��Ҫ��������أ������������
bool operator<(edge_node a,edge_node b)
{
    //return a.nextstate<b.nextstate;
    if(a.nextstate!=b.nextstate)
         return a.nextstate<b.nextstate;
    else
        return a.weight<b.weight;

}
//���ͼ����������ͱߵĹ�ϵ
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
˼������Щ���裺
1.�����������ʽ���޸ģ��������ӷ���
2.������ʽת��׺���ʽ
3.������Ӧ�Ĳ�����ɨ��ת�����������ʽ
*/
class  RegToNfa
{
    public:
        RegToNfa(string str)
        {
            Change(str);
            Suffixed(this->expression);
        }
        //ɨ��������ʽ
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
                  //һ�����⣬Ҫ�õ��ַ���,��������
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
        string expression;//��׺������ʽ
        stack<nfa_node>snode;//��Žڵ��ջ
        stack<char>operation;//��Ų�����
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
        //������ӷ���
        void Change(string str)
        {
            int i=0;
            char front=str[i];
            char rear=str[i+1];
            //ע�⣬����������ʽ������ʱ���õ���@���ţ�Ҫ�����������
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
        //�������ȼ�
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
        //ת��׺
        //����������ֱ��ѹջ�����������ţ�һֱ��ջ��ֱ�����������ţ������ų�ջ��
        //���������������������ȼ�����ջ��Ԫ����ջ�����������ţ�һֱ��ջ
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
        //���Ӳ���
        void Connect(Graph &g,int &num)
        {
             nfa_node  last=snode.top();//���ȳ�ջ��Ԫ��
             snode.pop();
             nfa_node  first=snode.top();//�ȶ������
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
        //�����
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
        //�հ�����
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
