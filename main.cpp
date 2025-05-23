/*
 * Author: Carter Lewis
 * Assignment title: Program 6
 * Assignment Description: Christofides approximate TSP tour
 * Due date: 4-13-2025
 * Date created: 4-11-2025
 * Date last modified: 4-11-2025
 */

#include <iostream>
#include <queue>
#include <set>

using namespace std;

struct edge {
    int v1;
    int v2;
    double distance;

    edge(int a, int b, double c): v1(a), v2(b), distance(c) {}
};

struct node {
    int id;
    int size;
    vector<node*> next;

    node(int n): id(n), size(1) {}

    node* find(int n) {
        if(id == n) return this;
        node* t;
        for(int i = 0; i < next.size(); i++) {
            t = next[i]->find(n);
            if(t != nullptr) return t;
        }
        return nullptr;
    }
    void orderNodes(vector<int>& order) {
        if(this != nullptr) {
            order.push_back(id);
            for(int i = 0; i < next.size(); i++) {
                next[i]->orderNodes(order);
            }
        }
    }
};

struct comparator {
    bool operator()(const edge& e1, const edge& e2) const {
        return e1.distance > e2.distance;
    }
};

node* prim (vector<pair<double, double> > grid) {
    node* head;
    double adjacencyMatrix[grid.size()][grid.size()];
    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid.size(); j++) {
            adjacencyMatrix[i][j] = -1;
            adjacencyMatrix[i][j] = sqrt(pow(grid[i].first-grid[j].first, 2)
                    + pow(grid[i].second-grid[j].second, 2));
        }
    }

    priority_queue<edge, vector<edge>, comparator> edges;
    set<int> cluster;
    head = new node(0);
    cluster.insert(0);
    for(int i = 1; i < grid.size(); i++) {
        edges.emplace(0,i,adjacencyMatrix[0][i]);
    }

    while(head->size < grid.size()) {
        edge tmp = edges.top();
        edges.pop();
        if(tmp.v1 != tmp.v2) {
            if(cluster.find(tmp.v1) == cluster.end()) {
                node* insertHere = head->find(tmp.v2);
                insertHere->next.push_back(new node(tmp.v1));
                head->size++;
                cluster.insert(tmp.v1);
                for(int i = 0; 0 < grid.size(); i++) {
                    if(tmp.v1!=i && i!=tmp.v1 || cluster.find(i)==cluster.end()) {
                        edges.emplace(tmp.v1, i, adjacencyMatrix[tmp.v1][i]);
                    }
                }
            }
            if(cluster.find(tmp.v2) == cluster.end()) {
                node* insertHere = head->find(tmp.v1);
                insertHere->next.push_back(new node(tmp.v2));
                head->size++;
                cluster.insert(tmp.v2);
                for(int i = 0; i < grid.size(); i++) {
                    if(tmp.v2 != i && i!=tmp.v2 || cluster.find(i)==cluster.end()) {
                        edges.emplace(tmp.v2, i, adjacencyMatrix[tmp.v2][i]);
                    }
                }
            }
        }

    }

    return head;
}
double christofides(node* head, vector<pair<double, double> > grid,
                    vector<int>& order) {
    double total = 0;
    double adjacencyMatrix[grid.size()][grid.size()];
    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid.size(); j++) {
            adjacencyMatrix[i][j] = -1;
            adjacencyMatrix[i][j] = sqrt(pow(grid[i].first-grid[j].first, 2)
                    + pow(grid[i].second-grid[j].second, 2));
        }
    }
    head->orderNodes(order);
    order.push_back(head->id);

    for(int i = 0; i < order.size() - 1; i++) {
        total += adjacencyMatrix[order[i]][order[i+1]];
    }
    return total;
}

int main() {
    int n;
    double a, b;
    cin >> n;
    vector<pair<double, double> > grid;
    while(n-->0) {
        cin >> a >> b;
        grid.push_back({a, b});
    }

    node* head;
    head = prim(grid);
    vector<int> order;
    double ans = christofides(head, grid, order);

    cout << "Approximate TSP tour (Christofides):" << endl;
    for(int i: order) {
        cout << i << " ";
    }
    printf("\nTotal tour length: %.2f\n", ans);
    return 0;
}
