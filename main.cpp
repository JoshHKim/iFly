#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <string>
using namespace std;
void extract(string line,string& origin,string& dest, double& weight,bool cost){
    stringstream ss(line);
    vector<string> parsed;
    while (ss.good()) {
        string data;
        getline(ss, data, ',');
        parsed.push_back(data);
    }
    origin=parsed[5];
    dest = parsed[7];
    if(cost)
        weight=stod(parsed[13]);
    else
        weight=stod(parsed[9]);
}
vector<vector<double>> adjMatrix(bool cost,string fileName,unordered_map<string,int>& locations){
    fstream infile;
    infile.open(fileName,ios::in);
    vector<vector<double>> aMatrix(270,vector<double>(270,0));
    string data;
    int count=1;
    getline(infile,data);
    if(infile.is_open()) {
        while (getline(infile, data)) {
            string origin;
            string dest;
            double weight;
            extract(data, origin, dest, weight, cost);
            if (locations[origin] == 0) {
                locations[origin] = count;
                count++;
            }
            if (locations[dest] == 0) {
                locations[dest] = count;
                count++;
            }
            if (aMatrix[locations[origin] - 1][locations[dest] - 1] == 0)
                aMatrix[locations[origin] - 1][locations[dest] - 1] = weight;
        }
    }else
        cout<<"Not open";
    return aMatrix;
}
vector<double> dijkstraAM(int src,vector<vector<double>> am){
    vector<double> sol(270,10000000000);//arbitrarily large number
    vector<bool> set(270,false);
    sol[src]=0;
    for(int x=0;x<269;x++){
        double min = 10000000000;//arbitrarily large number
        int mindex;
        for(int y=0;y<270;y++){
            if(!set[y]&&sol[y]<=min){
                min = sol[y];
                mindex=y;
            }
        }
        set[mindex]=true;
        for(int z=0;z<270;z++){
            if(!set[z]&&am[mindex][z]!=0&&sol[mindex]!=10000000000&&sol[mindex]+am[mindex][z]<sol[z])
                sol[z]=sol[mindex]+am[mindex][z];
        }
    }
    return sol;
}
int main() {
    unordered_map<string,int> locations;
    string fileName="Cleaned_2018_Flights.csv";
    vector<vector<double>> adjM=adjMatrix(true,fileName,locations);
    vector<double> dij=dijkstraAM(locations["PHL"]-1,adjM);
    for(double a:dij){
        cout<<" "<<a;
        cout<<endl;
    }
    return 0;
}