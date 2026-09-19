# Controle Financeiro em C

Programa de terminal para registrar receitas e despesas pessoais, com resumo financeiro, análise de gastos por categoria e **persistência em arquivo**: os dados continuam disponíveis mesmo depois de fechar o programa.

![C](https://img.shields.io/badge/C-A8B9CC?style=flat&logo=c&logoColor=black)
![Terminal](https://img.shields.io/badge/Aplicação-Terminal-4D4D4D?style=flat&logo=gnubash&logoColor=white)

<!--
  Adicione aqui um print ou GIF do programa rodando:

  ![Prévia do projeto](./screenshot.png)
-->

## Funcionalidades

- **Adicionar transação** com descrição, valor, tipo (receita ou despesa) e categoria.
- **Listar** todas as transações cadastradas.
- **Editar** uma transação já existente.
- **Remover** uma transação, com pedido de confirmação antes de excluir.
- **Pesquisar** por descrição, categoria ou tipo.
- **Resumo financeiro**, que mostra:
  - total de receitas, total de despesas e saldo;
  - maior despesa e média das despesas;
  - total gasto em cada categoria;
  - categoria que mais consumiu dinheiro e o percentual de cada categoria sobre o total de despesas.
- **Persistência automática:** as transações são carregadas ao abrir o programa e salvas em `transacoes.tsv` a cada inclusão, edição ou remoção.
- **Validação de entrada:** rejeita valores negativos, tipos inexistentes e categorias fora da lista.

### Categorias

| Receitas | Despesas |
|---|---|
| Salario | Alimentacao |
| Freelance | Moradia |
| Bolsa | Transporte |
| Investimentos | Estudos |
| Outros | Saude |
| | Lazer |
| | Compras |
| | Contas |
| | Outros |

## Tecnologias

- **Linguagem C** (biblioteca padrão: `stdio.h`, `string.h`, `ctype.h`)
- Uso de `struct`, vetores, matrizes de strings, ponteiros e passagem de parâmetros por referência
- Leitura e escrita de arquivos com `fopen`, `fscanf` e `fprintf`

## Como executar

É necessário ter um compilador C, como o **GCC** (no Windows, via MinGW ou MSYS2).

```bash
git clone https://github.com/arturokumura/controle-financeiro-c.git
cd controle-financeiro-c

gcc main.c -o main
./main          # no Windows: main.exe
```

Execute o programa a partir da pasta do projeto: o arquivo `transacoes.tsv` é criado e lido no diretório atual.

### Menu

```
==================MENU==================
[1]- Adicionar transacao
[2]- Listar transacoes
[3]- Mostrar resumo financeiro
[4]- Editar transacao
[5]- Remover transacao
[6]- Pesquisar transacao
[7]- Sair
```

## Estrutura do projeto

```
.
├── main.c            # Código-fonte completo
└── transacoes.tsv    # Criado automaticamente na primeira transação salva
```

## Como funciona

- Cada transação é uma `struct Transacao` com descrição, valor, tipo e categoria. As transações ficam em um vetor de até **50 posições**.
- Os dados são gravados em um arquivo **TSV** (valores separados por tabulação), uma transação por linha. Esse formato é simples de ler com `fscanf` e também abre direto no Excel ou no Google Planilhas.
- Um vetor auxiliar guarda o total gasto em cada categoria de despesa. Ele é atualizado ao adicionar, editar e remover transações, e recalculado a partir do arquivo quando o programa inicia, então o resumo financeiro não precisa percorrer tudo a cada consulta.
- As operações são separadas em funções (`adicionarTransacao`, `editarTransacao`, `removerTranscao`, `pesquisarPorCategoria`, `imprimirResumoFinanceiro` e outras), e o `main` fica responsável só pelo menu.


## Autor

**Artur Okumura**
[LinkedIn](https://linkedin.com/in/arturokumura) · [GitHub](https://github.com/arturokumura)
