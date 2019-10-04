#include <stdio.h>
#include <stdlib.h>
int main()
{
    char c[1000];
    char d, e, ch, l, u;
    float li;
    FILE *fptr;
    int a, b, p, ui,i;
    int rcount = 0, dcount = 0, count = 0, ucount = 0, id;

    float stime[800], ftime[800], uf;
    char tt[800];
    if ((fptr = fopen("Vegas.tr", "r")) == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }
    float ratio;
    while (fscanf(fptr, "%c %f %d %d %c %c %c %d %c %c %c %c %c %c %c %d %f %d %d", &d, &li, &b, &a, &e, &l, &ch, &p, &u, &u, &u, &u, &u, &u, &u, &ui, &uf, &id, &id) != EOF)
    {
        while (fgetc(fptr) != '\n')
            ;

            if (d == 'r')
            {
                rcount++;
                if (e == 't')
                    count = count + p;
                if (e == 'c')
                    ucount = ucount + p;
            }
            else if (d == 'd')
            {
                dcount++;
            }
            if (id < 800)
            {
                tt[id] = e;
                if (b == 0 || b == 1)
                    stime[id] = li;
                else if (a == 4 || a == 5)
                    ftime[id] = li;
            }

    }
    // rcount = rcount;
    ratio = ((float)rcount) / ((float)rcount + (float)dcount);
    printf("ratio : %f   tcp countper sec: %d\n", ratio, count/15);
    float tdelay,cdelay;
    for(i=0 ;i<800;i++)
    {
        if(tt[i]=='t')
            tdelay =tdelay+(ftime[i]-stime[i]);
        else if(tt[i]=='c')
            cdelay =cdelay+(ftime[i]-stime[i]);
    }
    tdelay = tdelay/800.0;
    cdelay = cdelay /800.0;
    printf("Avg Delays:\nTCP: %f",tdelay);
    return 0;
}
