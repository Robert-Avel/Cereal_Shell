# Cereal Shell

Cereal Shell e uma biblioteca em C para interpretar comandos simples e encaminha-los a funcoes registradas pela aplicacao. O projeto separa a leitura dos tokens, a analise da sintaxe e a entrega dos argumentos para callbacks definidos pelo usuario.

## Funcionalidades

- Registro de funcoes por nome.
- Interpretacao de argumentos posicionais e nomeados.
- Suporte a texto, inteiros com sinal, inteiros sem sinal e valores booleanos.
- Execucao de varios comandos na mesma entrada, separados por ponto e virgula.
- Acesso aos argumentos analisados por meio da API de `Function` e `NArgToken`.

## Requisitos

- GCC ou compilador C compativel.
- GNU Make e `ar`.


## Compilacao

Na raiz do projeto, execute:

```sh
make
```
O `Makefile` compila os arquivos de `src/` e empacota os objetos no `libCerealShell.a`.

## Instalação (Linux)

(Opicional)Defina o PREFIX, que por padrão é `/usr/local`, se não definido
```sh
export PREFIX="caminho/de/instalação"
```

e então executar:
```sh
sudo make install
```

## Uso

O exemplo abaixo registra uma funcao que soma dois valores `long` e executa um comando:

```c
#include "CerealShell.h"
#include "Wrapper.h"

long soma(long esquerda, long direita)
{
    return esquerda + direita;
}

FUNCTION_NEW(
    soma,
    LONG,
    2,
    (c0, c1),
    ARG_ASSEMBLY(0, LONG)
    ARG_ASSEMBLY(1, LONG)
)

int main(void)
{
    struct CerealShell *shell = CerealShell_new();
    if (shell == NULL) {
        return 1;
    }

    if (CerealShell_newFunction(shell, "soma", __outCShellsoma) != 0) {
        CerealShell_end(shell);
        return 1;
    }

    CerealShell_start(shell, "soma 20 22;");
    CerealShell_end(shell);
    return 0;
}
```

> Obs: Em C++, utilizar `extern "C" { #include <CerealShell.h> }`

O wrapper gerado por `FUNCTION_NEW` imprime o retorno da funcao. Para callbacks sem retorno, o cabecalho `Wrapper.h` tambem define `FUNCTION_NEW_VOID`.

## Sintaxe dos comandos

Uma chamada tem o formato `nome argumentos;`. Os argumentos podem ser posicionais ou nomeados:

```text
soma 20 22;
exibir "ola mundo";
configurar -limite 10 -ativo true;
primeiro; segundo;
```

Regras observaveis no lexer e no parser:

- Texto com espacos deve ficar entre aspas duplas.
- Valores numericos sem sufixo sao interpretados como `long`; um `U` depois do numero indica `unsigned long`, por exemplo `12U`.
- `true` e `false` sao valores booleanos.
- Argumentos nomeados usam `-chave valor`.
- Depois do primeiro argumento nomeado, nao podem vir argumentos posicionais.
- Cada chamada deve terminar com `;`.

## API principal

- `CerealShell_new()` cria uma instancia do Shell, retornando um ponteiro da instancia `struct CerealShell`.
- `CerealShell_newFunction(shell, nome, callback)` registra uma função: Wrapper gerada pelo `FUNCTION_NEW()` ou `VOID_FUNCTION_NEW()`.
    - `shell` : Ponteiro para a instancia do Shell.
    - `nome` : O nome da função que sistema usa para chamar.
    - `callback` : o endereço da função Wrapper.
- `CerealShell_start(shell, entrada)` analisa e executa os comandos da entrada.
    - `shell` : Ponteiro para a instancia do Shell.
    - `entrada`: Entrada dos comandos em String.
- `CerealShell_initLoop(shell)` inicia o loop interativo que le da entrada padrao.
    - `shell` : Ponteiro para a instancia do Shell.
- `CerealShell_end(shell)` libera a instancia.
    - `shell` : Ponteiro para a instancia do Shell.


## MACROS principais

- `VOID_FUNCTION_NEW(function_name, n_args, caller, args...)` Cria uma função Wrapper sem tipo retorno
    - `function_name` O nome da função principa
    - `n_args` O número de argumentos da função
    - `caller` O Chamador da função, se não houver argumentos, o valor deve ser apenas `()`. Caso contrário, deve haver o identificador de argumento com prefixo **c** 
        - Ex1: (c0, c1, c2) = Argumento posicional 0, 1 e 2. 
        - Ex2:(cnome, cidade, cgenero) = Argumento nomeados `nome`, `idade` e `genero`.
    - `args`: os contrutores de argumentos `ARG_ASSEMBLY_POS` ou `ARG_ASSEMBLY_NAMED`.
- `FUNCTION_NEW(function_name, return_type, n_args, caller, args...)` Cria uma função Wrapper com retorno de *tipo compativel*
    - `return_type`: a Macro de Tipo compativeis
    - `...` : Os outros argumentos são identicos ao `VOID_FUNCTION_NEW`.
- `ARG_ASSEMBLY_POS(pos, type)` Gera um argumento posicional em uma macro de geração de funções.
    - `pos`: identificador de posição (o primeiro argumento começa em 0)
    - `return_type`: o tipo do argumento no formato em Macro de Tipo compativeis
- `ARG_ASSEMBLY_NAMED(name, type)` Gera um argumento nomeado em uma macro de geração de funções.
    - `pos`: identificador do argumento.
    - `return_type`: o tipo do argumento no formato em Macro de Tipo compativeis
- Macros de Tipos Compativeis:
    - `CS_BOOL` bool
    - `CS_STRING` const char*
    - `CS_LONG` long
    - `CS_UNSIGNED_LONG` unsigned long

## Organizacao

- `include/`: cabecalhos publicos e estruturas da biblioteca.
- `src/`: implementacoes do lexer, parser, argumentos, funcoes e shell.
- `Makefile`: compilacao dos arquivos C e empacotamento dos objetos.

> *README.md gerado por IA*
