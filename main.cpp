#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <map>

using namespace std;

/**************************************************************
 * Helper functions for accessing container of a node. Uncomment the cout lines to see finer detailed explanations
 * Outouts of the performance of constructing a container.
 **************************************************************/
void displayFunctionAndDuration(string s, double d) {
    // cout << s << " construction: " << d << endl;
}

//access the container in a meaningful way to estimate memory references. Tree is single dimensional array
void accessContainer(double *tree, int size) {
    double sum = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < size; i++) {
        sum += tree[i];
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    //cout << "time to access data: " << elapsed.count() << endl;
    //cout << sum << endl;
}

//access the container in a meaningful way to estimate memory references. Tree is double dimensional array
template<class T>
void accessContainer(T tree, int N, int B) {
    double sum = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < B; j++)
            sum += tree[i].x[j];
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    //cout << "time to access data: " << elapsed.count() << endl;
    //cout << sum << endl;
}

/**************************************************************
 * Containers of Nodes
 **************************************************************/
class Node {
public:
    double *x;

    Node(int size) {
        x = (double *) malloc(size * sizeof(double));
        for (int j = 0; j < size; j++) {
            x[j] = 2;
        }
    }
};


class NodeV {
public:
    vector<double> x;

    NodeV(int size) {
        x = vector<double>(size, 2);
    }
};

//class NodeArray {
//public:
//    double x[C];
//};

class NodeArray {
public:
    double *x;

    NodeArray(int len) {
        x = new double[len];
        for (int i = 0; i < len; i++)
            x[i] = 2;
    }
};

/**************************************************************
 * Functions to test the performance of constructing containers of nodes
 **************************************************************/
pair<double, string> testVectorVector(int N, int B) {
    auto start = std::chrono::high_resolution_clock::now();
    vector<NodeV> tree = vector<NodeV>(N, NodeV(B));
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    string s = __func__;
    double d = elapsed.count();
    displayFunctionAndDuration(s, d);
    accessContainer(tree, N, B);
    return make_pair(d, s);
}

pair<double, string> testVectorPointer(int N, int B) {
    auto start = std::chrono::high_resolution_clock::now();
    vector<Node> tree = vector<Node>(N, Node(B));
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    string s = __func__;
    double d = elapsed.count();
    displayFunctionAndDuration(s, d);
    accessContainer(tree, N, B);
    return make_pair(d, s);
}


pair<double, string> testPointerVector(int N, int B) {
    auto start = std::chrono::high_resolution_clock::now();
    NodeV *tree = (NodeV *) malloc(N * sizeof(NodeV));
    //same time
    for (int i = 0; i < N; i++) {
        void *place = &tree[i];
        NodeV *f = new(place) NodeV(B);
        // tree[i] = *f;
    }
//    for (int i = 0; i < N; i++)
//        for (int j = 0; j < B; j++)
//            tree[i].x.push_back(2);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    string s = __func__;
    double d = elapsed.count();
    displayFunctionAndDuration(s, d);
    accessContainer(tree, N, B);
    free(tree);
    tree = NULL;
    return make_pair(d, s);
}

pair<double, string> testPointerPointer(int N, int B) {
    auto start = std::chrono::high_resolution_clock::now();
    Node *tree = (Node *) malloc(N * sizeof(Node));
    for (int i = 0; i < N; i++) {
        tree[i].x = (double *) malloc(B * sizeof(double));
        for (int j = 0; j < B; j++)
            tree[i].x[j] = 2;
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    string s = __func__;
    double d = elapsed.count();
    displayFunctionAndDuration(s, d);
    accessContainer(tree, N, B);
    return make_pair(d, s);
}

pair<double, string> testVectorArray(int N, int B) {
    auto start = std::chrono::high_resolution_clock::now();
    vector<NodeArray> tree(N, NodeArray(B));
    //NodeArray *tree = (NodeArray *) malloc(N * sizeof(NodeArray));

//    for (int i = 0; i < N; i++) {
////        tree[i].x = (double *) malloc(B * sizeof(double));
//
//        for (int j = 0; j < B; j++)
//            tree[i].x[j] = 2;

    //}
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    string s = __func__;
    double d = elapsed.count();
    displayFunctionAndDuration(s, d);
    accessContainer(tree, N, B);
    return make_pair(d, s);
}

pair<double, string> testSingleArray(int N, int B) {
    int M = N * B;
    auto start = std::chrono::high_resolution_clock::now();
    double *tree = (double *) malloc(M * sizeof(double));
    for (int i = 0; i < M; i++) {
        tree[i] = 2;
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    string s = __func__;
    double d = elapsed.count();
    displayFunctionAndDuration(s, d);
    accessContainer(tree, M);
    return make_pair(d, s);
}

void testPerformance(vector<int> &nodeSizes, vector<int> &valuesSizes) {
    for (int i = 0; i < nodeSizes.size(); i++) {
        int N = nodeSizes[i];
        for (int j = 0; j < valuesSizes.size(); j++) {
            int B = valuesSizes[j];
            map<double, string> maps;

            maps.insert(testPointerPointer(N, B));
            maps.insert(testVectorPointer(N, B));
            maps.insert(testPointerVector(N, B));
            maps.insert(testVectorVector(N, B));
            maps.insert(testVectorArray(N, B));
            maps.insert(testSingleArray(N, B));

            cout << "nodes: " << N << " each with " << B << " values" << endl;
            cout << "Size of Node Array: " << sizeof(NodeArray) << " and size of double " << sizeof(double) << " bytes"
                 << endl;

            for (auto item: maps) {
                string spacesString;
                for (int i = 0; i < 20 - item.second.size(); i++)
                    spacesString += " ";
                cout << item.second << spacesString << ": " << item.first << endl;
            }
            cout << "------------------------" << endl;
        }
    }
}


void testCahce(int N, int B) {
    //cout << testPointerPointer(N, B).first;
      //cout << testVectorPointer(N, B).first;
      //cout<< testPointerVector(N, B).first;
      cout<< testVectorVector(N, B).first;
     // cout<<testVectorArray(N, B).first;
    // cout <<testSingleArray(N, B).first;
}

int main() {
    vector<int> nodeSizes = {1000000, 2000000, 3000000, 40000000};
    vector<int> valuesSizes = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int N = 40000000, B = 4;

    //testPerformance(nodeSizes, valuesSizes);
    testCahce(N, B);

    return 0;
}
