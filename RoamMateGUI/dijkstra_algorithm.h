#ifndef DIJKSTRA_ALGORITHM_H
#define DIJKSTRA_ALGORITHM_H

#include <QMap>
#include <QString>
#include <QVector>

class DijkstraAlgorithm {
public:
    DijkstraAlgorithm();
    QVector<QString> findShortestPath(int src, int dest);
    QString getDistance(int dest);

private:
    static const int V = 37;
    static const int INF = 999;
    int parent[V];
    float dist[V];
    bool visited[V] = { false };

    float cost[V][V];
    QMap<int, QString> point;

    void loadData();
    void loadPoints();
    void loadCostMatrix();
    void init( int src);
    int getNearest();
    void dijkstra();
    QVector<QString> getPath(int src, int dest);
};

#endif // DIJKSTRA_ALGORITHM_H
