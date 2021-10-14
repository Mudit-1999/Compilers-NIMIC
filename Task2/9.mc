int n = 5;

int main()
{
    int a[n][n], b[n][n], c[n][n];
    int i, j;

    for (i = 0; i < n; i=i+1)
    {
        for (j = 0; j < n; j=j+1)
        {
            input(a[i][j]);
        }
    }
    
    for (i = 0; i < n; i=i+1)
    {
        for (j = 0; j < n; j=j+1)
        {
            input(b[i][j]);
        }
    }
    for (i = 0; i < n; i=i+1)
    {
        for (j = 0; j < n; j=j+1)
        {
            c[i][j] = a[i][j] + b[i][j];
        }
    }

    for (i = 0; i < n; i=i+1)
    {
        for (j = 0; j < n; j=j+1)
        {
            output(c[i][j]);
        }
    }

    return 0;
}
