#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;
 
void print(const vector<int>& a) {
    for (int value : a) cout << value << " ";
    cout << "\n";
}
 
vector<int> add(const vector<int>& a, int pos, int value) {
    int n = static_cast<int>(a.size());
    if (pos < 0 || pos > n)
        throw out_of_range("Invalid insertion index");
    vector<int> result(n + 1);
    for (int i = 0; i < pos; ++i) result[i] = a[i];
    result[pos] = value;
    for (int i = pos; i < n; ++i) result[i + 1] = a[i];
    return result;
}
 
vector<int> removeAt(const vector<int>& a, int pos) {
    int n = static_cast<int>(a.size());
    if (pos < 0 || pos >= n)
        throw out_of_range("Invalid removal index");
    vector<int> result(n - 1);
    for (int i = 0; i < pos; ++i) result[i] = a[i];
    for (int i = pos + 1; i < n; ++i)
        result[i - 1] = a[i];
    return result;
}
 
vector<int> merge(const vector<int>& a,
                  const vector<int>& b) {
    vector<int> result(a.size() + b.size());
    for (size_t i = 0; i < a.size(); ++i) result[i] = a[i];
    for (size_t i = 0; i < b.size(); ++i)
        result[a.size() + i] = b[i];
    return result;
}
 
int main() {
    const vector<int> a =
        {0, 1, 3, 4, 5, 2, 1, -4, -1, 10, 55};
    cout << "Original: "; print(a);
    cout << "Block 1: "; print(add(a, a.size(), 10));
    cout << "Block 2: "; print(add(a, 0, 10));
    cout << "Block 3: "; print(add(a, 4, 10));
    cout << "Block 4: "; print(removeAt(a, a.size() - 1));
    cout << "Block 5: "; print(removeAt(a, 0));
    cout << "Block 6: "; print(removeAt(a, 5));
    vector<int> a1 = {1, 2, 3};
    vector<int> a2 = {4, 5, 6};
    cout << "Block 7: "; print(merge(a1, a2));
    return 0;
}


