int is_prime(int N);

int is_prime(int N)
{
    if (N < 2)
    {
        return 0;
    }
    int prime, i;
    prime = 1;
    for (i = 2; i * i <= N; i = i + 1)
    {
        if (N % i == 0)
        {
            prime = 0;
        }
    }
    return prime;
}

int main()
{
    int N, sum, i;
    input(N);

    sum = 0;
    for (i = 1; i < N; i = i + 1)
    {
        if (is_prime(i)==1)
        {
            sum = sum + i;
        }
    }
    
    output(sum);

    return 0;
}