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
//����ս���µĲ���ʽ
struct node
{
    string symbol;
    int choice;
    node(string a,int b)
    {
        symbol=a;
        choice=b;
    }
    //�ս������ĳЩ���ս����û�в���ʽ��Ӧ
    node(string a)
    {
        symbol=a;
        choice=-1;
    }
};
//�̳�Ҫ���õ�first����follow��
class  Analy:public GetSet
{
    public:
        map<string,vector<node> >LLtable;//Ԥ�������
        stack<string>expstack;//��ű��ʽ
        stack<string>analystack;//����ջ
        string start;//��ʼ����
        //�����ǲ����������һ���ս������ս��
        void getTable()
        {
             int i;//��ǰ����ʽ
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
        //���÷�������з���
        void  analyProcedure()
        {
             start=myinit[0].left;
             expstack.push("$");
             analystack.push("$");
             analystack.push(start);
             string  token;
             cout<<"��������ʽ:"<<endl;
             getline(cin,token);
             int i=0;//�ǵó�ʼ��
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
             //�ѱ��ʽ����ѹ����ʽջ
             vector<string>::reverse_iterator  it;
             for(it=temp.rbegin();it!=temp.rend();it++)
             {
                  expstack.push(*it);
             }
             string  atop,etop;
             atop=analystack.top();
             etop=expstack.top();
             cout<<"��ǰջ��"<<atop<<" "<<etop<<endl;
             while(!analystack.empty()&&!expstack.empty())
             {
                 //������ս��
                  if(IsTerminal(atop))
                  {
                      if(atop!=etop)
                      {
                          cout<<"�﷨����"<<endl;
                          break;
                      }
                      else
                      {
                                cout<<atop<<"��"<<etop<<"ƥ��"<<endl;
                                analystack.pop();
                                atop=analystack.top();
                                expstack.pop();
                                etop=expstack.top();
                      }
                  }
                  //����Ƿ��ս��,�������
                  else
                  {
                        analystack.pop();
                        int flag=0;
                        //int flag1=0;
                        vector<node>query=LLtable[atop];
                        for(int i=0;i<query.size();i++)
                        {
                            //����ҵ��˷��ս�����ҵ���Ӧ�Ĳ���ʽ
                            if(query[i].symbol==etop)
                            {
                                 flag=1;
                                 int ans=query[i].choice;
                                 cout<<"ѡ�����ʽ:"<<ans<<endl;
                                 production temp=myinit[ans];
                                 vector<string>::reverse_iterator  it;
                                 //���ս��δ���д�����ִ���
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
                            cout<<"�﷨����"<<endl;
                            break;
                        }
                  }
                  if(analystack.top()=="$"&&analystack.top()=="$")
                  {
                     cout<<"��token���з��ϸ��ķ��Ĺ���"<<endl;
                     break;
                  }
                  cout<<"��ǰջ��"<<atop<<" "<<etop<<endl;
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
            cout<<"LL1������"<<endl;
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
