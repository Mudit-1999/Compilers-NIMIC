int merge(int arr[], int l, int m, int r);

int merge(int arr[], int l, int m, int r) 
{ 
	int i, j, k, n1, n2; 
	n1 = m - l + 1; 
	n2 = r - m; 

	int L[n1], R[n2]; 

	for (i = 0; i < n1; i =i+1)
	{ 
		L[i] = arr[l + i]; 
	}
	for (j = 0; j < n2; j =j+1) 
	{
		R[j] = arr[m + 1+ j]; 
	}

	i = 0;  
	j = 0;  
	k = l;  
	while (i < n1 && j < n2) 
	{ 
		if (L[i] <= R[j]) 
		{ 
			arr[k] = L[i]; 
			i =i+1; 
		} 
		else
		{ 
			arr[k] = R[j]; 
			j =j+1; 
		} 
		k =k+ 1; 
	} 

	while (i < n1) 
	{ 
		arr[k] = L[i]; 
		i =i + 1; 
		k =k + 1; 
	} 

	while (j < n2) 
	{ 
		arr[k] = R[j]; 
		j =	j + 1; 
		k =	k + 1; 
	}

	return 0;
} 

int mergeSort(int arr[], int l, int r) 
{ 
	int m;
	if (l < r) 
	{ 
		m = l + (r - l) / 2; 

		mergeSort(arr, l, m); 
		mergeSort(arr, m+1, r); 
		merge(arr, l, m, r); 
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

	mergeSort(a, 0, n - 1); 

    for (i = 0; i < n; i =i+ 1)
    {
        output(a[i]);
    }
    output();

	return 0;
} 
