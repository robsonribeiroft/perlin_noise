int AbreArquivoPgm(int *width, int *height, int *maxValue){
    char fname[30];
    char key[20];
    printf("Digite o nome do arquivo PGM de entrada: ") ;
    scanf("%s", fname) ;
    FILE *arq = fopen(fname , "r") ;
    if(arq == NULL) {
        printf("Erro na abertura do arquivo %s\n", fname) ;
        return 0 ;
    }

    /* le dados do cabecalho */
    fscanf(arq, "%s", key) ;
    if(strcmp(key,"P2") != 0) {
        printf("Arquivo nao e um PGM\n") ;
        fclose(arq) ;
        return 0 ;
    }
    /* Leitura dos parametros da imagem */
    fscanf(arq, "%d %d %d", width, height, maxValue) ;
}