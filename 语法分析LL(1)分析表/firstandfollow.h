#ifndef   FIRSTANDFOLLOW_H
#define   FIRSTANDFOLLOW_H

#include<stdio.h>
#include<stdlib.h>
#include<map>
#include<vector>
#include<set>
#include<algorithm>
#include<iostream>
using namespace std;
class  production
{
    public:
        string  left;
        vector<string>right;
        production(){}
        production(string a)
        {
            left=a;
            right.clear();
        }
        void  add(string b)
        {
            right.push_back(b);
        }
};
//�õ�first����follow��
class  GetSet
{
    public:
        map<int,production>myinit;//��Ų���ʽ
        vector<string>index;//�õ�����ʽ��ͼ�е�λ��,��һ�����⣬��ͬ�Ļ����
        map<string,set<string> >first;
        map<string,set<string> >follow;
        set<string>nonterminal;//���ս��
        set<string>terminal;//�ս��
        set<string>allcharacter;//������еķ�������ս������õ��ս��
        int n;
        //ע��:Ҫ�������ŵ�����
        void input()
        {
            cout<<"���������ʽ�ĸ���"<<endl;
            cin>>n;
            cin.ignore();//��������Իس����������뻺���������ݣ�������һ���������һ�������Ӱ��
            for(int k=0;k<n;k++)
            {
                string temp;
                getline(cin,temp);
                int i;
                string buff;
                for(i=0;i<temp.size();i++)
                {
                    if(temp[i]=='-')
                        break;
                    buff+=temp[i];
                }
                nonterminal.insert(buff);//���뵽���ս��
                production anew(buff);
                int j=i+2;
                vector<string>sub;
                while(j!=temp.size())
                {
                     while(j!=temp.size()&&temp[j]==' ')
                        j++;
                     int m=j;
                     while(m!=temp.size()&&temp[m]!=' ')
                        {
                            if(temp[m]=='(')
                            {
                                sub.push_back("(");
                                m++;
                                j++;
                                while(m!=temp.size()&&temp[m]!=')')
                                {
                                    m++;
                                }
                                sub.push_back(temp.substr(j,m-j));
                                j=m;
                            }
                            else
                                m++;
                        }
                     if(j!=m)
                     {
                         sub.push_back(temp.substr(j,m-j));
                         j=m;
                     }
                }
                 for(int p=0;p<sub.size();p++)
                    {
                        anew.add(sub[p]);
                        allcharacter.insert(sub[p]);
                    }
                 myinit[k]=anew;
                 index.push_back(anew.left);
            }
            set_difference(allcharacter.begin(),allcharacter.end(),nonterminal.begin(),nonterminal.end(),inserter(terminal,terminal.begin()));
            //ע��Ҫ��$������ս��
            terminal.insert("$");
        }
        //�ж��Ƿ����ս��
        bool IsTerminal(string a)
        {
            set<string>::iterator p2;
            for(p2=terminal.begin();p2!=terminal.end();p2++)
            {
                if(*p2==a)
                    return true;
            }
            return false;
        }
        vector<int> getindex(string a)
        {
             vector<int >b;
             int j=0;
             for(int i=0;i<n;i++)
             {
                 if(index[i]==a)
                     b.push_back(i);
             }
             return b;
        }
        void getFirst(int position)
        {
                  int flag=0;
                  int cnt=0;
                  string tleft=myinit[position].left;
                  vector<string>tright=myinit[position].right;
                  if(IsTerminal(tright[0]))
                  {
                        first[tleft].insert(tright[0]);
                  }
                  else
                  {
                        for(int j=0;j<tright.size();j++)
                        {
                               if(IsTerminal(tright[j]))
                               {
                                   first[tleft].insert(tright[j]);
                                   break;
                               }
                                    //�����˷��ս�����ݹ����first��
                               vector<int >temp=getindex(tright[j]);
                               for(int i=0;i<temp.size();i++)
                                    getFirst(temp[i]);
                               set<string>::iterator it;
                                //�������ķ��ս����first�����д���
                               for(it=first[tright[j]].begin();it!=first[tright[j]].end();it++)
                                {
                                    if(*it=="#")
                                       flag=1;
                                    else
                                       first[tleft].insert(*it);
                                }
                               if(flag==0)
                               {
                                   break;
                               }
                               else
                               {
                                   cnt+=flag;
                                   flag=0;
                               }
                        }
                        if(cnt==tright.size())
                            first[tleft].insert("#");
                  }
        }
        void  FirstResult()
        {
            for(int i=0;i<n;i++)
                getFirst(i);
        }
        void getFollow(string character)
        {
              for(int i=0;i<n;i++)
              {
                   int position=-1;
                   string  tleft=myinit[i].left;
                   vector<string>tright=myinit[i].right;
                   for(int j=0;j<tright.size();j++)
                    {
                        if(tright[j]==character)
                        {
                             position=j;
                             break;
                        }
                    }
                        //������滹�з���
                        if(position!=-1&&position<tright.size()-1)
                        {
                            string next=tright[position+1];
                            if(IsTerminal(next))
                            {
                                 follow[character].insert(next);
                            }
                            //�����ս�����������first��
                            else
                            {
                                int flag=0;
                                set<string>::iterator  it;
                                for(it=first[next].begin();it!=first[next].end();it++)
                                {
                                    if(*it=="#")
                                        flag=1;
                                    else
                                        follow[character].insert(*it);
                                }
                                if(flag==1&&tleft!=character)
                                {
                                     getFollow(tleft);
                                     set<string>::iterator it;
                                     for(it=follow[tleft].begin();it!=follow[tleft].end();it++)
                                        follow[character].insert(*it);
                                }
                            }
                        }
                        else if(position!=-1&&position==tright.size()-1&&tleft!=character)
                        {
                            getFollow(tleft);
                            set<string>::iterator it;
                            for(it=follow[tleft].begin();it!=follow[tleft].end();it++)
                                follow[character].insert(*it);
                        }
              }
        }
        void FollowResult()
        {
            string start=myinit[0].left;
            follow[start].insert("$");
            set<string>::iterator it;
            for(it=nonterminal.begin();it!=nonterminal.end();it++)
                getFollow(*it);
        }
        void show()
        {
            /*for(int i=0;i<myinit.size();i++)
            {
                cout<<myinit[i].left<<"-> ";
                for(vector<string>::iterator it=myinit[i].right.begin();it!=myinit[i].right.end();it++)
                    cout<<*it<<" ";
                cout<<endl;
            }
            set<string>::iterator p1,p2;
            for(p1=nonterminal.begin();p1!=nonterminal.end();p1++)
                cout<<*p1<<" ";
            cout<<endl;
            for(p2=terminal.begin();p2!=terminal.end();p2++)
                cout<<*p2<<" ";
            cout<<endl;*/
            cout<<"************************"<<endl;
            cout<<"first����:"<<endl;
            set<string>::iterator it,p;
            for(it=nonterminal.begin();it!=nonterminal.end();it++)
            {
                cout<<*it<<": ";
                set<string>temp=first[*it];
                for(p=temp.begin();p!=temp.end();p++)
                    cout<<*p<<" ";
                cout<<endl;
            }
            cout<<"************************"<<endl;
            cout<<"follow����:"<<endl;
            set<string>::iterator it1,p3;
            for(it1=nonterminal.begin();it1!=nonterminal.end();it1++)
            {
                cout<<*it1<<": ";
                set<string>temp=follow[*it1];
                for(p3=temp.begin();p3!=temp.end();p3++)
                    cout<<*p3<<" ";
                cout<<endl;
            }
        }
};
#endif
