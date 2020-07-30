#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <map>
#include <fstream>


using namespace std;


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
    //  ~Node() { free(x); }
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
        for (int j = 0; j < len; j++)
            x[j] = 2;
    }
//    ~NodeArray()
//    {
//        delete this->x;
//        this->x = nullptr;
//    }
};

/**************************************************************
 * Helper functions for accessing container of a node. Uncomment the cout lines to see finer detailed explanations
 * Outouts of the performance of constructing a container.
 **************************************************************/
void displayFunctionAndDuration(string s, double d) {
    // cout << s << " construction: " << d << endl;
}

//access the container in a meaningful way to estimate memory references. Tree is single dimensional array
double accessContainer(double *tree, int size) {
    double sum = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned long long i = 0; i < size; i++) {
        sum += tree[i];
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    //cout << "time to access data: " << elapsed.count() << endl;
    return sum;
}

//access the container in a meaningful way to estimate memory references. Tree is double dimensional array
template<class T>
double accessContainer(T tree, int N, int B) {
    double sum = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (unsigned long long i = 0; i < N; i++) {
        for (unsigned long long j = 0; j < B; j++)
            sum += tree[i].x[j];
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    // cout << "time to access data: " << elapsed.count() << endl;
    //cout << "sum: " <<sum << endl;
    return sum;
}


/**************************************************************
 * Functions to test the performance of constructing containers of nodes
 **************************************************************/
pair<double, string> testVectorPointer(int N, int B) {
    auto start = std::chrono::high_resolution_clock::now();
    vector<Node> tree = vector<Node>(N, Node(B));
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    string s = __func__;
    //cout << s << endl;
    double d = elapsed.count();
    displayFunctionAndDuration(s, d);
    //cout << "sum: " << accessContainer(tree, N, B) << endl;
    cout << accessContainer(tree, N,B) << " ";
    tree.clear();
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
    cout << s << endl;
    double d = elapsed.count();
    displayFunctionAndDuration(s, d);
    cout << "sum: " << accessContainer(tree, N, B) << endl;
    tree.clear();
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
    cout << s << endl;
    double d = elapsed.count();
    displayFunctionAndDuration(s, d);
    cout << "sum: " << accessContainer(tree, M) << endl;
    //cout << accessContainer(tree, M) << endl;
    //free(tree);
    //tree = NULL;
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
    cout << s << endl;
    double d = elapsed.count();
    displayFunctionAndDuration(s, d);
    cout << "sum: " << accessContainer(tree, N, B) << endl;
    free(tree);
    tree = NULL;
    return make_pair(d, s);
}

pair<double, string> testPointerVector(int N, int B) {
    auto start = std::chrono::high_resolution_clock::now();
    NodeV *tree = (NodeV *) malloc(N * sizeof(NodeV));
    //same time
    for (int i = 0; i < N; i++) {
        void *place = &tree[i];
        NodeV *f = new(place) NodeV(B);
        //tree[i] = *f;
    }
//    for (int i = 0; i < N; i++)
//        for (int j = 0; j < B; j++)
//            tree[i].x.push_back(2);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    string s = __func__;
    cout << s << endl;
    double d = elapsed.count();
    displayFunctionAndDuration(s, d);
    cout << "sum: " << accessContainer(tree, N, B) << endl;
    free(tree);
    tree = NULL;
    return make_pair(d, s);
}

pair<double, string> testVectorVector(int N, int B) {
    auto start = std::chrono::high_resolution_clock::now();
    vector<NodeV> tree = vector<NodeV>(N, NodeV(B));
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    string s = __func__;
    cout << s << endl;
    double d = elapsed.count();
    displayFunctionAndDuration(s, d);
    cout << "sum: " << accessContainer(tree, N, B) << endl;
    tree.clear();
    return make_pair(d, s);
}

void testPerformance(vector<int> &nodeSizes, vector<int> &valuesSizes, ofstream &fileOut) {
    for (int i = 0; i < nodeSizes.size(); i++) {
        int N = nodeSizes[i] * 1000000;
        for (int j = 0; j < valuesSizes.size(); j++) {
            int B = valuesSizes[j];
            map<double, string> maps;

            maps.insert(testSingleArray(N, B));
            maps.insert(testPointerPointer(N, B));
            maps.insert(testVectorPointer(N, B));
            maps.insert(testPointerVector(N, B));
            maps.insert(testVectorVector(N, B));
            maps.insert(testVectorArray(N, B));


            cout << "nodes: " << N << " each with " << B << " values" << endl;
            //cout << "Size of Node Array: " << sizeof(NodeArray) << " and size of double " << sizeof(double) << " bytes"<< endl;

            for (auto item: maps) {
                string spacesString;
                for (int i = 0; i < 20 - item.second.size(); i++)
                    spacesString += " ";
                cout << item.second << spacesString << ": " << item.first << endl;
                fileOut << N << "," << B << "," << item.second << "," << item.first << endl;
            }
            cout << "------------------------" << endl;
        }
    }
}


void testCache(int N, int B) {
    // sorted functions from fastest to slowest. Still a hypothesis
    double time;
   // time = testVectorPointer(N, B).first;
 //   cout << time << endl;
//    time = testVectorArray(N, B).first;
//    cout << "construction: " << time << endl;
    time = testSingleArray(N, B).first;
   cout << "construction: " << time << endl;
//   time = testPointerPointer(N, B).first;
//    cout << "construction: " << time << endl;
 //   time = testPointerVector(N, B).first;
  //  cout << "construction: " << time << endl;
//    time = testVectorVector(N, B).first;
 //   cout << "construction: " << time << endl;
}

int main(int argc, char** argv) {
//    if(argc < 2 || argc > 3)
//    {
//        cout << "Error: Incorrect number of arguments!" <<endl;
//        cout<<"Usage: " << argv[0] << " <number of nodes> <number of values in each node>";
//        return -1;
//    }
  //  int N = stoi(argv[1]), B = stoi(argv[2]);
//    //string fileName = argv[3];
//    ofstream fileOutput;
//    //fileOutput.open(fileName);
// //   vector<int> nodeSizes, valuesSizes;
//    //outer vector
////    for(int i = 1; i <= 10 ; i++)
////        nodeSizes.push_back(i);
////    //inner vector
////    for(int j = 1; j <= 10 ; j++)
////        valuesSizes.push_back(j);
//    //vector<int> valuesSizes = {5, 10};
//
//    //nodeSizes = {N} ;
//    // valuesSizes = {B};
//    //testPerformance(nodeSizes, valuesSizes, fileOutput);
//   for(int B = 1; B <= 48; B++)
//   {
//       int N = 8000000/B;
//       testCache(N,B);
//   }
    testCache(8000000,1);
//  //  fileOutput.close();
//    //free (argv);
    return 0;
}
