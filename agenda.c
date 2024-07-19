#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// const char conceito[] =  {
//     [0 ... 39] = 'F', //Reprovado
//     [40 ... 59] = 'E', //Exame Especial
//     [60 ... 69] = 'D',
//     [70 ... 79] = 'C',
//     [80 ... 89] = 'B',
//     [90 ... 100] = 'A',
// };

typedef struct
{
    char conceito, nome[100];
    int ID, nota[3], notaFinal;
} aluno_t;

typedef struct
{
    int numAlunos;
    char nome[100], ID[10];
    aluno_t alunos[6];
} turmas_t;

typedef struct
{
    char nome[100];
    int ID, numTurmas;
    turmas_t turmas[2];
} professor_t;

// Funcao responsavel pelo menu de acesso das operacoes
int menu(FILE *output_file, professor_t *professor, int *op); 

// Calcula a nota final do aluno (media aritmetica das 3 notas)
int notaFinal(professor_t *professor, int i, int j);

// Retorna a quantidade de alunos em uma turma
int GetNumAluno(professor_t *professor, char *id_turma);

// Retorna o indice da turma(maximo 2, diz se a turma esta no indice 0 ou 1)
int GetIndiceTurma(professor_t *professor, char *id_turma);

// Calcula o conceito(A,B,C,...,F) do aluno de acordo com o calculo da nota final
char calcConceito(professor_t *professor, int i, int j);

// Analisa o conceito atribuido a um aluno, interpreta e retorna a situacao(Aprovado, reprovado, exame especial)
const char* interpretaSituacao(professor_t *professor, int i, int x);

// Exibe as informacoes do professor(Nome, ID, turmas, alunos)
void exibirProfessor(professor_t *professor);

// Exibe as informacoes do aluno(Nome, ID, turmas, notas, conceito)
void exibirAluno(professor_t *professor); 

//Insire, respeitando um limite maximo de 6, um novo aluno a uma turma especifica
void inserirAluno(professor_t *professor);

// Atribui as 3 notas a um aluno especifico. Chama as funcoes calcConceito e notaFinal tambem
void inserirNota(professor_t *professor);

// Exibe as informacoes de uma turma especifica(Codigo, nome, alunos)
void exibirTurma(professor_t *professor);

// Exibe as informacoes dos alunos de uma turma e sua situacao(Aprovado, reprovado,...)
void situacaoAlunos(professor_t *professor);

// Exporta os dados da estrutura para um arquivo .txt
void exportarDados(FILE *file, professor_t *professor);

// Importa os dados de um arquivo .txt para uma estrutura
void importarDados(FILE *file,  professor_t *professor);


int main(int argc, char **argv)
{
    int op;
    professor_t prof;
    FILE *input_file, *output_file;
    input_file = fopen(argv[1], "r"); // Abre o arquivo de importacao no modo leitura 
    output_file = fopen(argv[2], "w"); // e o arquivo de exportacao no modo escrita
    // Verifica se houve algum erro na abertura de algum dos arquivos passados
    if(input_file == NULL || output_file == NULL)
    {
        perror("Error");
        return EXIT_FAILURE; // Caso haja algum erro, o programa encerra
    }
    importarDados(input_file, &prof); // Faz a importacao arquivo para estrutura
    do
    {
        // op e utilizado para monitorar as entradas do usuario para acessar as operacoes implementadas 
        op = menu(output_file, &prof, &op); 
    } while(op);
    fclose(input_file);
    fclose(output_file);
    return 0;
}

void situacaoAlunos(professor_t *professor)
{
    char id_turma[10];
    // O usuario entra o ID da turma desejado
    fgets(id_turma, 10, stdin);
    // Procura o caracter '\n' na string e substitui por '\0'
    id_turma[strcspn(id_turma, "\n")] = '\0'; 
    // Atribui o respectivo indice da turma para i 
    int i = GetIndiceTurma(professor, id_turma); 
    // Atribui o respectivo numero de alunos da turma para j
    int j = GetNumAluno(professor, id_turma);
    printf("Situacao na turma %s - %s\n", id_turma, professor->turmas[i].nome);
    for(int x = 0; x < j; x++)
    {
        printf("Aluno: %s\nMatricula: %d\n", professor->turmas[i].alunos[x].nome, professor->turmas[i].alunos[x].ID);
        printf("Nota final: %d - Conceito %c\n", professor->turmas[i].alunos[x].notaFinal, professor->turmas[i].alunos[x].conceito);
        // Chamada da funcao que interpreta o conceito de um aluno e diz sua situacao(retorna uma string)
        printf("Situacao: %s\n", interpretaSituacao(professor, i, x));
    }
}

const char* interpretaSituacao(professor_t *professor, int i, int x)
{   
    // Analisa os possiveis casos para cada conceito e retorna uma string
    if(professor->turmas[i].alunos[x].conceito == 'F')
        return "Reprovado";
    if(professor->turmas[i].alunos[x].conceito == 'E')
        return "Exame Especial";
    if(professor->turmas[i].alunos[x].conceito == 'D')
        return "Aprovado";
    if(professor->turmas[i].alunos[x].conceito == 'C')
        return "Aprovado";
    if(professor->turmas[i].alunos[x].conceito == 'B')
        return "Aprovado";
    if(professor->turmas[i].alunos[x].conceito == 'A')
        return "Aprovado";
}

void exibirTurma(professor_t *professor)
{   
    char id_turma[10];
    fgets(id_turma, 10, stdin);
    id_turma[strcspn(id_turma, "\n")] = '\0'; // Procura e substitui o caracter '\n' por '\0' 
    int i, j;
    // Atribui o respectivo indice da turma para i 
    i = GetIndiceTurma(professor, id_turma);
    // Atribui o respectivo numero de alunos da turma para j
    j = GetNumAluno(professor, id_turma);
    printf("Informacoes da turma %s - %s\n", professor->turmas[i].ID, professor->turmas[i].nome);
    printf("%d alunos\n", j);
    for(int x = 0; x < j; x++)
    {
        printf("Aluno: %s\nMatricula: %d\n", professor->turmas[i].alunos[x].nome, professor->turmas[i].alunos[x].ID);
    }
}

int GetNumAluno(professor_t *professor, char *id_turma)
{
    for(int i = 0; i < professor->numTurmas; i++)
    {
        if(!(strcmp(id_turma, professor->turmas[i].ID)))
        {
            // Retorna a quantidade de alunos de uma turma 
            return professor->turmas[i].numAlunos; 
        }
    }
}

int GetIndiceTurma(professor_t *professor, char *id_turma)
{
    for(int i = 0; i < 2; i++)
    {
        if(!(strcmp(id_turma, professor->turmas[i].ID)))
        {
            // Retorna o indice da turma no vetor turmas
            // Por exemplo, a turma ALG I pode estar na posicao 0 ou 1 do vetor(maximo de 2 posicoes)
            return i;
        }
    }
}

void inserirAluno(professor_t *professor)
{  
    int numAlunos, id_aluno, indice_turma;
    char nome[100], id_turma[10];
    fgets(nome, 100, stdin);
    nome[strcspn(nome, "\n")] = '\n'; // Procura e substitui o caracter '\n' por '\0' 
    scanf("%d", &id_aluno);
    fgets(id_turma, 10, stdin);
    id_turma[strcspn(id_turma, "\n")] = '\n'; // Procura e substitui o caracter '\n' por '\0' 
    // Atribui o respectivo numero de alunos da turma para numAlunos
    numAlunos = GetNumAluno(professor, id_turma);
    // Atribui o respectivo indice da turma para indice_turma
    indice_turma = GetIndiceTurma(professor, id_turma);
    // Uso de i e j como indices apenas para facilitar a legibilidade do codigo
    int j = numAlunos, i = indice_turma;
    if(numAlunos < 6) 
    {
        strcpy(professor->turmas[i].alunos[j].nome, nome);
        professor->turmas[i].alunos[j].ID = id_aluno;
        // Incrementa o numero de alunos de uma turma apos insercao(Ate um maximo de 6 alunos por turma)
        professor->turmas[i].numAlunos++;
    } else printf("Turma cheia\n");
}

void inserirNota(professor_t *professor)
{
    int id_aluno, achei = 0, i, j;
    char id_turma[10];
    scanf("%d", &id_aluno);
    fgets(id_turma, 10, stdin);
    id_turma[strcspn(id_turma, "\n")] = '\0'; // Procura e substitui o caracter '\n' por '\0' 
    // Atribui o respectivo indice da turma para i
    i = GetIndiceTurma(professor, id_turma); 
    for(int j = 0; j < professor->turmas[i].numAlunos; j++)
    {
        if(id_aluno == professor->turmas[i].alunos[j].ID)
        {
            for(int k = 0; k < 3; k++)
            {
                // Atribuicao de valores para cada elemento(3) do vetor nota
                scanf("%d", &professor->turmas[i].alunos[j].nota[k]);
            }
            // Apos insercao das notas, e possivel calcular a media das notas(notaFinal)
            // e o conceito do aluno(calcConceito)
            professor->turmas[i].alunos[j].notaFinal = notaFinal(professor, i, j);
            professor->turmas[i].alunos[j].conceito = calcConceito(professor, i, j);
            // Variavel de controle que e atualizada apos encontrar o aluno desejado
            achei = 1;
        }
        // Quebra da estrutura de repeticao se o aluno ja tiver sido encontrado
        if(achei)
            break;
    }

}

int notaFinal(professor_t *professor, int i, int j)
{
    // Calculo da nota final(media das 3 notas)
    // Apos o calculo, retorna o valor obtido
    int soma = 0, notaFinal;
    for(int x = 0; x < 3; x++)
    {
        soma += professor->turmas[i].alunos[j].nota[x];
    }
    notaFinal = soma/3;
    return notaFinal;
}

char calcConceito(professor_t *professor, int i, int j)
{
    int nota;
    // Calcula a nota final do aluno para o caso da funcao ser chamada antes da funcao inserirNota()
    nota = notaFinal(professor, i, j);
    // Retorna um valor de conceito de acordo com a nota calculada
    if(nota < 40)
        return 'F';
    if(nota >= 40 && nota < 60)
        return 'E';
    if(nota >= 60 && nota < 70)
        return 'D';
    if(nota >= 70 && nota < 80)
        return 'C';
    if(nota >= 80 && nota < 90)
        return 'B';
    if(nota >= 90)
        return 'A';
}

void exibirAluno(professor_t *professor)
{
    int id_aluno, numTurmas, numAlunos, achei = 0;
    char id_turma[10], conceito;
    scanf("%d", &id_aluno);
    fgets(id_turma, 10, stdin);
    id_turma[strcspn(id_turma, "\n")] = '\0'; // Procura e substitui o caracter '\n' por '\0' 
    numTurmas = professor->numTurmas;
    for(int i = 0; i < numTurmas; i++)
    {
        // Verifica se a turma do indice i e a turma desejada pelo usuario
        if(!(strcmp(id_turma, professor->turmas[i].ID)))
        {
            // Caso seja, captura o numero M de alunos 
            numAlunos = professor->turmas[i].numAlunos;
            for(int j = 0; j < numAlunos; j++)
            {
                // Procura, pelo ID, o aluno desejado na turma
                if(id_aluno == professor->turmas[i].alunos[j].ID)
                {
                    // Calcula o conceito para o caso de nao ter sido calculado ainda
                    conceito = calcConceito(professor, i, j);
                    printf("Aluno: %s\nMatricula: %d\n", professor->turmas[i].alunos[j].nome, id_aluno);
                    for(int k = 0; k < 3; k++)
                    {
                        printf("Prova %d: %d / ", k+1, professor->turmas[i].alunos[j].nota[k]);
                    }
                    printf("\nNota final: %d - Conceito %c\n", professor->turmas[i].alunos[j].notaFinal, conceito);
                    // Variavel de controle que e atualizada apos encontrar o aluno desejado
                    achei = 1;
                    // Quebra da estrutura de repeticao mais interna, uma vez que o aluno ja foi encotrado
                    break;
                }
            }
            // Quebra da estrutura de repeticao mais externa se o aluno ja tiver sido encontrado
            if(achei)
                break;
        }
    }
}

void exibirProfessor(professor_t *professor)
{
    int numTurmas;
    printf("Professor %s\nRegistro numero %d\n", professor->nome, professor->ID);
    for(int i = 0; i < professor->turmas[i].numAlunos; i++)
    {
        // Apresenta como saida as informacoes da turma(Nome, ID, alunos) do professor
        printf("Turma %s - %s, %d alunos\n", professor->turmas[i].ID, professor->turmas[i].nome, professor->turmas[i].numAlunos);
    }
}

void importarDados(FILE *file, professor_t *professor)
{
    int numAlunos, numTurmas;
    fgets(professor->nome, 100, file);
    professor->nome[strcspn(professor->nome, "\n")] = '\0'; // Procura e substitui o caracter '\n' por '\0' 
    fscanf(file, "%d", &professor->ID);
    fgetc(file);
    fscanf(file, "%d", &numTurmas);
    fgetc(file);
    professor->numTurmas = numTurmas;
    for(int i = 0; i < numTurmas; i++)
    {
        fgets(professor->turmas[i].nome, 100, file);
        fgets(professor->turmas[i].ID, 10, file);
        // Procura e substitui o caracter '\n' por '\0' 
        professor->turmas[i].nome[strcspn(professor->turmas[i].nome, "\n")] = '\0'; 
        professor->turmas[i].ID[strcspn(professor->turmas[i].ID, "\n")] = '\0';

    }
    for(int i = 0; i < numTurmas; i++)
    {
        fscanf(file, "%d", &numAlunos);
        fgetc(file);
        professor->turmas[i].numAlunos = numAlunos;
        for(int j = 0; j < numAlunos; j++)
        {
            fgets(professor->turmas[i].alunos[j].nome, 100, file);
            professor->turmas[i].alunos[j].nome[strcspn(professor->turmas[i].alunos[j].nome, "\n")] = '\0'; // Procura e substitui o caracter '\n' por '\0' 
            fscanf(file, "%d", &professor->turmas[i].alunos[j].ID);
            fgetc(file);
            for(int k = 0; k < 3; k++)
            {
                // Inicializa o vetor de notas para cada aluno
                professor->turmas[i].alunos[j].nota[k] = 0;
            }
        }
    }
    // printf("%s\n", professor->nome);
    // printf("%d\n", professor->ID);
    // printf("%s\n", professor->turmas[0].nome);
    // printf("%s\n", professor->turmas[0].ID);
    // printf("%s\n", professor->turmas[1].nome);
    // printf("%s\n", professor->turmas[1].ID);
    // printf("%d\n", professor->ID);
    // printf("%s\n", professor->turmas[0].alunos[2].nome);
    // printf("%d\n", professor->turmas[0].alunos[2].ID);
    // printf("%s\n", professor->turmas[1].alunos[1].nome);
    // printf("%d\n", professor->turmas[1].alunos[1].ID);
    // printf("%d\n", *numTurmas);   
}

void exportarDados(FILE *file, professor_t *professor)
{
    // Exportacao de dados formatas para um arquivo .txt
    int numAlunos, numTurmas;
    numTurmas = professor->numTurmas;
    fprintf(file, "Professor %s - Registro %d\n", professor->nome, professor->ID);
    for(int i = 0; i < numTurmas; i++)
    {
        fprintf(file, "Turma %s - %s\n", professor->turmas[i].ID, professor->turmas[i].nome);
        numAlunos = professor->turmas[i].numAlunos;
        for(int j = 0; j < numAlunos; j++)
        {
            fprintf(file, "Aluno: %s\nMatricula: %d\nNota Final: %d - Conceito %d - %s\n", 
            professor->turmas[i].alunos[j].nome, professor->turmas[i].alunos[j].ID, professor->turmas[i].alunos[j].notaFinal, 
            professor->turmas[i].alunos[j].conceito, interpretaSituacao(professor, i, j));
        }
    }
}

int menu(FILE *output_file, professor_t *professor, int *op)   
{
    // Captura a entrada passada pelo usuario
    char cursor = getchar(); 
    if (cursor == '\n')
    {
        // Se a entrada for apenas '\n', o programa retorna 0 para a variavel de controle op
        // e encerra a execucao
        return 0;
    }
    else
    {
        // Caso seja um valor diferente de '\n' e uma opcao valida, ocorre a seguinte operacao:
        // Codigo ASCII    Caractere: 
        //    48               0
        //    49               1
        //    ...             ...
        //    57               9
        // Como op e inteiro e o usuario fornece um char, ha conversao implicita e a seguinte operacao acontece:
        // Para cursor = '1' -->   *op = 49 - 48 = 1, por exemplo   
        *op = cursor - '0';
        while (getchar() != '\n');
        switch (*op)
        {
        case 1:
            exibirProfessor(professor);
            break;
        case 2:
            exibirAluno(professor);
            break;
        case 3:
            inserirAluno(professor);
            break;
        case 4:
            inserirNota(professor);
            break;
        case 5:
            exibirTurma(professor);
            break;
        case 6:
            situacaoAlunos(professor);
            break;
        case 7:
            exportarDados(output_file, professor);
            break;
        default:
            *op = 0;
            break;
        }
    }
    return 1;
}