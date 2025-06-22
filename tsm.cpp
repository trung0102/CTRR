#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;
string Traveling(int edgeList[][3], int numEdge, char start) {
    static const int N = 20;
    static const int BIG = 10000;
    int cmap[256];
    for (int i = 0; i < 256; ++i) cmap[i] = -1;
    vector<char> nodeList;
    nodeList.reserve(N);

    for (int i = 0; i < numEdge; ++i) {
        unsigned char ua = static_cast<unsigned char>(edgeList[i][0]);
        unsigned char va = static_cast<unsigned char>(edgeList[i][1]);
        if (cmap[ua] == -1) {
            if ((int)nodeList.size() >= N) return "";
            cmap[ua] = (int)nodeList.size();
            nodeList.push_back(static_cast<char>(ua));
        }
        if (cmap[va] == -1) {
            if ((int)nodeList.size() >= N) return "";
            cmap[va] = (int)nodeList.size();
            nodeList.push_back(static_cast<char>(va));
        }
    }
    int total = (int)nodeList.size();
    if (total == 0) return "";
    unsigned char us = static_cast<unsigned char>(start);
    if (cmap[us] == -1) return "";
    int root = cmap[us];
    if (root != 0) {
        char tmp = nodeList[0];
        nodeList[0] = nodeList[root];
        nodeList[root] = tmp;
        cmap[(unsigned char)nodeList[0]] = 0;
        cmap[(unsigned char)nodeList[root]] = root;
    }
    vector<vector<int>> dist(total, vector<int>(total, BIG));
    for (int i = 0; i < total; ++i) {
        dist[i][i] = 0;
    }
    for (int i = 0; i < numEdge; ++i) {
        unsigned char ua = static_cast<unsigned char>(edgeList[i][0]);
        unsigned char va = static_cast<unsigned char>(edgeList[i][1]);
        int w = edgeList[i][2];
        int u = cmap[ua];
        int v = cmap[va];
        if (u >= 0 && u < total && v >= 0 && v < total) {
            dist[u][v] = min(dist[u][v], w);
        }
    }
    int bitCount = total - 1; 
    if (bitCount < 0) return "";
    int fullMask = (bitCount > 0 ? (1 << bitCount) - 1 : 0);
    int maskCount = (bitCount > 0 ? (1 << bitCount) : 1);
    vector<vector<int>> memo(maskCount, vector<int>(total, BIG));
    vector<vector<int>> trace(maskCount, vector<int>(total, -1));
    if (total == 1) {
        string path;
        path += nodeList[0];
        path += ' ';
        path += nodeList[0];
        return path;
    }
    for (int j = 1; j < total; ++j) {
        int flag = 1 << (j - 1);
        if (dist[0][j] < BIG) {
            memo[flag][j] = dist[0][j];
            trace[flag][j] = 0;
        }
    }
    for (int mask = 1; mask <= fullMask; ++mask) {
        for (int j = 1; j < total; ++j) {
            int jb = 1 << (j - 1);
            if (!(mask & jb)) continue; 
            int prevMask = mask ^ jb;
            if (prevMask == 0) continue;
            for (int k = 1; k < total; ++k) {
                int kb = 1 << (k - 1);
                if (!(prevMask & kb)) continue;
                int prevCost = memo[prevMask][k];
                if (prevCost >= BIG) continue;
                int alt = prevCost + dist[k][j];
                if (alt < memo[mask][j]) {
                    memo[mask][j] = alt;
                    trace[mask][j] = k;
                }
            }
        }
    }
    int bestCost = BIG;
    int bestTail = -1;
    for (int j = 1; j < total; ++j) {
        int costJ = memo[fullMask][j];
        if (costJ >= BIG) continue;
        if (dist[j][0] >= BIG) continue;
        int totalCost = costJ + dist[j][0];
        if (totalCost < bestCost) {
            bestCost = totalCost;
            bestTail = j;
        }
    }
    if (bestTail < 0) {
        return "";
    }
    vector<int> seq;
    seq.reserve(total + 1);
    int cur = bestTail;
    int mask = fullMask;
    while (cur != 0) {
        seq.push_back(cur);
        int prev = trace[mask][cur];
        mask ^= (1 << (cur - 1));
        cur = prev;
    }
    seq.push_back(0);
    reverse(seq.begin(), seq.end());
    string path;
    bool first = true;
    for (int idx : seq) {
        if (!first) path += ' ';
        path += nodeList[idx];
        first = false;
    }
    path += ' ';
    path += nodeList[0];

    return path;
}