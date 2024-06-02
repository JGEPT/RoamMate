#include "dijkstra_algorithm.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

DijkstraAlgorithm::DijkstraAlgorithm() {
    loadData();
}

void DijkstraAlgorithm::loadData() {
    loadPoints();
    loadCostMatrix();
}

void DijkstraAlgorithm::loadPoints() {
    QFile file(":/resources/files/mapping.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open mapping.csv";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() >= 2) {
            int key = parts[0].toInt();
            QString value = parts[1];
            point.insert(key, value);
        }
    }
    file.close();
}

void DijkstraAlgorithm::loadCostMatrix() {
    QFile file(":/resources/files/MatrixCoordinateDistance.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open MatrixCoordinateDistance.csv";
        return;
    }

    QTextStream in(&file);
    int row = 0;
    while (!in.atEnd() && row < V) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        for (int col = 0; col < parts.size() && col < V; ++col) {
            cost[row][col] = parts[col].toFloat();
        }
        ++row;
    }
    file.close();
}

QVector<QString> DijkstraAlgorithm::findShortestPath(int src, int dest) {
    init(src);
    dijkstra();
    return getPath(src, dest);
}

void DijkstraAlgorithm::init(int src) {
    for (int i = 0; i < V; ++i) {
        parent[i] = i;
        dist[i] = INF;
    }
    dist[src] = 0;
}

int DijkstraAlgorithm::getNearest() {
    float minvalue = INF;
    int minnode = 0;
    for (int i = 0; i < V; ++i) {
        if (!visited[i] && dist[i] < minvalue) {
            minvalue = dist[i];
            minnode = i;
        }
    }
    return minnode;
}

void DijkstraAlgorithm::dijkstra() {
    for (int i = 0; i < V; ++i) {
        int nearest = getNearest();
        visited[nearest] = true;
        for (int adj = 0; adj < V; ++adj) {
            if (cost[nearest][adj] != INF && dist[adj] > dist[nearest] + cost[nearest][adj]) {
                dist[adj] = dist[nearest] + cost[nearest][adj];
                parent[adj] = nearest;
            }
        }
    }
}

QVector<QString> DijkstraAlgorithm::getPath(int src, int dest) {
    QVector<QString> path;
    qDebug() << dest;
    for (int i = 0; i < V; i++) {
        if (i == dest) {
            path.push_front(point[i]);
            int parnode = parent[i];
            while (parnode != src) {
                path.push_front(point[parnode]);
                parnode = parent[parnode];
            }
            path.push_front(point[src]);
        }
    }
    return path;
}

QString DijkstraAlgorithm::getDistance(int dest) {
    return QString::number(dist[dest]);
}
