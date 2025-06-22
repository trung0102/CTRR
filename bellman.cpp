#include "bellman.h"

int FindIndex(const vector<int>& sortedVec, int target){
    auto it = lower_bound(sortedVec.begin(), sortedVec.end(), target);
    if(it != sortedVec.end() && *it == target){
        return it - sortedVec.begin();
    }
    else return -1;  
}

vector<int> CreateVec(int edgeList[][3], int numEdges){
    vector<int> ret;
    for(int i=0; i<numEdges; ++i){
        int ver1 = edgeList[i][0];
        int ver2 = edgeList[i][1];
        if(find(ret.begin(),ret.end(),ver1) == ret.end()){
            ret.push_back(ver1);
        }
        if(find(ret.begin(),ret.end(),ver2) == ret.end()){
            ret.push_back(ver2);
        }
    }
    sort(ret.begin(),ret.end());
    return ret;
}
bool is_sp = true;
void BF(int edgeList[][3], int numEdges, char start_vertices, int BFValue[], int BFPrev[])
{
    vector<int> vertices = CreateVec(edgeList, numEdges);
    int num_vertices = vertices.size();
    int s = FindIndex(vertices, start_vertices);
    if (s == -1) return;
    if (BFValue[s] == -1)
    {
        BFValue[s] = 0;
        BFPrev[s] = -1;
    }
    int old_val[1000];
    for (int i = 0; i < num_vertices; i++)
    {
        old_val[i] = BFValue[i];
    }
    for (int i = 0; i < numEdges; i++)
    {
        for (int j = i + 1; j < numEdges; j++)
        {
            if ((edgeList[j][0] < edgeList[i][0]) ||
                (edgeList[j][0] == edgeList[i][0] && edgeList[j][1] < edgeList[i][1]))
            {
                for (int k = 0; k < 3; k++)
                {
                    int temp = edgeList[i][k];
                    edgeList[i][k] = edgeList[j][k];
                    edgeList[j][k] = temp;
                }
            }
        }
    }
    for (int i = 0; i < numEdges; i++)
    {
        int u = FindIndex(vertices, edgeList[i][0]);
        int v = FindIndex(vertices, edgeList[i][1]);
        int w = edgeList[i][2];

        if (u == -1 || v == -1 || old_val[u] == -1)
            continue;
        int newDist = old_val[u] + w;
        if (BFValue[v] == -1 || newDist < BFValue[v])
        {
            BFValue[v] = newDist;
            BFPrev[v] = u;
        }
    }
}

string BF_Path(int edges[][3], int numEdges, char start, char goal) {
    bool vertexExists[256] = {false};
    for (int i = 0; i < numEdges; ++i) {
        vertexExists[(unsigned char)edges[i][0]] = true;
        vertexExists[(unsigned char)edges[i][1]] = true;
    }
    int charToIndex[256];
    char indexToChar[256];  
    int vertexCount = 0;
    is_sp = false;
    for (int c = 0; c < 256; ++c) {
        if (vertexExists[c]) {
            charToIndex[c] = vertexCount;
            indexToChar[vertexCount] = static_cast<char>(c);
            vertexCount++;
        }
    }
    if (!vertexExists[(unsigned char)start] || !vertexExists[(unsigned char)goal]) {
        return "No path exists";
    }
    int startIndex = charToIndex[(unsigned char)start];
    int goalIndex = charToIndex[(unsigned char)goal];
    is_sp = true;
    for (int i = 0; i < numEdges; ++i) {
        for (int j = i + 1; j < numEdges; ++j) { 
            if ((edges[j][0] < edges[i][0]) || 
                (edges[j][0] == edges[i][0] && edges[j][1] < edges[i][1])) {
                for (int k = 0; k < 3; ++k) {
                    int temp = edges[i][k];
                    edges[i][k] = edges[j][k];
                    edges[j][k] = temp;
                }
            }
        }
    }
    vector<int> distance(vertexCount, -1);
    vector<int> predecessor(vertexCount, -1);
    distance[startIndex] = 0;
    is_sp = false;
    for (int iteration = 1; iteration < vertexCount; ++iteration) {
        vector<int> previousDistance = distance;
        for (int i = 0; i < numEdges; ++i) {
            int u = charToIndex[(unsigned char)edges[i][0]];
            int v = charToIndex[(unsigned char)edges[i][1]];
            int weight = edges[i][2];
            if (previousDistance[u] == -1) continue;
            int newDist = previousDistance[u] + weight;
            if (previousDistance[v] == -1 || newDist < distance[v]) {
                distance[v] = newDist;
                predecessor[v] = u;
            }
        }
    }
    if (distance[goalIndex] == -1) {
        return "No path exists";
    }
    is_sp = true;
    vector<char> path;
    int current = goalIndex;
    while (current != -1) {
        path.push_back(indexToChar[current]);
        current = predecessor[current];
    }
    if (path.back() != start) {
        return "No path exists";
    }
    string result;
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        if (!result.empty()) {
            result += ' ';
        }
        result += *it;
    }
    return result;
}