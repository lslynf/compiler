#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<vector>
#include<fstream>
#include<iostream>
using namespace std;
ofstream  f2("D:\\ans.txt");
//这个算法要求输入的文法不含有形如s->s的推导且不含有以空字符串为右部
//思路:产生式的结构
class production
{
   public:
    string left;
    vector<string>right;
    production(string a)
    {
        left=a;
        right.clear();
    }
    void add(string b)
    {
        right.push_back(b);
    }
};
//存放产生式
vector<production>input;
//消除间接左递归
void  LeftCursionRemove()
{
    int i,j;
    for(i=0;i<input.size();i++)
    {
        int flag=0;
        for(j=0;j<i;j++)
        {
            //char ch=input[j].left[0];
            string ch=input[j].left;
            vector<string>&iright=input[i].right;
            vector<string>&jright=input[j].right;
            vector<string>temp;
            vector<string>::iterator p1,p2;
            for(p1=iright.begin();p1!=iright.end();p1++)
            {
                if((*p1).size()>=ch.size())
               {
                    string  compare=(*p1).substr(0,ch.size());
                    if(compare==ch)
                    {
                        //保存产生式j的右部
                        flag=1;
                        for(p2=jright.begin();p2!=jright.end();p2++)
                        {
                            temp.push_back(*p2+(*p1).substr(ch.size()));
                            //cout<<*p2+(*p1).substr(ch.size())<<"得到";
                        }
                    }
               }
            }
            //改变产生i的右部
            if(flag==1)
            {
                for(p1=iright.begin();p1!=iright.end();p1++)
                {
                        if((*p1).size()>=ch.size())
                        {
                            string str1=(*p1).substr(0,ch.size());
                            if(str1!=ch)
                             {
                                 temp.push_back(*p1);
                             }
                        }
                        else
                            temp.push_back(*p1);
                }
            iright.clear();
            for(int k=0;k<temp.size();k++)
                iright.push_back(temp[k]);
            }
        }
    }
    f2<<"消除间接左递归后"<<endl;
    for(int i=0;i<input.size();i++)
    {
        f2<<input[i].left<<"-> ";
        int j;
        for(j=0;j<input[i].right.size()-1;j++)
            f2<<input[i].right[j]<<"|";
            f2<<input[i].right[j];
        f2<<endl;
    }
}
//消除直接左递归
void DirectLeftCursionRemove()
{
    vector<production>append;
    for(int i=0;i<input.size();i++)
    {
         string ch=input[i].left;
         int flag=0;
         vector<string>&iright=input[i].right;
         vector<string>::iterator p1;
         vector<string>temp1;//存放产生式右部产生递归部分的后面的字符串
         vector<string>temp2;
         for(p1=iright.begin();p1!=iright.end();p1++)
         {
             //如果存在相同的头，则进行转换
                if((*p1).size()>=ch.size())
                {
                    string str=(*p1).substr(0,ch.size());
                    if(str==ch)
                    {
                         flag=1;
                         temp1.push_back((*p1).substr(ch.size()));
                         //cout<<"得到了"<<(*p1).substr(1)<<endl;
                    }
                    else
                    {
                         temp2.push_back(*p1);
                         //cout<<*p1<<endl;
                    }
                }
                else
                    temp2.push_back(*p1);
                //如果存在需要产生递归的产生式
         }
         if(flag==1)
            {
                string  str=input[i].left+"^";
                //cout<<"新产生式的左部:"<<str<<endl;
                production  newchoice(str);
                for(int k=0;k<temp1.size();k++)
                {
                    newchoice.add(temp1[k]+str);
                }
                newchoice.add("#");
                append.push_back(newchoice);
                iright.clear();
                for(int k=0;k<temp2.size();k++)
                {
                    iright.push_back(temp2[k]+str);
                }
            }
            temp2.clear();
    }
    for(int k=0;k<append.size();k++)
    {
        input.push_back(append[k]);
    }
    f2<<"消除直接左递归后"<<endl;
    for(int i=0;i<input.size();i++)
    {
        f2<<input[i].left<<"-> ";
        int j;
        for(j=0;j<input[i].right.size()-1;j++)
            f2<<input[i].right[j]<<"|";
        f2<<input[i].right[j];
        f2<<endl;
    }
}
int main()
{
    int n;
    cout<<"请输入产生式的个数:"<<endl;
    cin>>n;
    cin.ignore();
    /*while(n--)
    {
        string a;
        cout<<"请输入产生式的左部:"<<endl;
        cin>>a;
        production   temp(a);
        int num;
        cout<<"请输入该产生式的右部个数"<<endl;
        cin>>num;
        string b;
        while(num--)
        {
            cin>>b;
            temp.add(b);
        }
        input.push_back(temp);
    }*/
    while(n--)
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
        production anew(buff);
        int j=i+2;
        vector<string>sub;
        while(j!=temp.size())
        {
            if(j!=temp.size()&&temp[j]=='|')
                j++;
            int k=j;
            while(k!=temp.size()&&temp[k]!='|')
                k++;
            if(k!=j)
            {
                 sub.push_back(temp.substr(j,k-j));
                 j=k;
            }
        }
        for(int i=0;i<sub.size();i++)
            anew.add(sub[i]);
        input.push_back(anew);
    }
    LeftCursionRemove();
    DirectLeftCursionRemove();
}
