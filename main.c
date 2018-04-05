#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#define true 1
#define false 0

int p[512];

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
    return a + t * (b - a);
}
double grad(int hash, double x, double y, double z) {
    int h = hash & 15;          // Take the hashed value and take the first 4 bits of it (15 == 0b1111)
    double u = h<8 ? x : y;     // If the most significant bit (MSB) of the hash is 0 then set u = x.  Otherwise y.
    double v;
    v = h<4 ? y : h==12 || h==14 ? x : z; // If the first and second significant bits are 0 set v = y
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

double noise(double x, double y, double z) {
    int X = (int)floor(x) & 255,
            Y = (int)floor(y) & 255,
            Z = (int)floor(z) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    double u = fade(x),
            v = fade(y),
            w = fade(z);
    int A = p[X]+Y, AA = p[A]+Z, AB = p[A+1]+Z, B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;

    return lerp(w, lerp(v, lerp(u, grad(p[AA], x  , y  , z),
                                grad(p[BA], x-1, y  , z )),
                        lerp(u, grad(p[AB], x  , y-1, z ),
                             grad(p[BB], x-1, y-1, z ))),
                lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),
                             grad(p[BA+1], x-1, y  , z-1 )),
                     lerp(u, grad(p[AB+1], x, y-1, z-1 ),
                          grad(p[BB+1], x-1, y-1, z-1 ))));
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
    float i = 0.0, j=0.0, k=0;
    loadPermutation("C:\\Users\\robso\\CLionProjects\\perlin_noise\\data.txt");

    do{
        //printf("Enter the values to x, y aned z(x y z):\n");
        //scanf("%lf %lf %lf", &x, &y, &z);
        printf("Noise for (%.3f, %.3f, %.3f) is %.17lf\n",i,j,k,noise(i,j,k));
        i+=0.001;
        j+=1.0;
        k+=1.0;
    }while (i < 256 && j<256 && k<256);

    return 0;
}
