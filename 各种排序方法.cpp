//
// Created by 24394 on 2026/8/16.
//
#include <iostream>
#include <functional>
using std :: cout;
using std :: endl;
using std :: function;

// 插入排序
void insert_sort(int arr[], int size){
    for(int i=1; i<size; ++i){
        if(arr[i] < arr[i-1]){   // i前面是已经排好序的部分（如果arr[i] >= arr[i-1]，说明不用往前面找地方插入）
            int temp = arr[i];
            int j = i - 1;
            // 此时默认下标i位置空出，从i-1开始，如果大于temp，就往右一位，再默认i-1空出，重复此过程，直到找到插入位置，结束循环
            while(j >= 0 && arr[j] > temp){
                arr[j+1] = arr[j];
                --j;
            }
            arr[j+1] = temp;
        }
    }
}

// 希尔排序（分块进行插入排序）
void shell_sort(int arr[], int size){
    for(int d=size/2; d>=1; d/=2){  // d表示arr中下标相差为d的元素
        for(int i=d; i<size; ++i){
            if(arr[i] < arr[i-d]){
                int temp = arr[i];
                int j = i-d;
                while(j >= 0 && arr[j] > temp){
                    arr[j+d] = arr[j];
                    j -= d;
                }
                arr[j+d] = temp;
            }
        }
    }
}

// 冒泡排序
void bubble_sort(int arr[], int size){
    for(int i=1; i<size; ++i){   // 第i轮交换
        bool flag = false;  // 记录本轮是否发生交换
        for(int j=0; j<size-i; ++j){
            if(arr[j] > arr[j+1]){
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                flag = true;
            }
        }
        if(!flag) return;   // 如果本轮没发生交换，说明剩下的部分已经有序，可以提前结束
    }
}

// 快速排序
void quick_sort(int arr[], int low, int high){
    auto partition = [](int arr[], int low, int high){  // 不需递归，类型直接写auto；同时不需由于不用改外部变量，不需[&]
        int pivot = arr[low];  // 选择第一个元素作枢轴
        while(low != high){
            // 靠右侧小于枢轴的数往左扔
            while(low < high && arr[high] >= pivot) high--;  // 从右往左找到当前第一个小于pivot的数
            arr[low] = arr[high];   // 与左侧空位交换（空位最初由取出枢轴产生，此后空位的位置不断因交换移动而改变）

            // 靠左侧大于枢轴的数往右扔
            while(low < high && arr[low] <= pivot) low++;
            arr[high] = arr[low];
        }
        arr[low] = pivot;
        return low;
    };

    if(low < high){
        int part_idx = partition(arr, low, high);
        quick_sort(arr, low, part_idx-1);
        quick_sort(arr, part_idx+1, high);
    }
}

// 选择排序
void select_sort(int arr[], int size){
    for(int i=1; i<size; ++i){   // 第i轮交换
        int min_index = i-1;  // 最小元素的索引值，本步中初始化为本轮排序的起点
        for(int j=i; j<size; ++j){
            if(arr[j] > arr[min_index]){
                min_index = j;
            }
        }
        // 第一次排序把最小的元素放在索引值为0的位置，之后第二次排序把最小的元素放在索引值为1的位置，以此类推
        if(min_index != i-1){
            int temp = arr[i-1];
            arr[i-1] = arr[min_index];
            arr[min_index] = temp;
        }
    }
}

// 归并排序
void merge_sort(int arr[], int low, int high){
    // 合并两个有序的数组（准确来说是一个数组的两个有序部分）
    auto merge = [&](int arr[], int low, int mid, int high){
        int temp[high-low+1];
        int i = low; int j = mid+1; int k = 0;
        while(i <= mid && j <= high){
            if(temp[i] <= temp[j]) temp[k++] = arr[i++];
            else temp[k++] = arr[j++];
        }
        while(i <= mid) temp[k++] = arr[i++];
        while(j <= high) temp[k++] = arr[j++];

        for(int p=0; p<k; ++p) arr[low+p] = temp[p];
    };

    if(low < high){
        int mid = low + (high - low)/2;
        merge_sort(arr, low, mid);  // 先排左边
        merge_sort(arr, mid+1, high);   // 再排右边
        merge(arr, low, mid, high);   // 把两个排序好的部分，用双指针有序合并
    }
}


