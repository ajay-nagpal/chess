#include<iostream>
#include<vector>
using namespace std;

int main(){
    vector<int>v{1,2};
    vector<vector<int>>v1{{1,2},{3,4}};

    for(int i=0;i<v.size();i++){
        cout<<v[i];
    }

    for(int i=0;i<v.size();i++){
        for(int j=0;j<v1[i].size();j++){
            cout<<v1[i][j];
        }
    }
}
