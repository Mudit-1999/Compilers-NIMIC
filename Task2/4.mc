int main()
{
    int x, y, z, limit;
    int cnt=0;
    limit =  100000000;
    for (z = 1; z <= limit; z =z+1)
    {
        for (x = 1; x <= limit; x =x+ 1)
        {
            for (y = 1; y <= limit; y =y+ 1)
            {
                if (x * x + y * y == z * z)
                {
                    output(x);
                    output(y);
                    output(z);
                    output();
                    cnt= cnt+1;
                }
            }
        }
    }
    output(cnt);
    return 0;
}