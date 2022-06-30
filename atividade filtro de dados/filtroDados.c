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
struct dado
{
    image img;
    int ncDado, nrDado, mlDado, tpDado;
};
// vetor de dados
struct dado dados[6];
int ncDado, nrDado, mlDado, tpDado;

void lerDados()
{
    // Ler dados
    int i = 0;
    dados[i].img = img_get("./dados/dado-0.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
    i++;
    dados[i].img = img_get("./dados/dado-1.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
    i++;
    dados[i].img = img_get("./dados/dado-2.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
    i++;
    dados[i].img = img_get("./dados/dado-3.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
    i++;
    dados[i].img = img_get("./dados/dado-4.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
    i++;
    dados[i].img = img_get("./dados/dado-5.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
    i++;
    dados[i].img = img_get("./dados/dado-6.pgm", &dados[i].nrDado, &dados[i].ncDado, &dados[i].mlDado, GRAY);
}

void freeDados()
{
    for (int i = 0; i < 6; i++)
    {
        img_free(dados[i].img);
    }
}

void diminuir(image In, image Out, int nl, int nc, int mn, int *pL, int *pC)
{
    int proporcao = ceil((float)nc / 100);
    int nlOut = nl / proporcao;
    int ncOut = nc / proporcao;

    int i, j;
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

void separarTons(image In, int mn, int *pL, int *pC, int qntdTons, int *pMn)
{
    for (int i = 0; i < *pL; i++)
    {
        for (int j = 0; j < *pC; j++)
        {
            int x = -1;
            int number = 0;
            // qntdTons = 6; // colocar 6
            do
            {   
                x++;
                number = (mn / qntdTons) * (x);
            } while (In[i * *pC + j] > (mn / qntdTons) * (x));
            In[(i * (*pC)) + j] = x;
        }
    }
    //*pMn = qntdTons;
}

void writeDado(image In, image Out, int *pC, int *pL, int mn)
{
    int l, c;
    l = *pL;
    c = *pC;

    *pC = *pC * 40;
    *pL = *pL * 40;
    // //Escreve o dado na imagem
    int x = 0;
    for (int i = 0; i < l; i++) //percorrem imagem pequena
    {
        for (int j = 0; j < c; j++)
        {
            printf("\n\t%d",((i) * *pC*40) + j*40);
            Out[((i) * *pC*40) + j*40] = 255;
            //Out[((i) * c) + j] = *dados[In[(i * c) + j]].img; // pega o dado com relação ao pixel atual de In
            // TODO: substituir cada pixel por um dado, lembrando que um dado é uma matriz de pixels
        }
    }
}

void filtroDados(image In, image *Out, int nl, int nc, int mn, int *pL, int *pC, int *pMn)
{
    diminuir(In, *Out, nl, nc, mn, pL, pC);
    separarTons(*Out, mn, pL, pC, 6, pMn);

    image FinalOut = img_alloc(*pL * 40, *pC * 40);
    writeDado(*Out, FinalOut, pC, pL, mn);

    img_free(*Out);
    (*Out) =  FinalOut;

    freeDados();
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
    // limpar tela
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
    filtroDados(In, &Out, nr, nc, ml, &nr, &nc, &ml);
    //-- save image
    img_put(Out, nameOut, nr, nc, ml, GRAY);
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
