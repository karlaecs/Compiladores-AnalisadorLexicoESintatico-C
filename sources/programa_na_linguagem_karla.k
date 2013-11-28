function fibonacci(int n)
{
     int i = 0;
     int j = 1;
     int k;
     for(k = 1; k < n; k = k + 1){
             output(i);
             int t = i + j;
             i = j;
             j = t;
     }
     return j;
}

