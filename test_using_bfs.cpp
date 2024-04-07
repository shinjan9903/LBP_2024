#include<bits/stdc++.h>
#define ld long double
using namespace std;
const int N=1000009;



bool visited[N];
vector<int> graph[N];
int value[N];


void  bfs(int r){
   queue<int> q;
   q.push(r);
   visited[r]=true;
   while(!q.empty()){
      for(auto child: graph[q.front()]){
        if(visited[child]) continue;
         q.push(child);
         visited[child]=true;
         value[child]=value[q.front()]+1;
      }
      q.pop();
   }
}


int main(){
   int n,fn; cin>>n>>fn;
   vector<int> finf(fn);
   for(int i=0;i<fn;i++){
       cin>>finf[i];
   }
   for(int i=0;i<n;i++){
      int m; cin>>m;
      while(m--){
         int y,dir,wt;cin>>y>>dir>>wt;
         graph[i].push_back(y);
      }
   }

   for(int i=0;i<fn;i++){
      cout<<finf[i]<<"\n";
      for(int j=0;j<n;j++){
         value[j]=1e9;
      }
      value[finf[i]]=0;
      bfs(finf[i]);
      for(int j=0;j<n;j++){
         cout<<j<<" "<<value[j]<<"\n";
      }
   }


}
