//automatic star finder routines
#include <stdio.h>
#include <stdlib.h>
#include "taki.h"
c_star st_target,
       st_next,
       st_current,
       st1,st2,st3;
double dt1,dt2,dt3;
char names[17][6]= {"A UMI","A TAU","B ORI","A AUR","A ORI","A CMA","A GEM","A CMI",
                    "B GEM","A LEO","A VIR","A BOO","A SCO","A LYR","A AQL","A CYG","A PSA"
                   };
double pi = 3.141592653589793;
double star_mat[17][2]=
{
    {37.960,  89.264}
    ,  {68.980,  16.509}
    ,  {78.634,  -8.202}
    ,  {79.172,  45.998}
    ,  {88.793,   7.407}
    , {101.287, -16.716}
    , {113.650,  31.888}
    , {114.825,   5.225}
    , {116.329,  28.026}
    , {152.093,  11.967}
    , {201.298, -11.161}
    , {213.915,  19.183}
    , {247.352, -26.432}
    , {279.234,  38.784}
    , {297.695,   8.868}
    , {310.358,  45.280}
    , {344.413, -29.622}
};
double distance[17][17];
double dt[3];

double  delta_angle(double ang_h, double ang_v, double ang_h1, double ang_v1,int rad)
{
    if (!rad)
    {
        ang_v/=RAD_TO_DEG;
        ang_h/=RAD_TO_DEG;
        ang_v1/=RAD_TO_DEG;
        ang_h1/=RAD_TO_DEG;
    }

    double delta_h = (ang_h1 - ang_h);
    double delta_v = (ang_v1  - ang_v);
    double a = sin(delta_v/2) * sin(delta_v/2) +
               cos(ang_v) * cos(ang_v1) *
               sin(delta_h/2) * sin(delta_h/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double distance = c*RAD_TO_DEG;
    //printf("%f\n", distance);
    return distance;
}
void fill_table(void)
{
    int i=0;
    int j=0;
    for (i=0; i<17; i++)
    {
        for (j=0; j<16; j++)
            distance[i][j]=delta_angle(star_mat[i][0],star_mat[i][1],star_mat[j][0],star_mat[j][1],0);
    }

}

void init_tak(void)
{
    reset_transforms(0.0, 0.0, 0.0);
    set_star(&st_target, 0.0, 90.0, 0.0, 36.80, 0);
    init_star(1, &st_target);
    set_star(&st_target, 0.00, 36.80, 180.00, 90.00, 0);
    init_star(2, &st_target);
    compute_trasform();
    set_star(&st_target, 178.0, 10.0, 0.0, 0.0,0);
    set_star(&st_next,178.0,10.0, 0.0, 0.0,0);
    to_alt_az(&st_target);
}

void foo_stars(int one,int two,int three)
{
    set_star(&st1,star_mat[one][0],star_mat[one][1],0,0,0);
    set_star(&st2,star_mat[two][0],star_mat[two][1],0,0,0);
    set_star(&st3,star_mat[three][0],star_mat[three][1],0,0,0);
    to_alt_az(&st1);
    to_alt_az(&st2);
    to_alt_az(&st3);
    dt[0]=delta_angle(st1.az,st1.alt,st2.az,st2.alt,1);
    dt[1]=delta_angle(st2.az,st2.alt,st3.az,st3.alt,1);
    dt[2]=delta_angle(st3.az,st3.alt,st1.az,st1.alt,1);
}

int main()
{
    int i=0;
    int j=0;
    fill_table();
    for (i=0; i<17; i++)

    {
        printf("%s ",names[i]);
        for (j=0; j<=i; j++)
            printf("%+06.3f  ",distance[i][j]);
        printf("\r\n");
    }

    init_tak();
    foo_stars(8,1,12);
    printf( "%+6.3f  %+6.3f  %+6.3f \r\n   ",dt[0],dt[1],dt[2]);
    printf("\r\n");;
    int k=0;
    for (k=0; k<=2; k++)
    {


        for (i=0; i<17; i++)

        {

            for (j=0; j<i; j++)
            {
                if (fabs(dt[k]-distance[i][j])<0.8)    printf(" (%s, %s) %+6.3f %f  ",names[i],names[j],distance[i][j],fabs(dt[k]-distance[i][j]));
            }

        }
        printf("\r\n");;
    }

    return 0;
}
