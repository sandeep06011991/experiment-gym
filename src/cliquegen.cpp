//
// Created by sandeep on 19/9/19.
//

#include <fstream>
#include "iostream"
#include "string"
using namespace std;
int main(int argc, char *argv[]) {
//    if(argc !=1)return -1;
    int csize = atoi(argv[1]);
//    std::ofstream outfile("data/clique_"+to_string(csize));
    std::ofstream outfile("data/clique");
    outfile << csize <<" "<< csize*csize <<"\n";
    for(int i=1;i<csize+1;i++){
        for(int j=i+1;j<csize+1;j++){
            outfile<<i <<' '<<j<<'\n';
        }
    }
    outfile.flush();
    outfile.close();
}