// priority queue
// Created by 24394 on 2026/8/19.
// 二叉堆（优先队列）
#include <iostream>
using std :: cout;
using std :: endl;

// 调整以k为根的子树为大根堆（根值永远大于左右子节点的值）
void head_adjust(int arr[], int len, int k){
    int temp = arr[k];
    for(int i=k*2+1; i<len; i=i*2+1){
        if(i+1 < len && arr[i] < arr[i+1]) ++i;  // 把i定位到左右更大的那一个

        if(temp >= arr[i]) break;  // 说明当前根大于左右
        else{
            arr[k] = arr[i];
            k = i;
        }
    }
    arr[k] = temp;
}

void build_max_heap(int arr[], int len){
    for(int i=len/2-1; i>=0; --i){
        head_adjust(arr, len, i);
    }
}

void heap_sort(int arr[], int size){
    build_max_heap(arr, size);
    for(int i=size-1; i>0; --i){
        int temp = arr[i];
        arr[i] = arr[0];
        arr[0] = temp;
        head_adjust(arr, i, 0);
    }
}

int main(){
    int arr[10] = {4, 8, 6, 7, 5, 2, 3, 1, 3, 7};
    build_max_heap(arr, 10);
    for(int i: arr) cout << i << " ";
    cout << endl;
    heap_sort(arr, 10);
    for(int i: arr) cout << i << " ";
    cout << endl;
}