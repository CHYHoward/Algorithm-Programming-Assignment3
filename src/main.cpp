#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
int n = 0;
struct Undirected_Node
{
    int rank;
    int parent;
};
struct Undirected_Edge
{
    int u;
    int v;
    int w;
};

struct Directed_Node
{
    int rank;
    int parent;
    int color;
    vector<int> pointer_target;
    int pointer_num;
};
struct Directed_Edge
{
    int u;
    int v;
    int w;
};

bool Undirected_edge_weight_decrease(Undirected_Edge e1, Undirected_Edge e2);
void Undirected_Make_Set(Undirected_Node vertex_list[], int vertex_num);
int Undirected_Find_Set(Undirected_Node vertex_list[], int vertex_num);
void Undirected_Union(Undirected_Node vertex_list[], int v1, int v2);
void undirected_alg(istream &fin, ostream &fout);

bool Directed_edge_weight_decrease(Directed_Edge e1, Directed_Edge e2);
void Directed_Make_Set(Directed_Node vertex_list[], int vertex_num);
int Directed_Find_Set(Directed_Node vertex_list[], int vertex_num);
void Directed_Union(Directed_Node vertex_list[], int v1, int v2);
void directed_alg(istream &fin, ostream &fout);
bool cyclechecker(Directed_Edge temp_edge, vector<int> *adjacency_list);
bool DFS(int visiting_vertex, vector<int> &state, vector<int> *temp_adjacency_list);
//////------undircted------/////
bool Undirected_edge_weight_decrease(Undirected_Edge e1, Undirected_Edge e2)
{
    if (e1.w > e2.w)
    {
        return true;
    }
    return false;
}

void Undirected_Make_Set(Undirected_Node vertex_list[], int vertex_num)
{
    vertex_list[vertex_num].parent = vertex_num;
    vertex_list[vertex_num].rank = 0;
}

int Undirected_Find_Set(Undirected_Node vertex_list[], int vertex_num)
{
    if (vertex_list[vertex_num].parent == vertex_num) // root
        return vertex_num;
    // Path Compression
    vertex_list[vertex_num].parent = Undirected_Find_Set(vertex_list, vertex_list[vertex_num].parent);
    return vertex_list[vertex_num].parent;
}

void Undirected_Union(Undirected_Node vertex_list[], int v1, int v2)
{
    // Union by rank
    v1 = Undirected_Find_Set(vertex_list, v1);
    v2 = Undirected_Find_Set(vertex_list, v2);
    if (vertex_list[v1].rank == vertex_list[v2].rank)
    {
        vertex_list[v1].parent = v2;
        vertex_list[v2].rank += 1;
    }
    else
    {
        if (vertex_list[v1].rank > vertex_list[v2].rank)
            vertex_list[v2].parent = v1;
        else if (vertex_list[v1].rank < vertex_list[v2].rank)
            vertex_list[v1].parent = v2;
    }
    return;
}

void undirected_alg(istream &fin, ostream &fout)
{

    fin >> n;
    Undirected_Node *node = new Undirected_Node[n];
    for (int i = 0; i < n; i++)
        Undirected_Make_Set(node, i);

    int m;
    fin >> m;
    vector<Undirected_Edge> edge;
    edge.resize(m);
    for (int i = 0; i < m; i++)
        fin >> edge.at(i).u >> edge.at(i).v >> edge.at(i).w;

    sort(edge.begin(), edge.end(), Undirected_edge_weight_decrease);
    // Mergesort
    vector<Undirected_Edge> removed_edge;
    int removed_sum = 0;
    int removed_num = 0;
    // Using Kruskal's MST Algorithm, but greater one comes first
    for (int i = 0; i < m; i++)
    {
        if (Undirected_Find_Set(node, edge.at(i).u) != Undirected_Find_Set(node, edge.at(i).v))
            Undirected_Union(node, edge.at(i).u, edge.at(i).v);
        else
        {
            removed_edge.push_back(edge.at(i));
            removed_sum = removed_sum + edge.at(i).w;
            removed_num++;
        }
    }
    fout << removed_sum << endl;
    for (int i = 0; i < removed_num; i++)
    {
        fout << removed_edge.at(i).u << " " << removed_edge.at(i).v << " " << removed_edge.at(i).w << endl;
    }
}

/////////-------directed---------////

bool Directed_edge_weight_decrease(Directed_Edge e1, Directed_Edge e2)
{
    // if(e1.w >e2.w){
    //     return true;
    // }
    // return false;
    return e1.w > e2.w;
}

void Directed_Make_Set(Directed_Node vertex_list[], int vertex_num)
{
    vertex_list[vertex_num].parent = vertex_num;
    vertex_list[vertex_num].rank = 0;
}

int Directed_Find_Set(Directed_Node vertex_list[], int vertex_num)
{
    if (vertex_list[vertex_num].parent == vertex_num) // the root
        return vertex_num;
    // Path Compression
    vertex_list[vertex_num].parent = Directed_Find_Set(vertex_list, vertex_list[vertex_num].parent);
    return vertex_list[vertex_num].parent;
}

void Directed_Union(Directed_Node vertex_list[], int v1, int v2)
{
    // union by rank
    v1 = Directed_Find_Set(vertex_list, v1);
    v2 = Directed_Find_Set(vertex_list, v2);
    if (vertex_list[v1].rank == vertex_list[v2].rank)
    {
        vertex_list[v1].parent = v2;
        vertex_list[v2].rank += 1;
    }
    else
    {
        if (vertex_list[v1].rank > vertex_list[v2].rank)
            vertex_list[v2].parent = v1;
        else if (vertex_list[v1].rank < vertex_list[v2].rank)
            vertex_list[v1].parent = v2;
    }
    return;
}
bool cyclechecker(Directed_Edge temp_edge, vector<int> *adjacency_list)
{
    vector<int> *temp_adjacency_list = new vector<int>[n];
    for (int i = 0; i < n; i++)
    {
        temp_adjacency_list[i] = adjacency_list[i];
    }
    temp_adjacency_list[temp_edge.u].push_back(temp_edge.v);
    vector<int> state(n, 0); // 0=WHITE
    for (int visiting_vertex = 0; visiting_vertex < n; ++visiting_vertex)
    {
        if (state[visiting_vertex] == 0)
        {
            if (DFS(visiting_vertex, state, temp_adjacency_list) == true)
            {
                return true;
            }
        }
    }
    delete[] temp_adjacency_list;
    return false;
}
bool DFS(int visiting_vertex, vector<int> &state, vector<int> *temp_adjacency_list)
{
    state[visiting_vertex] = 1; // 1=grey
    for (int i = 0; i < temp_adjacency_list[visiting_vertex].size(); i++)
    {
        if (state[temp_adjacency_list[visiting_vertex][i]] == 1)
        {
            return true;
        }
        else if (state[temp_adjacency_list[visiting_vertex][i]] == 0)
        {
            if (DFS(temp_adjacency_list[visiting_vertex][i], state, temp_adjacency_list))
            {
                return true;
            }
        }
    }
    state[visiting_vertex] = 2; // 2=black
    return false;
}

void directed_alg(istream &fin, ostream &fout)
{
    fin >> n;
    Directed_Node *node = new Directed_Node[n];
    for (int i = 0; i < n; i++)
    {
        node[i].pointer_num = 0;
    }
    for (int i = 0; i < n; i++)
        Directed_Make_Set(node, i);

    int m;
    fin >> m;
    vector<Directed_Edge> edge;

    edge.resize(m);
    for (int i = 0; i < m; i++)
    {
        fin >> edge.at(i).u >> edge.at(i).v >> edge.at(i).w;
        node[edge.at(i).u].pointer_target.push_back(edge.at(i).v);
        node[edge.at(i).u].pointer_num++;
    }

    sort(edge.begin(), edge.end(), Directed_edge_weight_decrease);

    vector<Directed_Edge> removed_edge;
    vector<Directed_Edge> remained_edge;
    int removed_sum = 0;
    int removed_num = 0;

    // Using Kruskal's MST Algorithm, but greater one comes first
    for (int i = 0; i < m; i++)
    {
        if (Directed_Find_Set(node, edge.at(i).u) != Directed_Find_Set(node, edge.at(i).v))
        {
            Directed_Union(node, edge.at(i).u, edge.at(i).v);
            remained_edge.push_back(edge.at(i));
        }
        else
        {
            removed_edge.push_back(edge.at(i));
            removed_sum = removed_sum + edge.at(i).w;
            removed_num++;
        }
    }

    vector<int> *adjacency_list = new vector<int>[n];
    for (int i = 0; i < remained_edge.size(); i++)
    {
        adjacency_list[remained_edge.at(i).u].push_back(remained_edge.at(i).v);
    }
    vector<Directed_Edge> ans;
    int ansWeight = 0;
    // cout << removed_edge.size() << "\n";
    for (int i = 0; i < removed_edge.size(); i++)
    {
        // cout << removed_edge[i].u << "\t" << removed_edge[i].v << "\t" << removed_edge[i].w << "\n";
        if (removed_edge[i].w < 0)
        {
            // cout << "negative\n";
            ans.push_back(removed_edge[i]);
            ansWeight += removed_edge[i].w;
        }
        else
        {
            // cout << "positive\n";
            if (cyclechecker(removed_edge[i], adjacency_list))
            {
                // cout << "flag1\n";
                ans.push_back(removed_edge[i]);
                ansWeight += removed_edge[i].w;
            }
            else
            {
                // cout << "flag2\n";
                adjacency_list[removed_edge[i].u].push_back(removed_edge[i].v);
            }
        }
    }

    fout << ansWeight << endl;
    for (int i = 0; i < ans.size(); i++)
        fout << ans.at(i).u << " " << ans.at(i).v << " " << ans.at(i).w << endl;
    return;
}

int main(int argc, char *argv[])
{
    ifstream fin;
    ofstream fout;
    fin.open(argv[1]);
    fout.open(argv[2]);

    char x;
    fin >> x;

    if (x == 'u')
    {
        undirected_alg(fin, fout);
    }
    else if (x == 'd')
    {
        directed_alg(fin, fout);
    }

    fin.close();
    fout.close();
    return 0;
}
