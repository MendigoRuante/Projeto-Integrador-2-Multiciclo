#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TAD.h"

int main() {

  Multiciclo *multiciclo = malloc(sizeof(Multiciclo));
  No *Pilha = NULL;
  multiciclo->pc = 0;
  multiciclo->Estado = 0;
  multiciclo->Parada_Estado = -1;
  int cont_inst = 0;

  for (int i = 0; i < NUM_REGISTRADORES; i++) {
    multiciclo->registradores[i] = 0;
  }

  Memoria *memoria = malloc(sizeof(Memoria)*Tam_Memoria);

  for (int i=0; i<Tam_Memoria;i++){
    strcpy(memoria[i].binario, "0000000000000000");
    memoria[i].dado = 0;
    if(i<=127){
      memoria[i].tipo = NOP;
    }else if (i>=128){
      memoria[i].tipo = Dado;
    }
    
  }
  int op_menuprincipal = 1;
  int opzerarpc = 0;
  while(op_menuprincipal != 0){
    printf("\n\n|=====================Menu Principal=====================|\n|0. Sair                                                 |\n|1. Carregar memória de instruções (.mem)                |\n|2. Imprimir memórias (instruções e dados)               |\n|3. Imprimir banco de registradores                      |\n|4. Imprimir todo o simulador (registradores e memórias) |\n|5. Salvar .asm                                          |\n|6. Salvar .mem                                          |\n|7. Executa Programa (run)                               |\n|8. Executa uma instrução (Step)                         |\n|9. Volta uma instrução (Back)                           |\n|========================================================|\nEscolha uma opção:\n");
    scanf("%d", &op_menuprincipal);
    switch(op_menuprincipal){
        default:
          printf("Opção Inválida...\n");
        break;
        case 0:
          printf("Saindo...");
        break;
        case 1:
          if (multiciclo->pc != 0){
            printf("Deseja colocar PC em 0?\n1.Sim\n2.Não\nEscolha uma opção:");
            scanf("%d", &opzerarpc);
            if (opzerarpc == 1){
              multiciclo->pc = 0;
            }
          }
          Ler_Arquivo(memoria, &cont_inst);
        break;
        case 2:
          imprimememoria(memoria);
        break;
        case 3:
          imprimir_registradores(multiciclo);
        break;
        case 4:
          imprimememoria(memoria);
          imprimir_registradores(multiciclo);
          printf("PC: %d\n", multiciclo->pc);
        break;
        case 5:
          ASM(memoria, cont_inst);
        break;
        case 6:
          GerarMEM(memoria);
        break;
        case 7:
          while(1){
            if (multiciclo->Parada_Estado == multiciclo->Estado){
              break;
            }
            empilhar(&Pilha,multiciclo);
            step(memoria, multiciclo);
          }
        break;
        case 8:
          empilhar(&Pilha,multiciclo);
          step(memoria, multiciclo);
        break;
        case 9:
          desempilhar(&Pilha,multiciclo);
        break;
    }
  }
  return 0;
}