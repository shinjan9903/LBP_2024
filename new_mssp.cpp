#include<bits/stdc++.h>
#define ld long double
using namespace std;
const int N=1000009;

vector<int> roots[N];
vector<int> roots_to_index[N];
vector<ld> delta[N];
vector<int> s[N];
vector<ld> dist[N][3];
vector<int> par[N][3];
vector<int> mapping[N];
bool vis[N];

long long p1[N],p2[N];
const long long prime1=1e9+9,prime2=1e7+19;
const int mod=1000000007;



struct edge{
    int v,dir;
    ld wt;
    edge(int v,int dir,ld wt){
        this->v=v;
        this->dir=dir;
        this->wt=wt;
    }

    bool operator==(const edge &a){
      return (a.v==v && a.dir==dir && a.wt==wt);
    }
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

void dfs(int v, vector<vector<int>>&common_edges, int i, int source){
    s[i][v]=source;
    delta[i][v]=dist[i/2][1][v]-dist[i/2][1][source];
    for(auto child: common_edges[v]){
        dfs(child, common_edges,i,source);
    }
}

void dfs3(int u, vector<vector<edge>> &g, vector<vector<edge>> &contracted_g, vector<ld> &m_out,vector<ld> &m_in, int i, int source){
    vis[u]=1;
    for(auto it: g[u]){
        if(it.dir==1){
            if(s[i][it.v]==source){
                if(!vis[it.v]) dfs3(it.v,g,contracted_g,m_out,m_in,i,source);
            }
            else{
                if(s[i][it.v]==it.v){
                    m_out[s[i][it.v]]=min(m_out[s[i][it.v]],it.wt+delta[i][u]); // update 0 with (dist[s,u]+weight[u,v])
                }
            }
        }
        else{
            if(u==source && s[i][it.v]!=source){
                m_in[s[i][it.v]]=min(m_in[s[i][it.v]],it.wt+delta[i][it.v]);
            }
        }
    }
}

void dfs2(int u, vector<vector<edge>> &g, vector<vector<edge>> &contracted_g,vector<ld> &m_out,vector<ld> &m_in, int i, int source, edge ee){
    vis[u]=1;
    ee.dir^=1;
    int ind=0;
    for(int x=0;x<g[u].size();x++){
      if(g[u][x]==ee){
        ind=x+1;
        break;
      }
    }
    for(int x=0;x<g[u].size();x++){
      edge it=g[u][(x+ind)%g[u].size()];
      if(it.dir==1){
            if(s[i][it.v]==source){
                if(!vis[it.v]) dfs2(it.v,g,contracted_g,m_out,m_in,i,source,it);
            }
            else{
                if(s[i][it.v]==it.v && m_out[s[i][it.v]]==it.wt+delta[i][u]){
                    contracted_g[s[i][u]].push_back(edge(it.v,it.dir,m_out[s[i][it.v]])); // replace 0 with dist[s,u]+weight[u,v]
                    m_out[s[i][it.v]]=INF;
                }
            }
        }
        else{
            if(u==source && s[i][it.v]!=source && m_in[s[i][it.v]]==it.wt+delta[i][it.v]){
                contracted_g[u].push_back(edge(s[i][it.v],it.dir,m_in[s[i][it.v]]));
                m_in[s[i][it.v]]=INF;
            }
        }
    }
}

vector<vector<edge>> contract(vector<vector<edge>> &g, vector<vector<int>>&common_edges, int i, vector<int>& sources){
    int n=g.size();
    vector<ld> m_out(n,INF);
    vector<ld> m_in(n,INF);

    for(auto source: sources)dfs(source, common_edges,i,source);

    vector<vector<edge>> contracted_g(n);
    for(int x=0;x<n;x++){
        if(s[i][x]!=x) continue;
        memset(vis,0,sizeof(vis));
        dfs3(x,g,contracted_g,m_out,m_in, i,x);
        memset(vis,0,sizeof(vis));
        edge ee(-1,0,-1);
        dfs2(x,g,contracted_g,m_out,m_in, i,x,ee);
    }
    return contracted_g;
}

  
void mssp(vector<vector<edge>> &g,int index){
    int n=g.size();
    delta[index*2].resize(n);
    delta[index*2+1].resize(n);
    s[index*2].resize(n);
    s[index*2+1].resize(n);
    for(int x=0;x<3;x++){
        dist[index][x].resize(n);
        par[index][x].resize(n);
    }
    int i1=0;
    int i2=roots[index].size()-1;
    int i=(i1+i2)/2;

    dijkstra(roots[index][i1],index,0,g);
    dijkstra(roots[index][i],index,1,g);
    dijkstra(roots[index][i2],index,2,g);

    if(i2-i1<=1) return;
    for(int x=0;x<n;x++){
        s[index*2][x]=s[index*2+1][x]=x;
        delta[index*2][x]=delta[index*2+1][x]=0;
    }

    
    vector<vector<edge>> h[2];
    h[0].resize(n);
    h[1].resize(n);
    for(int x=0;x<n;x++){
        for(auto it: g[x]){
            auto c=it.v;
            int mn=min(roots_to_index[index][x],roots_to_index[index][c]);
            int mx=max(roots_to_index[index][x],roots_to_index[index][c]);
            if(mx==-1 || (mx==i && mn==-1)){
                h[0][x].push_back(it);
                h[1][x].push_back(it);
            }
            else if(mx==i){
                if(mn<=i) h[0][x].push_back(it);
                else h[1][x].push_back(it);
            }
            else if(mx<i){
                if(mn==-1 || mn<=i)h[0][x].push_back(it);
            }
            else{
                if(mn==-1 || mn>i)h[1][x].push_back(it);
            }
            
        }
    }

    for(int j=0;j<2;j++){
        
        vector<vector<int>> common_edges;
        common_edges.resize(n);
        vector<int> sources;
        for(int x=0;x<n;x++){
            if(par[index][j][x]==par[index][j+1][x] && par[index][j][x]!=-1){
                common_edges[par[index][j][x]].push_back(x);
            }
        }
      
        for(int x=0;x<n;x++){
            if(par[index][j][x]!=-1 && par[index][j][x]!=par[index][j+1][x] && common_edges[x].size()){
                sources.push_back(x);
            }
        }
        vector<int> final_sources;
        for(auto it :sources){
            int p1=par[index][j][it]; int p2=par[index][j+1][it];
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
                final_sources.push_back(it);
                continue;
            }

            poss=1;
            check=0;
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
                if(it2.dir==1 && in_ts[it2.v] && check){
                    poss=0; break;
                }
            }
            if(poss){
                final_sources.push_back(it);
            } 
        }
        if(final_sources.size()){
            if(!j)h[0]=contract(h[0],common_edges,index*2,final_sources);
            else h[1]=contract(h[1],common_edges,index*2+1,final_sources);
        }
        vector<vector<edge>> hhh;
        int counter=0;
        mapping[index*2+j].resize(n);
        for(int x=0;x<h[j].size();x++){
            if(h[j][x].empty()) continue;
            mapping[index*2+j][x]=counter++;
        }
        hhh.resize(counter);
        for(int x=0;x<h[j].size();x++){
            if(h[j][x].empty()) continue;
            swap(hhh[mapping[index*2+j][x]],h[j][x]);
            for(auto &y:hhh[mapping[index*2+j][x]]){
                y.v=mapping[index*2+j][y.v];
            }
        }

        roots_to_index[index*2+j].assign(h[j].size(),-1);
        if(j==0){
            roots[index*2].resize(i+1);
            int k=0;
            for(int jj=i1;jj<=i;jj++){
                roots_to_index[index*2][mapping[index*2][roots[index][jj]]]=k;
                roots[index*2][k++]=mapping[index*2][roots[index][jj]];

            }
            mssp(hhh,index*2);
        }
        else{
            roots[index*2+1].resize(i2-i+1);
            int k=0;
            for(int jj=i;jj<=i2;jj++){
                 roots_to_index[index*2+1][mapping[index*2+1][roots[index][jj]]]=k;
                roots[index*2+1][k++]=mapping[index*2+1][roots[index][jj]];
            }
            mssp(hhh,index*2+1);
        }
    }

}

int query(int source,int target,int i1,int i2,int index){
    int i=(i1+i2)/2;
    if(source==i1) return dist[index][0][target];
    if(source==i2) return dist[index][2][target];
    if(source==i) return dist[index][1][target];
    if(source<=i) return query(source,mapping[index*2][s[index*2][target]],i1,i,index*2)+delta[index*2][target];
    else return query(source,mapping[index*2+1][s[index*2+1][target]],i,i2,index*2+1)+delta[index*2+1][target];
}

void init(){
  p1[0]=p2[0]=1;
  for(int i=1;i<N;i++){
    p1[i]=p1[i-1]*prime1%mod;
    p2[i]=p2[i-1]*prime2%mod;
  }
}

long double ran(int to,int from,int dir){
  if(dir){
    return (p1[from]*p2[to]%mod)/1e14;
  }
  return (p2[from]*p1[to]%mod)/1e14;
}

int main(){
    // freopen("embeddingdual.txt", "r", stdin);
    init();


    int n;cin>>n;
    int fn;cin>>fn;
    roots[1].resize(fn);
    roots_to_index[1].assign(n,-1);
    int temp_index=0;
    for(auto &i:roots[1]){
        cin>>i;
        roots_to_index[1][i]=temp_index++;
    }
    vector<int> roots_original=roots[1];
    // int roots_to_index[1][i]=n;
    int n_original=n;
    n+=fn;
    vector<vector<edge>> g(n);
    mapping[1].resize(n);
    s[1].resize(n);
    delta[1].resize(n);
    for(int i=0;i<n_original;i++){
        mapping[1][i]=i;
        int m;cin>>m;
        bool run=1;
        if(roots_to_index[1][i]==-1)run=0;
        for(int j=0;j<m;j++){
            int v,dir;
            ld wt;

            cin>>v>>dir>>wt;
            wt+=ran(i,v,dir);
            int prev_ind=(roots_to_index[1][i]-1+roots_to_index[1].size())%roots[1].size();
          int next_ind=(roots_to_index[1][i]+1)%roots[1].size();
            if(run && g[i].size()){
              if(g[i].back().v==roots_to_index[1][prev_ind] && v==roots_to_index[1][next_ind]){
                run=0;
                g[i].push_back(edge(roots_to_index[1][i]+n_original,0,0));
                g[roots_to_index[1][i]+n_original].push_back(edge(i,1,0));
                g[i].push_back(edge(v,dir,wt));
              }
              else{
                g[i].push_back(edge(v,dir,wt));
              }
          }
          else{
            g[i].push_back(edge(v,dir,wt));
          }
        }
        if(run){
          g[i].push_back(edge(roots_to_index[1][i]+n_original,0,0));
        g[roots_to_index[1][i]+n_original].push_back(edge(i,1,0));
        }
    }

    // // for(int i=0;i<n;i++){
    // //  cout<<i<<" ";8
    // //  cout<<g[i].size()<<"\n";
    // //  for(auto j:g[i]){
    // //    cout<<j.v<<" "<<j.dir<<" "<<j.wt<<"\n";
    // //  }
    // // }

    roots_to_index[1].assign(n,-1);
    for(int i=0;i<roots[1].size();i++){
      roots[1][i]=n_original+i;
      roots_to_index[1][n_original+i]=i;
    }
    mssp(g,1);
    for(int i=0;i<fn;i++){
      cout<<roots_original[i]<<"\n";
      for(int j=0;j<n_original;j++){
        cout<<j<<" "<<query(i,j,0,fn-1,1)<<"\n";
      }
      cout<<"\n";
    }
    // // for(int i=0;i<=9;i++){
    // //     cout<<query(i,6,0,fn-1,1)<<"\n";
    // //     cout<<query(i,7,0,fn-1,1)<<"\n";
    // //     cout<<query(i,8,0,fn-1,1)<<"\n";
    // //     cout<<query(i,9,0,fn-1,1)<<"\n";
    // //     cout<<query(i,10,0,fn-1,1)<<"\n";
    // //     cout<<"\n\n";
    // // }


    cout<<"hello1\n";
}