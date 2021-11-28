#include <iostream>
#include "kdtree.hpp"
#include <random>
using namespace std;
int main(){
    cout<<"Hello World\n";

    std::vector<std::pair<std::tuple<int,int>, int>> v;
    //srand(343456);
    srand(13);
    KDTree<std::tuple<int,int>,int> myTree2;
    for (int i = 0; i < 5; ++i) {
        int x=rand()%2;
        int y=rand()%2;
        int z=rand()%2;
        v.push_back({{x,y},z});
        myTree2.insert({x,y},z);
    }
//    cout<<"before"<<endl;
//    for (auto [p,va]: v) {
//        auto [x,y]=p;
//        cout<<x<<","<<y<<","<<va<<endl;
//    }

    KDTree<std::tuple<int,int>,int> myTree(v);

//    for (auto [p,va]: v) {
//        auto [x,y]=p;
//        cout<<x<<","<<y<<","<<va<<endl;
//    }
//    int max=-100000;
//    for (auto [p,value]: v) {
//        auto [x,y] = p;
//        if (x>max) max=x;
//    }
//    cout<<max<<endl;
//    cout<<std::get<0>((myTree.findMax(0))->first);
//    v.push_back({{1,2},1});
//    v.push_back({{1,2},2});
//    v.push_back({{1,8},3});
//    v.push_back({{4,4},4});

//    myTree2.insert({1,2},1);
//    myTree2.insert({1,2},2);
//    myTree2.insert({1,8},3);
//    myTree2.insert({4,4},4);
//    myTree.insert({5,3},53);
//    myTree.erase({1,1});
//    myTree.insert({8,2},82);
//    myTree.insert({9,1},91);
//    //myTree.erase({5,3});
//    for (auto &item : myTree) {
//        cout <<item.second << " ";
//    }
//
//    cout<<"\n";
////    KDTree<std::tuple<int,int>,int> myTree2;
////    myTree2=myTree;
////    myTree.insert({9,4},91);
//    for (auto &item : myTree2) {
//        cout <<item.second << " ";
//    }

    return 0;
}
