#include <iostream>
#include <vector>
using namespace std;

// 哨兵
template<typename T> 
void merge(vector<T>& arr, int left, int mid, int right) {
    vector<T> tmp(right-left);
    int i = left, j = mid, k = 0;
    while(i < mid && j < right) {
        if(arr[i] <= arr[j]) {
            tmp[k++] = arr[i++];
        } else {
            tmp[k++] = arr[j++];
        }
    }
    // 判断哪个数组还有剩余
    int start = i, end = mid;
    if(j < right) {
        start = j;
        end = right;
    }
    while(start < right) {
        tmp[k++] = arr[start++];
    }
    // tmp 拷贝回A[left...right]
    for(int i = 0; i < right-left; i++) {
        arr[left+i] = tmp[i];
    }
}

template<typename T> 
void mergeHelper(vector<T>& arr, int left, int right) {
    if(left >= right-1) return;  // 1个的时候
    int mid = left + (right-left+1)/2;
    mergeHelper(arr, left, mid);
    mergeHelper(arr, mid, right);
    merge(arr, left, mid, right);
}


template<typename T> 
void merge_sort(vector<T>& arr) {
    mergeHelper(arr, 0, arr.size());
}

int main() {
	vector<int> arr = { 61, 17, 29, 22, 34, 60, 72, 21, 50, 1, 62 };
	merge_sort(arr);
	for (int i = 0; i < arr.size(); i++)
		cout << arr[i] << ' ';
	cout << endl;
	vector<float> arrf = { 17.5, 19.1, 0.6, 1.9, 10.5, 12.4, 3.8, 19.7, 1.5, 25.4, 28.6, 4.4, 23.8, 5.4 };
	merge_sort(arrf);
	for (int i = 0; i < arrf.size(); i++)
		cout << arrf[i] << ' ';
	cout << endl;
	return 0;
}