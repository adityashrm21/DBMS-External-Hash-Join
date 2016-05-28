#include<bits/stdc++.h>

FILE *fp;
using namespace std;

namespace patch
{
    template < typename T > string to_string( const T& n )
    {
        ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

using namespace patch;

string binary(int n)
{
    int i;
    string bin;
    vector<int> arr;

     while(n>0)
    {
        i=0;
        while(pow(2,i)<=n) i++;
        i--;
        arr.push_back(i);
        n-=pow(2,i);
     }

    for(int i=15;i>=0;i--)
    {
        if(find(arr.begin(), arr.end(), i)==arr.end())
        bin+="0";
        else bin+="1";
    }
    return bin;
}

int decimal(string s)
{
    int i, ans=0;
    for(int i=s.length()-1;i>=0;i--)
    {
        if(s[i]=='1')
        ans+=pow(2,i);
    }

    return ans;
}



vector<int> join(vector<int> reln1, vector<int> reln2)
{

    vector<int> ans;
    int m=reln1.size(), n=reln2.size(), flag;
    if(min(m,n)==m)
    flag=1;
    else flag=2;

    if(flag==1)
    {
        for(int i=0;i<min(m,n);i++)
        {
            if(find(reln2.begin(),reln2.end(),reln1[i])!=reln2.end())
            ans.push_back(reln1[i]);

        }
    }
        if(flag==2)
    {
        for(int i=0;i<min(m,n);i++)
        {
            if(find(reln1.begin(),reln1.end(),reln2[i])!=reln1.end())
            ans.push_back(reln2[i]);

        }
    }

    return ans;
}
void hashFun(vector<pair<vector<int>,int>>& buckets,vector<int>& pagesreln, int n, int pages, int recPerPage, int relation_no, int round)
{
    //vector<pair<vector<int>,int>> buckets(pages-1);
    int I=pages-1;
    string bin_rep=binary(n);
    string s=bin_rep.substr(16-round-I, round+I);
    //cout<<s<<endl;
    //for(int )
    string temp;
    int buck=decimal(s);

    buck = buck%I;

    FILE *out1;
    buckets[buck].first.push_back(n);

    if(pagesreln[buck]==0)
    {
       pagesreln[buck]++;
    }


    buckets[buck].second++;
    if(buckets[buck].second>recPerPage)
    {
        //cout<<"Page for bucket "<<buck<<" full. Flushing page to the secondary memory.\n\n";
        pagesreln[buck]++;
        string filename="relation"+to_string(relation_no)+".round"+to_string(round)+".bucket"+to_string(buck);
        temp=filename+".txt";
        buckets[buck].second=0;
       // cout<<"$\n";
        out1 = fopen(temp.c_str(),"w");
        //out1.open(filename.c_str(), fstream::out);

            for(int j=0;j<buckets[buck].first.size();j++)
            {
                fprintf(out1,"%d\n",buckets[buck].first[j]);

            }

        fclose(out1);
 //       out1.close();
    }

    //return buckets;
}

void recursive_hash(string file_rel1, string file_rel2, int pages,int pageSize, int recSize1, int recSize2,int hashRounds, int hashRound, int bucket_no)
{


     if(hashRound>hashRounds)
    {
        cout<<"\nUnable to perform join in the given number of rounds of hashing\n\n";
        return;
    }
    cout<<"round "<<hashRound<<":\n";

    ifstream reln1, reln2;
    FILE *reln1_out, *reln2_out;

    string temp=file_rel1+".txt";
    reln1.open(temp.c_str());
    temp=file_rel2+".txt";
    reln2.open(temp.c_str());

    vector<int> pagesreln1(pages-1,0), relation1;
    vector<int> pagesreln2(pages-1,0), relation2;

    int recPerPage1=pageSize/recSize1;
    int recPerPage2=pageSize/recSize2;

    char out[100];
    int flush_page1=0, flush_page2=0;

    if (reln1.is_open())
    {
        while (reln1>>out)
        {
            //cout<<out<<endl;
            relation1.push_back(stoi(out));
        }
    }

    reln1.close();

    int s1=pages-1,ss1=relation1.size();
    int pages1 = ss1/recPerPage1;
    if(ss1%recPerPage1 != 0)
        pages1++;

    //cout<<"Size of relation 1: "<<pages1<<" pages\n"<<endl;

    if (reln2.is_open())
    {
        while (reln2 >> out)
        {
            //cout<<out<<endl;
            relation2.push_back(stoi(out));

        }
    }
    reln2.close();

    int s2=pages-1,ss2=relation2.size();
    //int c=0;
    int pages2 = ss2/recPerPage2;
    if(ss2%recPerPage2 != 0)
        pages2++;
   // cout<<"Size of relation 2: "<<pages2<<" pages\n"<<endl;

    //cout<<"Total number of available pages: "<<pages<<endl<<endl;

    vector<pair<vector<int>, int>> buckets1(pages-1);
    vector<pair<vector<int>, int>> buckets2(pages-1);

    for(int i=0;i<ss1;i++)
            hashFun(buckets1,pagesreln1,relation1[i],pages, recPerPage1, 1, hashRound);
            //cout<<"Tuple "<<i<<": "<<relation1[i]<<" Mapped to bucket: "<<buckets1

    for(int i=0;i<pages-1;i++)
    {
        //cout<<"Size of bucket "<<i<<": "<<pagesreln1[i]<<endl;
    }

    cout<<"\nProcessed relation 1\n";

        for(int i=0;i<ss2;i++)
        hashFun(buckets2,pagesreln2,relation2[i],pages, recPerPage2, 2, hashRound);


    for(int i=0;i<pages-1;i++)
    {
        //cout<<"Size of bucket "<<i<<": "<<pagesreln2[i]<<endl;
    }
    cout<<"\nProcessed relation 2\n";
    int count=0;

    for(int i=0;i<pages-1;i++)
    {
        cout<<"\nbucket"<<i<<": \n";
        cout<<"R1 keys: ";
        for(int j=0;j<buckets1[i].first.size();j++)
        {    if(!buckets2[i].first.empty())
                cout<<buckets1[i].first[j]<<", ";
        }

        cout<<"Pages: "<<pagesreln1[i]<<" ";
        cout<<"R2 keys: ";
        for(int j=0;j<buckets2[i].first.size();j++)
        {   if(!buckets2[i].first.empty())
                cout<<buckets2[i].first[j]<<", ";
        }
        cout<<"Pages: "<<pagesreln2[i]<<" ";
        //cout<<endl;
        if(pagesreln1[i]+pagesreln2[i]<pages)
        {
            cout<<"in memory join : yes\n";
            count++;
            if(count==pages-1)
            return;
        }
        else cout<<" in memory join : no\n";
    }
    /*cout<<endl;
    for(int i=0;i<buckets2.size();i++)
    {
        cout<<"\nbucket "<<i<<": ";
        cout<<"R2 keys: ";
        for(int j=0;j<buckets2[i].first.size();j++)
        {   if(!buckets2[i].first.empty())
                cout<<buckets2[i].first[j]<<", ";
        }
        cout<<"Pages: "<<pagesreln2[i];
        //cout<<endl;
    }*/
    cout<<endl;
// base case for recursion


        string file_name1, file_name2;


        if(pages1+pages2>=pages)
        {

        for(int i=0;i<pages-1;i++)
        {

            file_name1=file_rel1+".round"+to_string(hashRound)+".bucket"+to_string(i);
            temp=file_name1+".txt";
            file_name2=file_rel2+".round"+to_string(hashRound)+".bucket"+to_string(i);

            //reln1_out.open(file_name1.c_str(), fstream::out);
            reln1_out=fopen(temp.c_str(),"w");

            //reln1_out<<buckets1[j].first->val;
                //cout<<"Bucket "<<i<<": ";
                for(int j=0;j<buckets1[i].first.size();j++)
                {
                    fprintf(reln1_out,"%d\n",buckets1[i].first[j]);
                    //cout<<buckets1[i].first[j]<<" ";
                }

            //reln1_out.close();
            fclose(reln1_out);

            //reln2_out.open(file_name2.c_str(), fstream::out);
            temp=file_name2+".txt";
            reln2_out=fopen(temp.c_str(),"w");
               for(int j=0; j<buckets2[i].first.size();j++)
                    fprintf(reln2_out,"%d\n",buckets2[i].first[j]);

            fclose(reln2_out);

            recursive_hash(file_name1, file_name2, pages, pageSize, recSize1, recSize2,hashRounds, hashRound+1, i);

        }
    }

    else
    {
        vector< vector<int>> result;
        vector<int> res, all_values;

        //cout<<"In memory join: Yes .\n\n";
        for(int i=0;i<pages-1;i++)
        {

            res=join(buckets1[i].first,buckets2[i].first);
            result.push_back(res);

        }



        for(int i=0;i<result.size();i++)
        {
            for(int j=0;j<result[i].size();j++)
                {
                    all_values.push_back(result[i][j]);

                }
        }

        /*vector<int>::iterator it;
        it = unique (all_values.begin(), all_values.end());
        all_values.resize(distance(all_values.begin(),it) );*/




        for(int i=0;i<all_values.size();i++)
        {
            //cout<<all_values[i]<<endl;
            fprintf(fp,"%d\n",all_values[i]);
        }

    }

}

main()
{

    fp=fopen("final_join.txt","w");

    int recSize1, recSize2, pageSize, pages, hashRounds, recPerPage1, recPerPage2;

    cout<< "enter record 1 size\n"; cin>> recSize1;
    cout<< "enter record 2 size\n"; cin>> recSize2;
    cout<< "enter page size\n"; cin>> pageSize;
    cout<< "enter numbers of pages\n"; cin>> pages;
    cout<< "enter number of hashing rounds\n"; cin>> hashRounds;
    recPerPage1=pageSize/recSize1;
    recPerPage2=pageSize/recSize2;

    string file1, file2;
    file1="relation1";
    file2="relation2";

    recursive_hash(file1, file2, pages, pageSize, recSize1, recSize2,hashRounds, 1, 0);

    fclose(fp);

    return 0;
}
