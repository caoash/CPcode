/*
	As a disclaimer my solution only get 5/15 test cases, I am not sure why though, I assume it is a minor error. The first
thing you should think about with this problem is, given the side of B A is currently on, what other sides can A get to. This is
because figuring out what other sides it can get to determines the directions B can go. If you consider the grid as a graph, this
problem immediatley becomes a question of biconnected components, as you are considering if you can get to node B even if you get
rid of the edge connecting A to B from the side it is currently on. To do this, you use the standard algorithm for finding
articulation points by performing a dfs and testing whether a subtree connects back to one of its ancestors, and keep the nodes
currently in the component on a stack and set them as a biconnected component when you find an articulation point. Also, I did
not just keep which parts of the grid are the same connected component but also kept which edges between the graph are biconnected.
This then allows you to see if you are in one position next to a box, which other positions you can get to. After finding the
biconnected components, one just has to form a bfs from box B and transition the pushes moving B and moving to different sides of
B. Lastly, one can answer the queries just by seeing if the bfs marked the part of the grid as accessible. The complexity of this
is just O(n) because you are just doing a dfs and bfs.
*/

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <stack>
#include <queue>
using namespace std;
#define endl '\n'

const int maxn = 1500;
int n, m, q;
pair<int, int> a, b;
char grid[maxn][maxn];
bool dp[maxn][maxn][4];
bool visited[maxn][maxn];

int s = 1, t;
int disc[maxn][maxn], low[maxn][maxn];
pair<int, int> par[maxn][maxn];
int bicon[maxn][maxn][4];
stack<pair<pair<int, int>, int>> stk;
queue<pair<pair<int, int>, int>> qd;

int dx[4] = {1, 0, -1, 0};
int dy[4] = {0, 1, 0, -1};

bool works(int x, int y){
	if(x < 0 || x >= n || y < 0 || y >= m) return 0;
	if(grid[x][y] == '#') return 0;
	return 1;
}

void setbicon(pair<pair<int, int>, int> cp){
	pair<pair<int, int>, int> p = {{-2, -2}, -2};
	
	while(!stk.empty() && p != cp){
		p = stk.top();
		stk.pop();
		int x = p.first.first, y = p.first.second, i = p.second;
		
		bicon[x][y][i] = s;
		bicon[x + dx[i]][y + dy[i]][(i + 2) % 4] = s; 
	}
	
	s++;
}

void dfs(int x, int y){
	disc[x][y] = low[x][y] = ++t;
	int c = 0;
	
	for(int i = 0; i < 4; i++){
		int nx = x + dx[i], ny = y + dy[i];
		if(!works(nx, ny)) continue;
		
		if(!disc[nx][ny]){
			c++;
			par[nx][ny] = {x, y};
			stk.push({{x, y}, i});

			if(b == make_pair(nx, ny)) qd.push({b, (i + 2) % 4});
			
			dfs(nx, ny);
			low[x][y] = min(low[x][y], low[nx][ny]);
			
			if(par[x][y] == make_pair(-1, -1) && c > 1) setbicon({{x, y}, i});
			if(par[x][y] != make_pair(nx, ny) && low[nx][ny] >= disc[x][y]) setbicon({{x, y}, i});
		}else if(par[x][y] != make_pair(nx, ny) && disc[nx][ny] < low[x][y]){
			low[x][y] = disc[nx][ny];
			stk.push({{x, y}, i});
		}
	}
}

void print(){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			for(int k = 0; k < 4; k++) cout << bicon[i][j][k] << " ";
			cout << "| ";
		}
		cout << endl;
		cout << "--------------------------------------------------------" << endl;
	}
}

void solve(){
	while(!qd.empty()){
		int x = qd.front().first.first, y = qd.front().first.second, d = qd.front().second;
		qd.pop();
		
		int nx = x - dx[d], ny = y - dy[d];
		if(works(nx, ny)){
			qd.push({{nx, ny}, d});
			dp[nx][ny][d] = 1;
		} 
		
		for(int i = 0; i < 4; i++){
			if(dp[x][y][i]) continue;
			if(bicon[x][y][i] != bicon[x][y][d]) continue;
			qd.push({{x, y}, i});
			dp[x][y][i] = 1;
		}
	}

	dp[b.first][b.second][0] = 1;
}

void answer(){
	int x, y;
	cin >> x >> y;
	x--, y--;
	
	for(int i = 0; i < 4; i++){
		if(!dp[x][y][i]) continue;
		
		cout << "YES" << endl;
		return;
	}
	
	cout << "NO" << endl;
}

int main(){
	freopen("pushabox.in", "r", stdin);
	freopen("pushabox.out", "w", stdout);
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	
	cin >> n >> m >> q;
	
	for(int i = 0; i < n; i++){
		cin >> grid[i];
		for(int j = 0; j < m; j++){
			if(grid[i][j] == 'A'){
				a = {i, j};
				grid[i][j] = '.';
			}else if(grid[i][j] == 'B'){
				b = {i, j};
				grid[i][j] = '.';
			}
		}
	}
	
	par[a.first][a.second] = {-1, -1};
	dfs(a.first, a.second);
	setbicon({{-1, -1}, -1});
	
	solve();
	
	for(int i = 0; i < q; i++) answer();

	return 0;
}
