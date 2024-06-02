#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <stdlib.h>

using namespace std;

#define INF 999
#define V 37

void init(int *parent, float *dist, int src) {
    for (int i = 0; i < V; i++) {
        parent[i] = i;
        dist[i] = INF;
    }
    dist[src] = 0;
}

int getNearest(bool *visited, float *dist) {
    float minvalue = INF;
    int minnode = 0;
    for (int i = 0; i < V; i++) {
        if (!visited[i] && dist[i] < minvalue) {
            minvalue = dist[i];
            minnode = i;
        }
    }
    return minnode;
}

void dijkstra(bool *visited, float **cost, float *dist, int *parent) {
    for (int i = 0; i < V; i++) {
        int nearest = getNearest(visited, dist);
        visited[nearest] = true;
        for (int adj = 0; adj < V; adj++) {
            if (cost[nearest][adj] != INF && dist[adj]>dist[nearest]+cost[nearest][adj]) {
                dist[adj] = dist[nearest] + cost[nearest][adj];
                parent[adj] = nearest;
            }
        }
    }
}

void display(float *dist, int *parent, int src,  map <int, string> point, int dest) {
    vector<string> path;
    cout<<"Source:\t\t\tCost:\t\t\t Path:";
    cout<<endl;
    for (int i = 0; i < V; i++) {
        if(i == dest) {
            cout<<i + 1<<"\t\t\t"<< dist[i] << " meter/s" <<"\t\t"<<" ";
            path.insert(path.begin(), point.at(i));
            int parnode = parent[i];
            while(parnode != src) {
                path.insert(path.begin(), point.at(parnode));
                parnode = parent[parnode];
            }
            path.insert(path.begin(), point.at(src));
        }
    }

    for (int j = 0; j < path.size(); j++) {
        if (j > 0) {
            cout<<" ";
        }
        cout<<path[j];
        if (j != path.size() - 1) {
            cout<<" -->";
        }
    }
}

int main(void) {
    int src, dest;

    // dynamically allocate arrays
    
    float *dist = (float*)malloc(V*sizeof(float));
    int *parent = (int*)malloc(V*sizeof(int));
    bool *visited = (bool*)calloc(V, sizeof(bool));
    float **cost = (float**)malloc(V * sizeof(float *));
    for (int i = 0; i < V; i++) {
        cost[i] = (float*)malloc(V * sizeof(float));
    }

    map <int, string> point;

    ifstream iFile("mapping.csv");
    if (!iFile.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    string line = "", key = "";
    int location;
    while (getline(iFile, line)) {
        location = line.find(',');
        key = line.substr(0, location);
        line = line.substr(location + 1, line.length());
        point.insert({stof(key), line});

    }
    iFile.close();

    ifstream inputFile("MatrixCoordinateDistance.csv");
    if (!inputFile.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
    }

    line = "";
    int i = 0;
    while (getline(inputFile, line)) {
        stringstream inputString(line);
        string tempString;
        int j = 0;
        while (getline(inputString, tempString, ',') && j < V) {
            cost[i][j] = atof(tempString.c_str());
            j++;
        }
        i++;
        if (i >= V) break; 
    }
    inputFile.close();

    cout<<"Current location: ";
    cin>>src;
    cout<<"Destination: ";
    cin>>dest;
    src--;
    dest--;
    init(parent, dist, src);
    dijkstra(visited, cost, dist, parent); 
    display(dist, parent, src, point, dest);

    free(dist);
    free(parent);
    free(visited);
    free(cost);
    return 0;
}