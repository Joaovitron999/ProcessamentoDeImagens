/*-------------------------------------------------------------------------
 * Operacao de convolucao
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagelib.h"
#include <math.h>

// imagens dos dados
struct dado{
    image img;
    int ncDado,nrDado,mlDado,tpDado;
};
//vetor de dados
struct dado dados[6];
int ncDado, nrDado, mlDado, tpDado;

void lerDados(){
     //Ler dados
    int i = 0;
    dados[i].img = img_get("0.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
    i++;
    dados[i].img = img_get("1.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
    i++;
    dados[i].img = img_get("2.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
    i++;
    dados[i].img = img_get("3.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
    i++;
    dados[i].img = img_get("4.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
    i++;
    dados[i].img = img_get("5.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
    i++;
    dados[i].img = img_get("6.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);  
}

void freeDados(){
    for(int i = 0; i < 6; i++){
        img_free(dados[i].img);
    }
}

void writeDado(int number, image Out, int i, int j, int nc, int mn){    
    
    // int dadoNP = dados[number].nrDado * dados[number].ncDado;   //Numero de pixels do dado
    // //Escreve o dado na imagem
    // int x = 0;
    // for(int r = 0; r < dados[number].ncDado; r+=((nc - 1)* dados[number].ncDado)){
    //     for(int c = 0; c < dados[number].nrDado; c++){
    //         Out[i * dados[number].ncDado + r + c] = dados[number].img[x];
    //         x++;
    //     }
    // }

    
    //Out[i * nc + j] = number*mn/6;    //teste com intensidades
    //printf("\n\t%d",number*mn/6);
}

void diminuir(image In, image Out, int nl, int nc, int mn, int* pL, int *pC){
    int proporcao = ceil((float)nc/100);
    int nlOut = nl/proporcao;
    int ncOut = nc/proporcao;

    
    int i,j;
    for (i = 0; i < nlOut; i++)
    {
        for (j = 0; j < ncOut; j++)
        {
            Out[i * ncOut + j] = In[proporcao * i * nc + j * proporcao];
        }
    }

    printf("\n\t%d",nlOut);
    printf("\n\t%d",ncOut);

    *pL = nlOut;
    *pC = ncOut;

    In = Out;
}

void separarTons(image In, image Out, int nl, int nc, int mn, int *pL,int *pC,int qntdTons){
    for (int i = 0; i < *pL ; i++)
    {
        for (int j = 0; j < *pC ; j++)
        {
            
            int x = 0;
            int number = 0;
            //qntdTons = 6; // colocar 6
            do
            {
                number = (mn/qntdTons)*(x) ;
                x++;
            }
            while(In[(i * (*pC)) + j]>= (mn/qntdTons)*(x) && !In[i * *pC + j]<(mn/qntdTons)*(x+1));
            In[(i * (*pC)) + j] = number;
        }
    }
    Out = In;
}

void filtroDados(image In, image Out, int nl, int nc, int mn, int *pL,int *pC)
{
    diminuir(In,Out,nl,nc,mn,pL,pC);
    separarTons(In,Out,nl,nc,mn,pL,pC,6);
    //writeDado();
    freeDados();
}





void teste(image In, image Out, int nl, int nc, int mn)
{
    for (int i = 0; i <= nl; i++){
        for (int j = 0; j <= nc; j++)
        {
            int x = 0;
            int number = 0;
            int qntdTons = 6; // colocar 6
            do
            {
              number = (mn/qntdTons)*(x) ;
              x++;
            }
            while(In[i * nc + j]>= (mn/qntdTons)*(x) && !In[i * nc + j]<(mn/qntdTons)*(x+1));
            //int number = In[i * nc + j]/mn/6;
            writeDado(number,Out,i,j,nc,mn);
        }
    }
}



void msg(char *s)
{
    printf("\nConvolucao");
    printf("\n-------------------------------");
    printf("\nUso:  %s  nome-imagem[.pgm] \n\n", s);
    printf("    nome-imagem[.pgm] é o nome do arquivo da imagem \n");
    exit(1);
}

/*-------------------------------------------------------------------------
 * main function
*-------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    lerDados();
    //limpar tela
    system("clear");
    int nc, nr, ml, tp;
    char *p, nameIn[100], nameOut[100], cmd[110];
    image In, Out;
    if (argc < 2)
        msg(argv[0]);
    img_name(argv[1], nameIn, nameOut, GRAY);
    //-- read image
    In = img_get(nameIn, &nr, &nc, &ml, GRAY);
    Out = img_alloc(nr, nc);
    //-- transformation
    filtroDados(In, Out, nr, nc, ml,&nr,&nc);
    //teste(In, Out, nr, nc, ml);
    //-- save image
    img_put(Out, nameOut, nr, nc, ml, GRAY);
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
