#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char descricao[100];
    float valor;
    char tipo[50];
    char categoria[50];
} Transacao;

#define MAX_TRANSACOES 50
#define ARQUIVO_TRANSACOES "transacoes.tsv"

void atualizarTotaisDespesas(const Transacao *lista, int tam, char categoriasDespesas[9][30], float valoresDespesas[9]) {
    for (int i = 0; i < 9; i++) {
        valoresDespesas[i] = 0;
    }

    for (int i = 0; i < tam; i++) {
        if (strcmp(lista[i].tipo, "Despesa") == 0) {
            for (int j = 0; j < 9; j++) {
                if (strcmp(lista[i].categoria, categoriasDespesas[j]) == 0) {
                    valoresDespesas[j] += lista[i].valor;
                    break;
                }
            }
        }
    }
}

void salvarTransacoes(const Transacao *lista, int tam) {
    FILE *arquivo = fopen(ARQUIVO_TRANSACOES, "w");
    if (arquivo == NULL) {
        printf("Erro ao salvar as transacoes.\n");
        return;
    }

    for (int i = 0; i < tam; i++) {
        fprintf(arquivo, "%s\t%.2f\t%s\t%s\n", lista[i].descricao,
                lista[i].valor, lista[i].tipo, lista[i].categoria);
    }

    fclose(arquivo);
}

int carregarTransacoes(Transacao *lista, char categoriasDespesas[9][30],
                       float valoresDespesas[9]) {
    FILE *arquivo = fopen(ARQUIVO_TRANSACOES, "r");
    if (arquivo == NULL) {
        return 0;
    }

    int total = 0;
    Transacao transacao;
    while (total < MAX_TRANSACOES &&
           fscanf(arquivo, "%99[^\t]\t%f\t%49[^\t]\t%49[^\n]\n",
                  transacao.descricao, &transacao.valor, transacao.tipo,
                  transacao.categoria) == 4) {
        lista[total++] = transacao;
    }

    fclose(arquivo);
    atualizarTotaisDespesas(lista, total, categoriasDespesas, valoresDespesas);
    return total;
}

void Imprimir() {
    printf("\n");
    printf("==================MENU==================\n");
    printf("[1]- Adicionar transacao\n");
    printf("[2]- Listar transacoes\n");
    printf("[3]- Mostrar resumo financeiro\n");
    printf("[4]- Editar transacao\n");
    printf("[5]- Remover transacao\n");
    printf("[6]- Pesquisar transacao\n");
    printf("[7]- Sair\n");
    printf("\n");
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Transacao adicionarTransacao(char listaCategoriaReceitas[5][30], char listaCategoriaDespesas[9][30], float valoresDespesas[9]) {
    Transacao trans;
    int tipo, categoria;
    printf("------------------------------------------\n");
    printf("               Nova Transacao            \n");
    printf("------------------------------------------\n");

    limparBuffer();

    printf("Descricao: ");
    fgets(trans.descricao, sizeof(trans.descricao), stdin);
    trans.descricao[strcspn(trans.descricao, "\n")] = '\0';

    printf("Valor(R$): ");
    scanf("%f", &trans.valor);
    while(trans.valor < 0) {
        printf("Valor invalido. Tente novamente!\n");
        printf("Valor(R$): ");
        scanf("%f", &trans.valor);
    }
    
    printf("1- Receita\n");
    printf("2- Despesa\n");
    printf("Tipo: ");
    scanf("%d", &tipo);
    while(tipo < 1 || tipo > 2) {
        printf("Tipo de transacao invalida. Tente novamente!\n");
        printf("1- Receita\n");
        printf("2- Despesa\n");
        printf("Tipo: ");
        scanf("%d", &tipo);
    }
    printf("\n");

    if(tipo == 1) {
        for(int i=0; i <5; i++) {
            printf("[%d] - %s\n", i+1, listaCategoriaReceitas[i]);
        }
    } else {
        for(int i=0; i<9; i++) {
            printf("[%d] - %s\n", i+1, listaCategoriaDespesas[i]);
        }
    }
    printf("Categoria: ");
    scanf("%d", &categoria);
    while (tipo == 1 && (categoria < 1 || categoria > 5)) {
        printf("Categoria invalida. Tente novamente!\n");
        for(int i=0; i <5; i++) {
            printf("[%d] - %s\n", i+1, listaCategoriaReceitas[i]);
        }
        printf("Categoria: ");
        scanf("%d", &categoria);
    }

    while (tipo == 2 && (categoria < 1 || categoria > 9)) {
        printf("Categoria invalida. Tente novamente!\n");
        for(int i=0; i <9; i++) {
            printf("[%d] - %s\n", i+1, listaCategoriaDespesas[i]);
        }
        printf("Categoria: ");
        scanf("%d", &categoria);
    }

    if(tipo == 2) {
        valoresDespesas[categoria - 1] += trans.valor;
    } 

    if(tipo == 1) {
        strcpy(trans.tipo,"Receita");
        strcpy(trans.categoria , listaCategoriaReceitas[categoria-1]);
    } else {
        strcpy(trans.tipo ,"Despesa");
        strcpy(trans.categoria ,listaCategoriaDespesas[categoria-1]);
    }
    
    return trans;
}

void listarTransacoes(Transacao *list, int tam) {
    if(tam == 0) {
        printf("Voce ainda nao possui transacoes!\n");
    } else {
        for(int i=0; i< tam; i++) {
            printf("%d° Transacao\n", i+1);
            printf("Descricao: %s\n", list[i].descricao);
            printf("Valor: R$%.2f\n", list[i].valor);
            printf("Tipo: %s\n", list[i].tipo);
            printf("Categoria: %s\n", list[i].categoria);
            printf("\n");
            printf("\n");
        }
    }
}

void imprimirResumoFinanceiro(Transacao *list, int tam, char categoriasDespesas[9][30], float valoresDespesas[9]) {
    float receita = 0, despesas = 0, maior_gasto = 0;
    float saldo, media_despesas;
    int indice_maior_gasto = -1;
    int numeros_despesas = 0;

    for(int i=0; i<tam; i++) {
        if (strcmp(list[i].tipo, "Receita") == 0) {
            receita += list[i].valor;
        } else {
            despesas += list[i].valor;
            if(list[i].valor > maior_gasto || indice_maior_gasto == -1) {
                maior_gasto = list[i].valor;
                indice_maior_gasto = i;
            }
            numeros_despesas++;
        }
    }
    saldo = receita - despesas;
    
    if (numeros_despesas > 0) {
        media_despesas = despesas / numeros_despesas;
    } else {
        media_despesas = 0;
    }

    printf("====================Resumo Financeiro====================\n");
    printf("\n");
    printf("Receita: R$%.2f\n", receita);
    printf("Despesas: R$%.2f\n", despesas);
    printf("Saldo: R$%.2f\n", saldo);

    if (numeros_despesas > 0) {
           printf("Maior despesa: %s - R$%.2f\n", list[indice_maior_gasto].descricao, maior_gasto);

           printf("Media das despesas: R$%.2f\n", media_despesas);
    } else {
           printf("Nenhuma despesa cadastrada.\n");
        }

    printf("\n");
    printf("\n");
    printf("================GASTOS POR CATEGORIA==================\n");
    printf("\n");

    int indiceCategoriaMaisCara = 0;
    float maior = valoresDespesas[0];
    for(int i=0; i<9; i++) {
        printf("%s\n", categoriasDespesas[i]);
        printf("R$ %.2f\n", valoresDespesas[i]);
        if(valoresDespesas[i] > maior) {
            maior = valoresDespesas[i];
            indiceCategoriaMaisCara = i;
        }
    }
        printf("\n");
        printf("\n");
        printf("=============Analise=========\n");
        printf("\n");
        printf("Categoria que mais consumiu: \n");
        printf("%s - %.2f\n", categoriasDespesas[indiceCategoriaMaisCara], maior);

        if(despesas > 0) {
            for (int i = 0; i < 9; i++) {
                float percentualDespesa = (valoresDespesas[i] * 100) / despesas;
                printf("%s representa %.2f%% das despesas\n",categoriasDespesas[i], percentualDespesa);

            } 
        } else {

            printf("Nao existem despesas para calcular os percentuais.\n");
    }
}

void imprimirOpcaoInvalida() {
    printf("Digite uma opcao valida!");
}

Transacao editarTransacao(Transacao *lista, int n, char listaCategoriaReceitas[5][30], char listaCategoriaDespesas[9][30], float valoresDespesas[9]) {
    Transacao tEditar = lista[n-1];
    int tipo, categoria;

    float valorAntigo = tEditar.valor; 
    char tipoAntigo[50]; 
    char categoriaAntiga[50];
    strcpy(tipoAntigo, tEditar.tipo); 
    strcpy(categoriaAntiga, tEditar.categoria);
    printf("=================EDITAR TRANSACAO================\n");
    printf("\n");
    printf("Descricao atual: %s\n", tEditar.descricao);
    printf("Valor atual: %.2f\n",tEditar.valor);
    printf("Tipo atual: %s\n", tEditar.tipo);
    printf("Categoria: %s\n", tEditar.categoria);

    limparBuffer();
    printf("Nova descricao: ");
    fgets(tEditar.descricao, sizeof(tEditar.descricao), stdin);
    tEditar.descricao[strcspn(tEditar.descricao, "\n")] = '\0';

    
    printf("Novo valor(R$): ");
    scanf("%f", &tEditar.valor);
    while(tEditar.valor < 0) {
        printf("Valor invalido. Tente novamente!\n");
        printf("Valor(R$): ");
        scanf("%f", &tEditar.valor);
    }
    
    printf("1- Receita\n");
    printf("2- Despesa\n");
    printf("Tipo: ");
    scanf("%d", &tipo);
    while(tipo < 1 || tipo > 2) {
        printf("Tipo de transacao invalida. Tente novamente!\n");
        printf("1- Receita\n");
        printf("2- Despesa\n");
        printf("Tipo: ");
        scanf("%d", &tipo);
    }
    printf("\n");

    if(tipo == 1) {
        for(int i=0; i <5; i++) {
            printf("[%d] - %s\n", i+1, listaCategoriaReceitas[i]);
        }
    } else {
        for(int i=0; i<9; i++) {
            printf("[%d] - %s\n", i+1, listaCategoriaDespesas[i]);
        }
    }
    printf("Categoria: ");
    scanf("%d", &categoria);

    while (tipo == 1 && (categoria < 1 || categoria > 5)) {
        printf("Categoria invalida. Tente novamente!\n");
        for(int i=0; i <5; i++) {
            printf("[%d] - %s\n", i+1, listaCategoriaReceitas[i]);
        }
        printf("Categoria: ");
        scanf("%d", &categoria);
    }

    while (tipo == 2 && (categoria < 1 || categoria > 9)) {
        printf("Categoria invalida. Tente novamente!\n");
        for(int i=0; i <9; i++) {
            printf("[%d] - %s\n", i+1, listaCategoriaDespesas[i]);
        }
        printf("Categoria: ");
        scanf("%d", &categoria);
    }

    if(strcmp(tEditar.tipo,"Despesa") == 0) {
        for(int i=0; i<9; i++) {
            if(strcmp(categoriaAntiga, listaCategoriaDespesas[i]) == 0) {
                valoresDespesas[i] -= valorAntigo;
                break;
            }
        }
    }

    if(tipo == 1) {
        strcpy(tEditar.tipo, "Receita");
        strcpy(tEditar.categoria, listaCategoriaReceitas[categoria-1]);
    } else {
        strcpy(tEditar.tipo, "Despesa");
        strcpy(tEditar.categoria, listaCategoriaDespesas[categoria - 1]);
        valoresDespesas[categoria - 1] += tEditar.valor;
    }
    return tEditar;
}

void removerTranscao(Transacao *list, int *tamanho, int indiceRemover, char categoriasDespesas[9][30], float valoresDespesas[9]) {
    if(indiceRemover < 0 || indiceRemover >= *tamanho ) {
        printf("Indice invalido!\n");
        return;
    }
    // Se for uma despesa, retirar o valor da categoria
    if(strcmp(list[indiceRemover].tipo, "Despesa") == 0) {

        for(int i = 0; i < 9; i++) {
            if(strcmp(list[indiceRemover].categoria, categoriasDespesas[i]) == 0) {
                valoresDespesas[i] -= list[indiceRemover].valor;
                break;
            }
        }
    }
    for(int i = indiceRemover; i < *tamanho - 1; i++) {
        list[i] = list[i + 1];
    }
    (*tamanho)--;
}

int pesquisarPorDescricao(Transacao *list, int tam, char descricao[50],Transacao *resultado) {
    int tem = 0;
    int indice;
    for(int i=0; i<tam; i++) {
        if(strcmp(list[i].descricao, descricao) == 0) {
            tem = 1;
            indice = i;
        }
    }
    if(tem == 0) {
        printf("Transacao nao encontrada!\n");
        return 0;
    }
    *resultado = list[indice];
    return 1;
}

int pesquisarPorCategoria(Transacao *list, int tam, char categoria[50], Transacao *resultado) {
    int j = 0;
    for(int i=0; i<tam; i++) {
        if(strcmp(list[i].categoria, categoria) == 0) {
            resultado[j] = list[i];
            j++;
        }
    }
    if(j == 0) {
        printf("Transacao nao encontrada!\n");
    }
    return j;
}

int pesquisarPorTipo(Transacao *list, int tam, char tipo[50], Transacao *resultado) {
    int j = 0;
    for(int i=0; i<tam; i++) {
        if(strcmp(list[i].tipo, tipo) == 0) {
            resultado[j] = list[i];
            j++;
        }
    }
    if(j == 0) {
        printf("Transacao nao encontrada!\n");
        return 0;
    }
    return j;
}

void pesquisarTransacao(Transacao *list, int tam) {
    int opcao;
    printf("================ PESQUISA ===============\n");
    printf("\n");
    printf("[1]- Pesquisar por descricao\n");
    printf("[2]- Pesquisar por cateogoria\n");
    printf("[3]- Pesquisar por tipo\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    while(opcao < 1 || opcao > 3) {
        printf("Escolha uma opcao valida. Tente novamente!\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
    }
    if(opcao == 1) {
        char desc[50];
        limparBuffer();
        printf("Digite a descricao: ");
        fgets(desc, sizeof(desc), stdin);
        desc[strcspn(desc, "\n")] = '\0';

        Transacao vetor;
        if (pesquisarPorDescricao(list, tam, desc, &vetor)) {
            printf("\n");
            printf("---------------------------------------------\n");
            printf("Descricao: %s\n", vetor.descricao);
            printf("Valor: %.2f\n", vetor.valor);
            printf("Tipo: %s\n", vetor.tipo);
            printf("Categoria: %s\n\n", vetor.categoria);
        }

    } else if(opcao == 2) {
        char cat[50];
        limparBuffer();
        printf("Digite a categoria: ");
        fgets(cat, sizeof(cat), stdin);
        cat[strcspn(cat, "\n")] = '\0';

        Transacao resultados[50];
        int qtd = pesquisarPorCategoria(list, tam, cat, resultados);
        printf("\n");
        printf("---------------------------------------------\n");
        for(int i=0; i<qtd; i++) {
            printf("Descricao: %s\n", resultados[i].descricao);
            printf("Valor: %.2f\n", resultados[i].valor);
            printf("Tipo: %s\n", resultados[i].tipo);
            printf("Categoria: %s\n\n", resultados[i].categoria);
        }
    } else {
        char tipo[50];
        limparBuffer();
        printf("Digite o tipo: ");
        fgets(tipo, sizeof(tipo), stdin);
        tipo[strcspn(tipo, "\n")] = '\0';

        Transacao resultados[50];
        int qtd = pesquisarPorTipo(list, tam, tipo, resultados);
        printf("\n");
        printf("---------------------------------------------\n");
        for(int i=0; i<qtd; i++) {
            printf("Descricao: %s\n", resultados[i].descricao);
            printf("Valor: %.2f\n", resultados[i].valor);
            printf("Tipo: %s\n", resultados[i].tipo);
            printf("Categoria: %s\n\n", resultados[i].categoria);
        }
    }

}
int main() {
    int opcao = -1;
    int totalTransacoes = 0;
    Transacao transacoes[MAX_TRANSACOES];

    char categoriasDespesas[9][30] = {
       "Alimentacao", //0
       "Moradia", //1
       "Transporte", //2
       "Estudos",//3
       "Saude",//4
       "Lazer",//5
       "Compras",//6
       "Contas",//7
       "Outros"//8
    };

    char categoriasReceitas[5][30] = {
       "Salario",
       "Freelance",
       "Bolsa",
       "Investimentos",
       "Outros"
    };

    float valoresDespesas[9] = {0};

    totalTransacoes = carregarTransacoes(transacoes, categoriasDespesas, valoresDespesas);
    if (totalTransacoes > 0) {
        printf("%d transacao(oes) carregada(s) do arquivo.\n", totalTransacoes);
    }

    while(opcao != 7) {
        Imprimir();
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
        if(totalTransacoes < MAX_TRANSACOES) {
            transacoes[totalTransacoes] = adicionarTransacao(categoriasReceitas, categoriasDespesas, valoresDespesas);
            totalTransacoes++;
            salvarTransacoes(transacoes, totalTransacoes);
            printf("\nTransacao adicionada com sucesso!\n\n");
            break;
        } else {
            printf("Limite de transacoes atingido!\n");
            break;
        }
        
        case 2:
            listarTransacoes(transacoes, totalTransacoes);
            break;
        case 3:
            imprimirResumoFinanceiro(transacoes, totalTransacoes, categoriasDespesas, valoresDespesas);
            break;

        case 4:
            int numeroTransacao;
            printf("\n");
            printf("Qual o numero da transacao que deseja editar? \n");
            scanf("%d", &numeroTransacao);
            if(numeroTransacao < 1 || numeroTransacao > totalTransacoes) {
                printf("Numero de trasacao invalido!\n");
                break;
            } else {
                transacoes[numeroTransacao-1] = editarTransacao(transacoes, numeroTransacao, categoriasReceitas, categoriasDespesas, valoresDespesas);
                salvarTransacoes(transacoes, totalTransacoes);
                printf("\nTransacao editada com suecesso!\n\n");
                break;
            }
        
        case 5:
            int numeroTransacaoRemover;
            printf("\n");
            printf("Qual o numero da transacao que deseja excluir? \n");
            scanf("%d", &numeroTransacaoRemover);
            printf("\n");
            if(numeroTransacaoRemover < 1 || numeroTransacaoRemover > totalTransacoes) {
                printf("Numero de transacao invalido");
                break;
            } else {
                printf("\n");
                printf("[%d]º Transacao\n", numeroTransacaoRemover);
                printf("Descricao: %s\n", transacoes[numeroTransacaoRemover-1].descricao);
                printf("Valor: R$%.2f\n", transacoes[numeroTransacaoRemover-1].valor);
                printf("Tipo: %s\n",transacoes[numeroTransacaoRemover-1].tipo);
                printf("Categoria: %s\n", transacoes[numeroTransacaoRemover-1].categoria);
                printf("\n");

                char confimar;
                printf("Confirmar exclusao?(s/n): ");
                scanf(" %c", &confimar);
                if(tolower(confimar) == 's') {
                    removerTranscao(transacoes, &totalTransacoes, numeroTransacaoRemover-1, categoriasDespesas,valoresDespesas);
                    salvarTransacoes(transacoes, totalTransacoes);
                    printf("Transacao excluida com sucesso!\n");
                    break;
                } else {
                    printf("Exclusao cancelada!");
                    break;
                }
            }
        
        case 6:
            pesquisarTransacao(transacoes, totalTransacoes);
            break;

        case 7:
            printf("Saindo...\n");
            break;

        default:
            imprimirOpcaoInvalida();
            break;
        }
    }
}
