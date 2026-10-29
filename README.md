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

- `CerealShell_new()` cria uma instancia.
- `CerealShell_newFunction(shell, nome, callback)` registra um callback.
- `CerealShell_start(shell, entrada)` analisa e executa os comandos da entrada.
- `CerealShell_initLoop(shell)` inicia o loop interativo que le da entrada padrao.
- `CerealShell_end(shell)` libera a instancia.
- `Function_getArgPositional()` e `Function_getArgNamed()` consultam argumentos durante a execucao de um callback.
- `NArgToken_getArg_l()`, `NArgToken_getArg_cc()` e as demais funcoes `NArgToken_getArg_*` obtem o valor tipado de um argumento.

## Organizacao

- `include/`: cabecalhos publicos e estruturas da biblioteca.
- `src/`: implementacoes do lexer, parser, argumentos, funcoes e shell.
- `Makefile`: compilacao dos arquivos C e empacotamento dos objetos.

> *README.md gerado por IA*