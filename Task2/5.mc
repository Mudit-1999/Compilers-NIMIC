int permute(int a[], int l, int r);

int n, a[10000];

int permute(int a[], int l, int r) 
{
    int i,il;
	if (l == r)
    {
        for (i = 0; i < n; i =i+ 1)
        {
            output(a[i]);   
        }
        output();
    }
	else
	{ 
		for (il = l; il <= r; il =il+1) 
		{ 
            int tmp=a[l];
            a[l]=a[il];
            a[il]=tmp; 
			permute(a, l + 1, r);
            tmp=a[l];
            a[l]=a[il];
            a[il]=tmp;
		} 
	}
    return 0; 
} 

int main() 
{
    input(n);

    int i;
    for (i = 1; i <= n; i =i+1)
    {
        a[i] = i;
    }

	permute(a, 0, n - 1);

    return 0;
}
