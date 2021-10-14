int pow(int a, int b);

int pow(int a, int b)
{
    int tmp, i;
    tmp = 1;
    for (i = 0; i < b; i = i + 1)
    {
        tmp = tmp * a;
    }
    return tmp;
}

int main()
{
    int N, k, sum, i;
    input(N);
    input(k);

    for (i = 1; i <= N; i = i + 1)
    {
        sum = sum + pow(i, k);
    }
    
    output(sum);

    return 0;
}
