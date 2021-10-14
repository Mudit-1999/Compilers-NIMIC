int a[100000];
int insertion_sort(int arr[], int n);

int insertion_sort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i = i + 1)
    {
        key = arr[i];  
        j = i - 1;
  
        while (j >= 0 && arr[j] > key)
        {  
            arr[j + 1] = arr[j];  
            j = j - 1;  
        }  
        arr[j + 1] = key;  
    }
    return 0;
}

int main()
{
    int n;
    input(n);

    int  i;
    for (i = 0; i < n; i =i+ 1)
    {
        input(a[i]);
    }

    insertion_sort(a, n);

    for (i = 0; i < n; i =i+1)
    {
        output(a[i]);
    }
    output();

    return 0;
}