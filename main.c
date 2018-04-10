#include<stdlib.h>
#include<stdio.h>
#include<math.h>

/*
 * Tabela de pesquisa de hash, conforme definido por Ken Perlin.
 * Esta é uma matriz aleatoriamente organizada de todos os números de 0 a 255 inclusive.
 * */
int p[512];


/*
 * Função de atenuação conforme definido por Ken Perlin.
 * Isso facilita os valores de coordenadas, de modo que facilitem os valores integrais.
 * Isso acaba suavizando a saída final.
 * */
double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

/*
 *  Função para interporlação de valores.
 *  Utilizando a interpolação é possível obter valores diferentes, mas próximos aos valores originais,
 *  assim, não havera um salto sobre os tons de cinza.
 * */
double lerp(double t, double a, double b) {
    return ((1 - t) * a + t * b);
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

    int width, height, holder;

    puts("Qual o tamanho da img que deseja criar(largura altura)?");
    scanf("%d %d", &width, &height);

    loadPermutation("C:\\Users\\robso\\CLionProjects\\perlin_noise\\data.txt");

    FILE *file = fopen("perlin.pgm", "w");
    if (file == NULL){
        puts("Erro na criação do arquivo");
    } else{
        fprintf(file, "P2");
        fprintf(file,"%d %d 255\n", height, width) ;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                holder = (int) (noise((double)i/255, (double)j/255) * 255);
                fprintf(file, "%d ", holder);
            }
        }
    }
    fclose(file);

    return 0;
}
