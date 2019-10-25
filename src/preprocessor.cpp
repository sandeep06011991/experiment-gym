//
// Created by sandeep on 11/8/19.
//

#include "iostream"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include "definitions.h"
using namespace std;

struct Vertex {
    int id;
    int degree;
};

struct ID_Map {
    int id;
    int mapping;
};

// orders in decreasing order of degree
bool comparator(Vertex n1, Vertex n2){
    if(n1.degree > n2.degree) return true;
    if(n1.degree == n2.degree) return n1.id < n2.id;
    return false;
}

bool comparator1(ID_Map n1,ID_Map n2){
    if(n1.mapping <= n2.mapping) return true;
    return false;
}

bool comparator2(int n1,int n2){
    if(n1 <= n2) return true;
    return false;
}
/*
 * To Remap ids of the graph such that if id_a<id_b
 * d(id_a) < d(id_b)
 * Gameplan
 * 1. Read input graph and create a list of vertex and degrees (DONE)
 * 2. Sort list by degree (DONE)
 * 2. Create a new vertex mapping  (DONE)
 * 3. Remap Input graph with new vertex ID (DONE) */
void preprocess(string inputfileName, string outputfileName){
//  INPUT
//    std::ifstream infile("data/data1.graph");
    std::ifstream infile("data/"+inputfileName);
    int a, b;
    int V, E;
    unordered_map<int, int> *vertex_degree_map = new unordered_map<int ,int>;
    unordered_map<int,bool> *vertex_valid_map = new unordered_map<int,bool>;
    unordered_map<int,vector<int> *> *graph = new unordered_map<int,vector<int> *>;
    infile >> V >> E;

//  Read all degree information.
    int edge_count = 0;
    int max_v = 0;
    while (infile >> a >> b)
    {
        max_v = max(a,max_v);
        max_v = max(b,max_v);
        auto it = vertex_degree_map->find(a);
        if(it == vertex_degree_map->end()){
            vertex_degree_map->insert(make_pair(a,1));
            vector<int>* t = new vector<int>();
            t->push_back(b);
            graph->insert(make_pair(a,t));
        }else{
            it->second ++;
            graph->find(a)->second->push_back(b);
        }

        it = vertex_degree_map->find(b);
        if(it == vertex_degree_map->end()){
            vertex_degree_map->insert(make_pair(b,1));
            vector<int>* t = new vector<int>();
            t->push_back(a);
            graph->insert(make_pair(b,t));
        }else{
            it->second ++ ;
            graph->find(b)->second->push_back(a);
        }
        // process pair (a,b)
        edge_count++;
    }
    infile.close();
    cout << "Edges Read: " << edge_count << "\n";

//  K-Core
//  Extract k-core table
    while(true){
        bool exit = true;
        for(auto it: *vertex_degree_map){
            if(it.second==1){
                exit = false;
                int nodeId = it.first;
                auto neighbours = graph->find(nodeId)->second;
                for (auto n = neighbours->begin(); n < neighbours->end(); n++){
                    (*vertex_degree_map)[*n] = (*vertex_degree_map)[*n] - 1;
                }
                (*vertex_degree_map)[nodeId] = 0;
//                cout << "removing " << nodeId << "\n";
            }
        }
        if(exit)break;
    }
    cout << "K core computation complete\n";
//  New Graph structure deleting vertexs
    unordered_map<int, int> *vertex_kcoredegree_map = new unordered_map<int ,int>;
    unordered_map<int,vector<int> *> *kcoregraph = new unordered_map<int,vector<int> *>;
    int total = 0;
    int not_deleted = 0;
    int edges = 0;
    for(auto g:*graph){
        total ++;
        int nodea = g.first;
        vector<int> *nb = g.second;
        if((*vertex_degree_map)[nodea]>0){
            vector<int>* t = new vector<int>();
            int n_size = 0;
            for(auto nodebpt = nb->begin();nodebpt < nb->end();nodebpt++){
                int nodeb = *nodebpt;
                if((*vertex_degree_map)[nodeb]>0) {
                    n_size++;
                    t->push_back(nodeb);
                    }
                }
            if(n_size>0){
                vertex_kcoredegree_map->insert(make_pair(nodea,n_size));
                kcoregraph->insert(make_pair(nodea,t));
                edges  = edges + n_size;
                not_deleted ++;

            }
        }
    }
    free(vertex_degree_map);
    free(graph);
    vertex_degree_map = vertex_kcoredegree_map;
    graph = kcoregraph;
    cout << "Total :" << total << "\n";
    cout << "NOT Deleted :" << not_deleted << "\n";

    E = edges;
    V = not_deleted;
//  Sort by degree
    vector<Vertex>* nodes = new vector<Vertex>;
    auto m = vertex_degree_map->begin();
    int p = 0;int pt=0;
    while(m!=vertex_degree_map->end()){
        struct Vertex t ;
        t.id = m->first;
        t.degree = m->second;
        if(t.degree==1)p++;
        nodes->push_back(t);
        m++;
        pt++;
    }
    cout <<"Single Node vertex found: " << p << "\n";
    cout << "Total vertex found" <<  pt << "\n";
//    vertex_degree_map->clear();
    sort(nodes->begin(), nodes->end(), comparator);


//  Create a new mapping
    int* new_vertexes_mapping  = (int *)malloc((max_v+1) * sizeof (int));
    int counter = 1;
    for(auto tt = nodes->begin(); tt != nodes->end(); tt++ )    {
//        cout  << tt->id << " " << tt->degree <<"\n";
          assert(tt->id < (max_v+1));
          new_vertexes_mapping[tt->id] = counter;
          counter ++;
    }
    nodes->clear();
//  Print reordered mapping into a new file
    std::ofstream outfile("data/"+outputfileName);
    outfile << V << " " << E << "\n";

    vector<ID_Map>* vA = new vector<ID_Map>;
    for(auto it = graph->begin();it!=graph->end();it++){
        struct ID_Map t;
        t.id = it->first;
        t.mapping = new_vertexes_mapping[t.id];
        vA->push_back(t);
    }
    sort(vA->begin(),vA->end(),comparator1);

    vector<int> * temp = new vector<int>;
    for(auto i = vA->begin();i!=vA->end();i++){
        auto it = graph->find((*i).id);
        int a = it->first;

        auto d = vertex_degree_map->find(a);
        if(d->second==1)continue;

        vector<int> *b = it->second;
        for(auto itt = b->begin();itt !=b->end();itt++){
            auto dd = vertex_degree_map->find(*itt);
            if(dd->second==1)continue;
            temp->push_back(new_vertexes_mapping[*itt]);
        }
        b->clear();
        sort(temp->begin(),temp->end(),comparator2);
        for(auto ittt = temp->begin();ittt !=temp->end();ittt++){
            outfile << new_vertexes_mapping[a] << " " << *ittt <<"\n";
        }
        temp->clear();
    }

//    cout << "Reordering Complete!\n";
}

/* Reorder graph ids by degree */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << argc <<"Usage: filename graphQuery";
        return 1;
    }
    string fileName;
    if(string("test").compare(argv[1])==0){
        fileName = "test.txt";
    }
    if (string("amazon").compare(argv[1])==0) {
        fileName = "com-amazon.ungraph.txt";
    }
    if (string("lj").compare(argv[1])==0) {
        fileName = "com-lj.ungraph.txt";
    }
    if (string("lj1").compare(argv[1])==0) {
        fileName = "soc-LiveJournal1.txt";
    }
    if (string("youtube").compare(argv[1])==0) {
        fileName = "com-youtube.ungraph.txt";
    }
    if (string("orkut").compare(argv[1])==0) {
        fileName = "com-orkut.ungraph.txt";
    }
    if (string("clique").compare(argv[1])==0) {
        fileName = "clique";
    }
    string outFileName = fileName + ".ro";
    preprocess(fileName, outFileName);

}