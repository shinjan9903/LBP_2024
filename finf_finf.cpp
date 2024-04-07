#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
// #include "sttrees/st_splay.h"
// #include "sttrees/st_linear.h"
// #include "sttrees/st_edge_splay.h"
// #include "sttrees/st_edge_linear.h"
// #include "ettrees/et_tree.h"
// #include "sttrees/st_splay.cpp"

#define ll long long
using namespace std;
const int N=1e6+10;
const int M=6e6+10;
const int MM=3e6;
const int INF=1e9;
struct dart{
    ll v;
    ll dir;
    ll next;
};

struct edge{
    ll u; 
    ll v; 
    ll uindex;
    ll vindex;
    ll wt;
};

vector<pair<ll,ll>> graph[N];
vector<pair<ll,ll>> dual_graph[N];
vector<pair<ll,ll>> interdigitating_tree[N];
vector<pair<ll,ll>> query_edge_list;
vector<pair<ll,ll>> primal_edge_list;
vector<ll> tension;
edge edge_list[M];
vector<int> d; // store distance
vector<int> p; // store parent
vector<int> optiden; //this is the iden of edge for which the minimum distance is set and we need to pop this edge
int face_num[MM][2];
bool used[MM][2];


int main(){

    freopen("embedding1e6.txt", "r", stdin);

    ll n; cin>>n;
    // graph.resize(n+1);
    int m=0;
    for(int i=1;i<=n;i++){
        ll sz; cin>>sz;
        while(sz--){    
            ll iden,y,wt; cin>>iden>>y>>wt;
            m++;
            if(y==1){
                edge_list[iden].u=i;
                edge_list[iden].uindex=graph[i].size();
            }
            else {
                edge_list[iden].v=i;
                edge_list[iden].vindex=graph[i].size();
            }
            edge_list[iden].wt=wt;
            graph[i].push_back({iden,y});

        }
    }
    m=m/2;
    int cur=1;
    int faces=1;
    vector<pair<int, int>> finf;
    
    // // face num denotes the face number associated with a dart {i,j}
    
    memset(used, 0 , sizeof(used));
    cout<<"faces\n";
    while(cur<m+1){
        int dir;
        if(used[cur][0] && used[cur][1]){
            cur++; continue;
        }
        else if(used[cur][0]){
            dir=1;
        }
        else dir=0;
        int now=cur;
        int dir_initial=dir;
        // cout<<"\n"<<faces<<"\n";
        do{
            face_num[now][dir]=faces;
            used[now][dir]=1;
            ll v,index;
            if(dir==1){
                v=edge_list[now].v;
                index=edge_list[now].vindex;
            }
            else{
                v=edge_list[now].u;
                index=edge_list[now].uindex; 
            }
           
            // cout<<v<<" ";
            index=(index+1)%graph[v].size();
            now=graph[v][index].first;
            if(faces==2){finf.push_back({v,now});}
            dir=graph[v][index].second;

        } while(now!=cur || dir!=dir_initial);
        faces++;
        // cout<<"\n";

    }

    for(auto x: finf)cout<<x.second<<" ";

    cout<<"hello1\n";


   


    return 0;


    

}