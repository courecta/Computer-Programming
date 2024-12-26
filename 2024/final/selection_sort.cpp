#include <stdio.h>

int min_pos(const int arr[], int sz);
void swap(int* p, int* q);
void sort(int arr[], int sz);
int min(const int arr[], const int size);

int main(){
    int n = 0;
    scanf("%d", &n);
    int arr[10] = {0};
    for(int i = 0 ; i < n ; i++) scanf("%d", &arr[i]);
    sort(arr, n);
    for(int i = 0 ; i < n ; i++) printf("%d ", arr[i]);
    printf("\n");
}

int min_pos(const int arr[], int sz)
{
    int min_val = 0;
    for(int i = 0; i < sz; i++) if(arr[i] < arr[min_val]) min_val = i;
    return min_val;
}

void swap(int* p, int* q)
{
    int tmp = *p;
    *p = *q;
    *q = tmp;
}

void sort(int arr[], int sz)
{
    for(int i = 0 ; i < sz ; i++)
    {
        int p = min_pos(arr + i, sz - i);
        swap(&arr[i], &arr[p + i]);
    }
}

