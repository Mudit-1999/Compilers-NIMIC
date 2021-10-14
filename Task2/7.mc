int out[100000];
int count[10];
int getMax(int arr[], int n);
int countSort(int arr[], int n, int exp);
int radixsort(int arr[], int n);

int getMax(int arr[], int n) 
{ 
	int mx = arr[0], i; 
	for (i = 1; i < n; i =i+ 1)
	{ 
		if (arr[i] > mx)
		{ 
			mx = arr[i];
		}
	} 
	return mx; 
} 

int countSort(int arr[], int n, int exp) 
{ 
	int i; 

	for (i = 0; i < 10; i =i+ 1)
	{
		count[i] = 0;
	}

	for (i = 0; i < n; i =i+1)
	{ 
		count[ (arr[i] / exp) % 10 ] =  count[ (arr[i] / exp) % 10 ] + 1; 
	}

	for (i = 1; i < 10; i =i+ 1)
	{ 
		count[i] =count[i] + count[i - 1]; 
	}

	for (i = n - 1; i >= 0; i =i-1) 
	{ 
		out[count[ (arr[i]/exp)%10 ] - 1] = arr[i]; 
		count[ (arr[i]/exp)%10 ] = count[ (arr[i]/exp)%10 ] - 1; 
	} 

	for (i = 0; i < n; i =i+ 1) 
	{
		arr[i] = out[i];
	}
	return 0;
} 

int radixsort(int arr[], int n) 
{ 
	int m, exp;
	m = getMax(arr, n); 
	
	for (exp = 1; m / exp > 0; exp = exp * 10)
	{ 
		countSort(arr, n, exp);
	}
	return 0;
} 



int main() 
{
	int n;
    input(n);

    int a[n], i;
    for (i = 0; i < n; i = i+ 1)
    {
        input(a[i]);
    }

	radixsort(a, n); 

    for (i = 0; i < n; i =i+ 1)
    {
        output(a[i]);
    }
    output();

	return 0;
} 
