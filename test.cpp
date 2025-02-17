#include <iostream>
#include <string>
#include <set>

using namespace std;


int main() {
    set<int> temp = {1, 2, 3};
    temp.erase(4);
    for (const auto& item : temp) {
        cout << item << endl;
    }

    return 0;
}