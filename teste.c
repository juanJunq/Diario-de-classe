#include <stdio.h>
#include <string.h>

typedef enum 
{
    jan = 1, fev, mar, abr, mai, jun, jul, ago, set, out, nov, dez
} Mes;

typedef struct {
    int dia, ano;
    Mes mes; 
} Data;

typedef struct {
    char nome[100], local[100];
    Data data;
} Evento;

void cadastrar_eventos(Evento agenda[], int n)
{
    int mes;
    for(int i = 0; i < n; i++)
    {
        fgets(agenda[i].nome, 100, stdin);
        agenda[i].nome[strcspn(agenda[i].nome, "\n")] = '\0';
        fgets(agenda[i].local, 100, stdin);
        agenda[i].local[strcspn(agenda[i].local, "\n")] = '\0';
        scanf("%d %d %d", &agenda[i].data.dia, &mes, &agenda[i].data.ano);
        while(getchar() != '\n');
        agenda[i].data.mes = (Mes) mes;
    }
}

void imprimir_eventos(Evento agenda[], Data d, int n)
{
    int achei = 0;
    for(int i = 0; i < n; i++)
    {
        if(agenda[i].data.dia == d.dia && agenda[i].data.mes == d.mes && agenda[i].data.ano == d.ano)
        {
            printf("%s %s\n", agenda[i].nome, agenda[i].local);
            achei = 1;
        }
    }
    if(!achei)
    {
        printf("Nenhum evento encontrado!\n");
    }
}

int main()
{   
    int n, mes;
    scanf("%d", &n);
    Evento agenda[n];
    Data data;
    cadastrar_eventos(agenda, n);
    for(int i = 0; i < 3; i++){
        printf("%s %s\n", agenda[i].nome, agenda[i].local);
    }
    // scanf("%d %d %d", &data.dia, &mes, &data.ano);
    // imprimir_eventos(agenda, data, n);
    return 0;
}
