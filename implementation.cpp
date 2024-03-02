#include<bits/stdc++.h>
using namespace std;
const int INF=1e9;
const int fn=10,n=10;

int roots[fn];
int delta[fn][n];
int s[fn][n];
int dist[fn][3][n];
int par[fn][3][n];

struct edge{
    int v,dir,wt;
};

const int INF = 1000000000;

void dijkstra(int s, int i,int k, const vector<vector<edge>> &adj) {
    int n = adj.size();

    for(int x=0;x<n;x++){
        dist[i][k][x]=INF;
        par[i][k][x]=-1;
    }

    dist[i][k][s] = 0;
    using pii = pair<int, int>;
    priority_queue<pii, vector<pii>, greater<pii>> q;
    q.push({0, s});
    while (!q.empty()) {
        int v = q.top().second;
        int d_v = q.top().first;
        q.pop();
        if (d_v != dist[i][k][v])
            continue;

        for (auto edge : adj[v]) {
            if(edge.dir==0)continue;
            int to = edge.v;
            int len = edge.wt;
            if (dist[i][k][v] + len < dist[i][k][to]) {
                dist[i][k][to] = dist[i][k][v] + len;
                par[i][k][to] = v;
                q.push({dist[i][k][to], to});
            }
        }
    }
}

void dfs(int v, vector<vector<int>>&common_edges, vector<bool> &in_ts){
    in_ts[v]=1;
    for(auto child: common_edges[v]){
        dfs(child, common_edges, in_ts);
    }
}

void dfs3(int u, vector<vector<edge>> &g, vector<vector<edge>> &contracted_g,vector<bool> &in_ts, vector<int> &mini, int source){
    for(auto it: g[u]){
        if(it.dir==1){
            if(in_ts[it.v])dfs3(it.v,g,contracted_g,in_ts,mini,source);
            else{
                mini[it.v]=min(mini[it.v],0); // update 0 with (dist[s,u]+weight[u,v])
            }
        }
    }
}

void  dfs2(int u, vector<vector<edge>> &g, vector<vector<edge>> &contracted_g,vector<bool> &in_ts,vector<int> &mini, int i){
    if(in_ts[u]){
        for(auto it: g[u]){
            if(in_ts[it.v])dfs2(it.v,g,contracted_g,in_ts,mini,i);
            else if(it.dir==0){
                if(u==s[i][u]){  
                    contracted_g[u].push_back(it);
                    // adding edge as it was
                }
                else{
                    //set infinity
                    ;
                } 
            }
            else{
                //dir=1
                // push with updated weights(adding delta)
                edge e; 
                e.v=it.v; e.dir=it.dir; e.wt=it.wt+delta[i][u];
                if(mini[u]==0)contracted_g[s[i][u]].push_back(e); // replace 0 with dist[s,u]+weight[u,v]
            }
        }
    }    
}

vector<vector<edge>> contract(vector<vector<edge>> &g, vector<vector<int>>&common_edges, int source, int i){
    vector<bool> in_ts(n,0);
    vector<int> mini(n,INF);

    dfs(source, common_edges,in_ts);
    for(int u=0;u<n;u++){
        if(in_ts[u]){
           s[i][u]=source;
           delta[i][u]=dist[i][0][u]-dist[i][0][source];
        }
    }
    vector<vector<edge>> contracted_g;

    dfs3(source,g,contracted_g,in_ts,mini, source);
    dfs2(source,g,contracted_g,in_ts,mini, i);
    
    for(int u=0;u<n;u++){
        if(in_ts[u])continue;
        for(auto it: g[u]){
            if(in_ts[it.v]){
                if(it.dir==0){
                    edge e; e.v=s[i][it.v]; e.dir=it.dir; e.wt=it.wt;
                    if(mini[u]==it.wt)contracted_g[u].push_back(e);
                }
                else{
                    if(it.v==s[i][it.v]){
                        contracted_g[u].push_back(it);
                    }
                    else{
                        ;//infinity
                    }
                }
            }
            else{
                contracted_g[u].push_back(it);
            }
           //update weights
        }
    }
}


void mssp(int i1,int i2,vector<vector<edge>> &g){
    int i=(i1+i2)/2;
    dijkstra(i1,i,0,g);
    dijkstra(i,i,1,g);
    dijkstra(i2,i,2,g);
    if(i2-i1<=1) return;
    for(int j=0;j<2;j++){   
        vector<vector<edge>> h=g;
        vector<vector<int>> common_edges;
        common_edges.resize(n);
        vector<int> sources;
        for(int x=0;x<n;x++){
            if(par[i][j][x]==par[i][j+1][x] && par[i][j][x]!=-1){
                common_edges[par[i][j][x]].push_back(x);
            }
            else sources.push_back(x);
        }
        
        for(auto it :sources){
            int p1=par[i][j][it]; int p2=par[i][j+1][it];
            bool in_ts[n]; memset(in_ts,0,sizeof(in_ts));
            for(auto child:common_edges[it]){
                in_ts[child]=1;
            }
            bool poss=1;
            bool check=0;
            //p2 p1
            for(auto it2 : g[it]){
                if(check==0 && it2.v==p1 && it2.dir==0){
                    poss=0; break;
                }
                if(it2.v==p2 && it2.dir==0){
                    check=1; 
                }
                if(it2.v==p1 && it2.dir==0){
                    check=0; 
                }
                if(it2.dir==1&&in_ts[it2.v]&&!check){
                    poss=0; break;
                }
            }
            if(poss){
                h=contract(h,common_edges, it, i);
                continue;
            }

            poss=1;
            check=0;
            //p1 p2
            for(auto it2 : g[it]){
                if(check==0 && it2.v==p2 && it2.dir==0){
                    poss=0; break;
                }
                if(it2.v==p2 && it2.dir==0){
                    check=0; 
                }
                if(it2.v==p1 && it2.dir==0){
                    check=1; 
                }
                if(it2.dir==1 && in_ts[it2.v] && !check){
                    poss=0; break;
                }
            }
            if(poss){
                h=contract(h,common_edges, it, i);
            } 
        }
      
        if(j==0)mssp(i1,i,h);
        else mssp(i, i2, h);
    }

}

int query(int source,int target,int i1,int i2){
    int i=(i1+i2)/2;
    if(source==i1) return dist[i][0][target];
    if(source==i2) return dist[i][2][target];
    if(source<=i) return query(s[(i1+i)/2][source],target,i1,i)+delta[i][target];
    else return query(s[(i+i2)/2][source],target,i,i2)+delta[i][target];
}

int main(){
	int n;cin>>n;
    vector<vector<edge>>
    for(int i=0;i<n;i++){

    }
}