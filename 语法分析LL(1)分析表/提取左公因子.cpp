#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;
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
vector<production>input;
string getcom(string str1,string str2)
{
    int i=0;
    string ans;
    while(i!=str1.size()&&i!=str2.size()&&str1[i]==str2[i])
        {
            ans+=str1[i++];
            //i++;
        }
   // cout<<ans<<endl;
    return ans;
}
//提取左公因子
void  LeftFactor()
{
     vector<production>append;
     for(int i=0;i<input.size();i++)
     {
         string ch=input[i].left;
         vector<string>&temp=input[i].right;
         string  compare=getcom(temp[0],temp[1]);
         vector<string>::iterator  p1;
         vector<string>temp1,temp2;
         string start=ch+"^";
         temp2.push_back(compare+start);
         for(p1=temp.begin();p1!=temp.end();p1++)
         {
               if((*p1).size()>compare.size())
               {
                    if((*p1).substr(0,compare.size())==compare)
                        temp1.push_back((*p1).substr(compare.size()));
                    else
                        temp2.push_back((*p1));
               }
               else if((*p1).size()==compare.size()&&*p1==compare)
               {
                    temp1.push_back("#");
               }
               else
                temp2.push_back((*p1));
         }
         temp.clear();
         for(int j=0;j<temp2.size();j++)
            temp.push_back(temp2[j]);
         production  newchoice(start);
         for(int j=0;j<temp1.size();j++)
            newchoice.add(temp1[j]);
         append.push_back(newchoice);
     }
     for(int k=0;k<append.size();k++)
     {
        input.push_back(append[k]);
     }
     cout<<"提取左公因子后:"<<endl;
     for(int i=0;i<input.size();i++)
    {
        cout<<input[i].left<<"-> ";
        int j;
        for(j=0;j<input[i].right.size()-1;j++)
            cout<<input[i].right[j]<<"|";
        cout<<input[i].right[j];
        cout<<endl;
    }
}
int main()
{
       int n;
       cout<<"请输入产生式的个数"<<endl;
       cin>>n;
       cin.ignore();
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
           production  anew(buff);
           int j=i+2;
           vector<string>sub;
           while(j!=temp.size())
           {
               if(j!=temp.size()&&temp[j]=='|')
                    j++;
               int k=j;
               while(k!=temp.size()&&temp[k]!='|')
                   k++;
               if(j!=k)
               {
                   sub.push_back(temp.substr(j,k-j));
                   j=k;
               }
           }
           for(int k=0;k<sub.size();k++)
              anew.add(sub[k]);
            input.push_back(anew);
       }
       LeftFactor();
}
