int main()
{
    int N, prime, i;
    input(N);

    prime = 1;

    for (i = 2; i * i <= N; i = i + 1)
    {
        if (N % i == 0)
        {
            prime = 0;
        }
    }
    
    if (prime==1)
    {
        output(1);
    }
    else
    {
        output(0);
    }

    return 0;
}
