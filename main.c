#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "functions.h"
#define true 1
#define false 0

int p[512];
int *width, *height, *maxValue;


double fade(double t) {
    /*
     *
     Fade function as defined by Ken Perlin.  This eases coordinate values
     so that they will ease towards integral values.  This ends up smoothing
     the final output.
     6t^5 - 15t^4 + 10t^3
    */
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(double t, double a, double b) {
    return ((1 - t) * a + t * b);
    //return a + t * (b - a);
}
double grad(int hash, double x, double y) {
    switch(hash & 3){
        case 0: return x + y;
        case 1: return -x + y;
        case 2: return x - y;
        case 3: return -x - y;
        default: return 0;
    }
}

double noise(double x, double y) {
    int xi = (int) floor(x) & 255;
    int yi = (int) floor(y) & 255;
    int g1 = p[p[xi] + yi];
    int g2 = p[p[xi + 1] + yi];
    int g3 = p[p[xi] + yi + 1];
    int g4 = p[p[xi + 1] + yi + 1];

    double xf = x - floor(x);
    double yf = y - floor(y);

    double d1 = grad(g1, xf, yf);
    double d2 = grad(g2, xf - 1, yf);
    double d3 = grad(g3, xf, yf - 1);
    double d4 = grad(g4, xf - 1, yf - 1);

    double u = fade(xf);
    double v = fade(yf);

    double x1Inter = lerp(u, d1, d2);
    double x2Inter = lerp(u, d3, d4);
    double yInter = lerp(v, x1Inter, x2Inter);

    return yInter;
}

void loadPermutation(char* fileName){
    FILE* fp = fopen(fileName,"r");
    int permutation[256],i;

    for(i=0;i<256;i++)  fscanf(fp,"%d",&permutation[i]);

    fclose(fp);

    for ( i=0; i < 256 ; i++) p[256+i] = p[i] = permutation[i];
}


int main(){
    double x=3.14, y=42, z=7;
    float i = 0.14, j=5.19, k=0;
    printf("Noise for (%.3f, %.3f, %.3f) is %.17lf\n",i,j,k,noise(i,j));

    //AbreArquivoPgm(width, height, maxValue);
    //printf("%p %p %p", width, height, maxValue);
    /*
    loadPermutation("C:\\Users\\robso\\CLionProjects\\perlin_noise\\data.txt");

    do{
        //printf("Enter the values to x, y aned z(x y z):\n");
        //scanf("%lf %lf %lf", &x, &y, &z);
        printf("Noise for (%.3f, %.3f, %.3f) is %.17lf\n",i,j,k,noise(i,j,k));
        i+=0.001;
        j+=1.0;
        k+=1.0;
    }while (i < 256 && j<256 && k<256);
    */

    return 0;
}
