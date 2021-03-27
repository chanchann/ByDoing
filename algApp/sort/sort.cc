#include <iostream>
#include <vector>
using namespace std;

template<typename T> 
void bubble_sort(vector<T>& arr) {
    int len = arr.size();
	for (int i = 0; i < len - 1; i++) {
		bool flag = false;   // 提前退出冒泡循环的标志位	
		for (int j = 0; j < len - 1 - i; j++) {
			if (arr[j] > arr[j + 1]) {
				swap(arr[j], arr[j + 1]);
				flag = true;   // 表示有数据交换
			}
		}
		if(!flag) break;  // 没有数据交换，提前退出		
	}
}
int main() {
	vector<int> arr = { 61, 17, 29, 22, 34, 60, 72, 21, 50, 1, 62 };
	bubble_sort(arr);
	for (int i = 0; i < arr.size(); i++)
		cout << arr[i] << ' ';
	cout << endl;
	vector<float> arrf = { 17.5, 19.1, 0.6, 1.9, 10.5, 12.4, 3.8, 19.7, 1.5, 25.4, 28.6, 4.4, 23.8, 5.4 };
	bubble_sort(arrf);
	for (int i = 0; i < arrf.size(); i++)
		cout << arrf[i] << ' ';
	cout << endl;
	return 0;
}