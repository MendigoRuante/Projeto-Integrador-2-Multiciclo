# Projeto Multiciclo MIPS

Simulador de um processador MIPS multiciclo implementado em C. O projeto foi desenvolvido por uma equipe de 3 integrantes ao longo de 4 sprints.

## 😎 Descrição

Este simulador é capaz de interpretar e executar instruções binárias no formato MIPS, utilizando vários ciclos de clock para cada instrução (arquitetura multiciclo).

Durante a execução, o simulador:
- Carrega um arquivo `.mem` contendo as instruções binárias e os dados.
- Permite salvar:
  - Arquivo `.asm` com as instruções já decodificadas.
  - Arquivo `.mem` com o conteúdo da memória.

O sistema também possui uma funcionalidade de **retrocesso (back)**, utilizando uma **pilha de estados**, possibilitando retornar múltiplas etapas anteriores da simulação.

## 🛠️ Tecnologias Utilizadas

- Linguagem C
- Arquitetura MIPS Multiciclo
- Estrutura de dados (pilha)

## 🚀 Como Executar

1. Compile o código em C com o compilador de sua preferência:
   ```bash
   gcc Multiciclo.c TAD.c -o Multiciclo
2. Execute o programa
   ```bash
   ./Multiciclo
