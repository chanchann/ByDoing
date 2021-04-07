#include <iostream>
#include <vector>
using namespace std;

template<typename T> 
void insert_sort(vector<T>& arr) {
    if(arr.empty()) return;
    int n = arr.size();
    for(int i = 1; i < n; i++) {
        int value = arr[i];
        int j = i - 1;
        for(; j >= 0; j--) {
            if(arr[j] > value) {
                arr[j+1] = arr[j];
            } else {
                break;
            }
        }
        arr[j+1] = value; // 插入数据
    }
}



int main() {
	vector<int> arr = { 61, 17, 29, 22, 34, 60, 72, 21, 50, 1, 62 };
	insert_sort(arr);
	for (int i = 0; i < arr.size(); i++)
		cout << arr[i] << ' ';
	cout << endl;
	vector<float> arrf = { 17.5, 19.1, 0.6, 1.9, 10.5, 12.4, 3.8, 19.7, 1.5, 25.4, 28.6, 4.4, 23.8, 5.4 };
	insert_sort(arrf);
	for (int i = 0; i < arrf.size(); i++)
		cout << arrf[i] << ' ';
	cout << endl;
	return 0;
}