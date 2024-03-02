#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int N=1e5+10;
const int M=2e5+10;

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
};

vector<pair<ll,ll>> graph[N];
vector<pair<ll,ll>> dual_graph[N];
vector<ll> interdigitating_tree[N];
edge edge_list[M];


int main(){
    ll n; cin>>n;
    int m=0;
    for(int i=1;i<=n;i++){
        ll sz; cin>>sz;
        while(sz--){    
            ll iden,y; cin>>iden>>y;
            m++;
            if(y==1){
                edge_list[iden].u=i;
                edge_list[iden].uindex=graph[i].size();
            }
            else {
                edge_list[iden].v=i;
                edge_list[iden].vindex=graph[i].size();
            }
            graph[i].push_back({iden,y});

        }
    }
    m=m/2;
    int cur=1;
    int faces=0;
    ll face_num[m+1][2];
    bool used[m+1][2];
    memset(used, 0 , sizeof(used));
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
            index=(index+1)%graph[v].size();
            now=graph[v][index].first;
            dir=graph[v][index].second;

        } while(now!=cur || dir!=dir_initial);
        faces++;

    }

    for(int i=1;i<=m;i++){
        dual_graph[face_num[i][1]].push_back({face_num[i][0], 1});
        dual_graph[face_num[i][0]].push_back({face_num[i][1], 0});
    }
    cout<<"DUAL GRAPH: \n\n";
    for(int i=0;i<faces;i++){
        cout<<i<<"\n";
        for(auto it : dual_graph[i]){
            cout<<it.first<<" "<<it.second<<"\n";
        }
        cout<<"\n";
    }
    vector<bool> in_spanning_tree(m+1,0);

    // Enter the edges in the minimum spanning tree T of primal graph
    for(int i=0;i<n-1;i++){
        ll iden; cin>>iden;
        in_spanning_tree[iden]=1;
    }
    for(int i=1;i<=m;i++){
        if(!in_spanning_tree[i]){
            cout<<face_num[i][1]<<" "<<face_num[i][0]<<"\n";
            interdigitating_tree[face_num[i][1]].push_back(face_num[i][0]);
        }
    }

    cout<<"INTERDIGITATING TREE: \n";
    for(int i=0;i<faces;i++){
        cout<<i<<"\n";
        for(auto it : interdigitating_tree[i]){
            cout<<it<<" ";
        }
        cout<<"\n\n";
    }
}