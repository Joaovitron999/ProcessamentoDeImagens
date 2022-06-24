/*-------------------------------------------------------------------------
 * Operacao de filtro de dados
 * Por Jõao Vitor Fonseca, biblioteca adaptada de Luiz Eduardo da Silva.
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

void writeDado(image In,image Out, int *pC,int *pL, int mn){        
    int l,c;
    l = *pL;
    c = *pC;

    

    *pC = *pC * 10;
    *pL = *pL * 10;
    // //Escreve o dado na imagem
    int x = 0;
    for (int i = 0; i < l; i+=40)
    {
        for (int j = 0; j < c; j+=40)
        {
            Out[(i) * c + (j)] = 300;//dados[(i+xL) * c + (j+xC)].img[xL*40 + xC];
        }
    }
    

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

    

    *pL = nlOut;
    *pC = ncOut;

    In = Out;
}

void separarTons(image In, image Out, int nl, int nc, int mn, int *pL,int *pC,int qntdTons, int *pMn){
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
            if(x>qntdTons){
                In[(i * (*pC)) + j] = qntdTons;
            }else{
                In[(i * (*pC)) + j] = x;
            }
            
        }
    }
    
    *pMn = qntdTons;
}

void filtroDados(image In, image Out, int nl, int nc, int mn, int *pL,int *pC,int *pMn)
{
    diminuir(In,Out,nl,nc,mn,pL,pC);
    separarTons(Out,In,nl,nc,mn,pL,pC,6,pMn);
    //void writeDado(image In,image Out, int *pC int *pL, int mn){    
    writeDado(In,Out,pC,pL,mn);
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
            //writeDado(number,Out,i,j,nc,mn);
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
    filtroDados(In, Out, nr, nc, ml,&nr,&nc,&ml);
    //teste(In, Out, nr, nc, ml);
    //-- save image
    img_put(Out, nameOut, nr, nc, ml, GRAY);
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
