/*-------------------------------------------------------------------------
 * Operacao de convolucao
 * Por Luiz Eduardo da Silva.
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagelib.h"

void ordena(int *v, int n)
{
    int i, j, tmp;
    for (i = 0; i < n; i++)
    {
        int m = i;
        for (j = i + 1; j < n - 1; j++)
            if (v[j] < v[m])
                m = j;
        tmp = v[i];
        v[i] = v[m];
        v[m] = tmp;
    }
}
void filtroDados(image In, image Out, int nl, int nc, int mn)
{
    int w[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};
    for (int i = 1; i < nl - 1; i++)
        for (int j = 1; j < nc - 1; j++)
        {
            int sum = 0, vetor[9];
            vetor[0] = In[(i - 1) * nc + (j - 1)];
            vetor[1] = In[(i - 1) * nc + j];
            vetor[2] = In[(i - 1) * nc + (j + 1)];
            vetor[3] = In[i * nc + (j - 1)];
            vetor[4] = In[i * nc + j];
            vetor[5] = In[i * nc + (j + 1)];
            vetor[6] = In[(i + 1) * nc + (j - 1)];
            vetor[7] = In[(i + 1) * nc + j];
            vetor[8] = In[(i + 1) * nc + (j + 1)];
            ordena(vetor, 9);
            // for (int y = -1; y <= 1; y++)
            //     for (int x = -1; x <= 1; x++)
            //     {
            //         vetor[0] = In[(i + y) * nc + (j + x)] * w[y + 1][x + 1];
            //     }
            // valor = sum;
            // if (sum < 0)
            //     valor = 0;
            Out[i * nc + j] = vetor[4];
            // Out[i * nc + j] = valor;
            
        }
}

void writeDado(int number, image Out, int i, int j, int nc, int mn){
    //Escreve o dado na imagem

    Out[i * nc + j] = 1;    //teste com intensidades
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
    //filtroDados(In, Out, nr, nc, ml);
    teste(In, Out, nr, nc, ml);
    //-- save image
    img_put(Out, nameOut, nr, nc, ml, GRAY);
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
