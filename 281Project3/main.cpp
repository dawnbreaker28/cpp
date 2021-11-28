#include <iostream>
#include "kdtree.hpp"
typedef std::tuple<int, int, double> KEY;
typedef int VALUE;
typedef std::pair<KEY , VALUE> KEY_VALUE;
using namespace std;
int main() {
    srand(124);
    KDTree<KEY, VALUE> kdt3;
    std::vector<KEY_VALUE> pairs;
    for (int i = 0; i < 10000; ++i) {
        KEY_VALUE k = {{rand() % 3000, rand() % 3000, rand() % 3000}, rand() % 1000000};
        kdt3.insert(k.first, k.second);
    }

    cout << (kdt3.findMax(0))->second << endl;
    auto kdt = kdt3;
    kdt3.insert({1, 1, 1}, 1);

//    pairs.push_back({{10, 8, 3.5}, 1});
//    pairs.push_back({{12, 3, 4}, 2});
//    pairs.push_back({{6, 7, 3.8}, 3});
//    pairs.push_back({{32, 11, 2}, 4});
//    pairs.push_back({{5, 12, 4.8}, 5});
//    pairs.push_back({{20, 5, 9.6}, 6});
//    pairs.push_back({{18, 9, 5}, 7});
//    pairs.push_back({{7, 20, 5.6}, 8});
//    pairs.push_back({{6, 7, 3.8}, 10});
//
//    for (int i = 0; i < 20; ++i) {
//        auto[k,val]= pairs[static_cast<unsigned long long int>(i)];
//        auto[x,y,z]=k;
//        cout<<x<<","<<y<<","<<z<<","<<val<<endl;
//    }

    vector<int> va1;
    vector<int> va2;
    for (auto[p, va] :kdt) {
        cout << va << " ";
    }
    std::cout << "\n";

    int flag = 1;
    for (unsigned int i = 0; i < va1.size(); ++i) {
        if (va1[i] != va2[i]) flag = 0;
    }
    cout << flag << endl;
//    std::cout<<"\n";
//    for (int i = 0; i < 9; ++i) {
//        int temp= rand()%10;
//        auto[k,val]= pairs[static_cast<unsigned long long int>(temp)];
//        auto[x,y,z]=k;
//        cout<<x<<","<<y<<","<<z<<","<<val<<endl;
//        kdt.erase(k);
//        kdt2.erase(k);
//    }
//    for (auto [p,va] :kdt) {
//        std::cout<<va<<" ";
//    }
//    std::cout<<"\n";
//    for (auto [p,va] :kdt2) {
//        std::cout<<va<<" ";
//    }
//    std::cout<<"\n";
//    KDTree<KEY, VALUE> kdt2{pairs}

}