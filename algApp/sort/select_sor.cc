#include <iostream>
#include <vector>
using namespace std;

template<typename T> 
void select_sort(vector<T>& arr) {
    if(arr.empty()) return;
    int n = arr.size();
    for(int i = 0; i < n; i++) {
        int minIdx = i;
        for(int j = i + 1; j < n; j++) {
            if(arr[j] < arr[minIdx]) {
                minIdx = j;
            }
        }
        if(minIdx != i) swap(arr[i], arr[minIdx]);
    }
}


int main() {
	vector<int> arr = { 61, 17, 29, 22, 34, 60, 72, 21, 50, 1, 62 };
	select_sort(arr);
	for (int i = 0; i < arr.size(); i++)
		cout << arr[i] << ' ';
	cout << endl;
	vector<float> arrf = { 17.5, 19.1, 0.6, 1.9, 10.5, 12.4, 3.8, 19.7, 1.5, 25.4, 28.6, 4.4, 23.8, 5.4 };
	select_sort(arrf);
	for (int i = 0; i < arrf.size(); i++)
		cout << arrf[i] << ' ';
	cout << endl;
	return 0;
}