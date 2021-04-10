#include <iostream>
#include <vector>
using namespace std;

template<typename T> 
int partition(vector<T>& arr, int left, int right) {
    int midVal = arr[right-1];
    int i = left;
    for(int j = left; j < right-1; j++) {
        if(arr[j] < midVal) {
            swap(arr[i], arr[j]);
            i++;
        }
    }
    swap(arr[i], arr[right-1]);
    return i;
}

template<typename T> 
void quickHelper(vector<T>& arr, int left, int right) {
    if(left >= right-1) return;
    int mid = partition(arr, left, right); // 获取分区点
    quickHelper(arr, left, mid);
    quickHelper(arr, mid+1, right);
}

template<typename T> 
void quick_sort(vector<T>& arr) {
    if(arr.empty()) return;
    quickHelper(arr, 0, arr.size());
}


int main() {
	vector<int> arr = { 61, 17, 29, 22, 34, 60, 72, 21, 50, 1, 62 };
	quick_sort(arr);
	for (int i = 0; i < arr.size(); i++)
		cout << arr[i] << ' ';
	cout << endl;
	vector<float> arrf = { 17.5, 19.1, 0.6, 1.9, 10.5, 12.4, 3.8, 19.7, 1.5, 25.4, 28.6, 4.4, 23.8, 5.4 };
	quick_sort(arrf);
	for (int i = 0; i < arrf.size(); i++)
		cout << arrf[i] << ' ';
	cout << endl;
	return 0;
}
