int n = 5;

int main() {
    int a[n][n], b[n][n], c[n][n];
    int i, j, sum, k;

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
            sum = 0;
            for (k = 0; k < n; k=k+1)
            {
                sum =sum+a[i][k] * b[k][j];
            }
            c[i][j] = sum;
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
