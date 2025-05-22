#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Tam_Instrucao 17    // 16 bits + 1 para o \0
#define NUM_REGISTRADORES 8 // Numero de Registradores, 3 bits = 8
#define Tam_Memoria 256 // Limite da Memória

typedef enum { TipoR, TipoI, TipoJ, NOP } TipoInstrucao;

typedef struct {
  char binario[17];
  int op, rs, rt, rd, imm, funct, end;
  TipoInstrucao tipo;
  // Memória de dados
  int dado;
} Memoria;

typedef struct {
  int Ula_A;
  int Ula_B;
  int UlaZero;
  int SinalOverflow;
  int UlaSaida;
} Ula;

typedef struct {
  int pc;
  int registradores[NUM_REGISTRADORES];
  Memoria IR;
  int Regis_Inter;
  int Parada_Estado;
  int Estado;
  Ula Ula;
} Multiciclo;

// FUNÇÕES
void controle(Memoria *instrucao, Multiciclo *multiciclo, int estado);
void imprimir_registradores(Multiciclo *multiciclo);
Ula ULA(int ulaop, int operando1, int operando2);
int BinParaDec(char binario[]);
void Ler_Arquivo(Memoria *memoria, int *qtd_instrucoes, int *qtd_dados);
void identificainstrucao(Multiciclo *multiciclo);
void step(Memoria *memoria, Multiciclo *multiciclo);
void run(Memoria *memoria, Multiciclo *multiciclo);

void Decodicador(Memoria *memoria){
    char auxiliar[16];
    strncpy(auxiliar, memoria->binario, 4);
    auxiliar[4] = '\0';
    memoria->op = strtol(auxiliar, NULL, 2);
    switch(memoria->op){
    case 0: //Tipo R
      memoria->tipo = TipoR;
      strncpy(auxiliar, memoria->binario, 4);
      auxiliar[4] = '\0';
      memoria->op = strtol(auxiliar, NULL, 2);
      // RS
      strncpy(auxiliar, memoria->binario + 4, 3);
      auxiliar[3] = '\0';
      memoria->rs = strtol(auxiliar, NULL, 2);
      // RT
      strncpy(auxiliar, memoria->binario + 7, 3);
      auxiliar[3] = '\0';
      memoria->rt = strtol(auxiliar, NULL, 2);
      // RD
      strncpy(auxiliar, memoria->binario + 10, 3);
      auxiliar[3] = '\0';
      memoria->rd = strtol(auxiliar, NULL, 2);
      //Funct
      strncpy(auxiliar, memoria->binario + 13, 3);
      auxiliar[3] = '\0';
      memoria->funct = strtol(auxiliar, NULL, 2);
      //outros campos 0
      memoria->imm = 0;
      memoria->end = 0;
      break;
    case 2: //Tipo J
      memoria->tipo = TipoJ;
      // OP
      strncpy(auxiliar, memoria->binario, 4);
      auxiliar[4] = '\0';
      memoria->op = strtol(auxiliar, NULL, 2);
      // END
      strncpy(auxiliar, memoria->binario + 4, 12);
      auxiliar[12] = '\0';
      memoria->end = strtol(auxiliar, NULL, 2);
      //outros campos 0
      memoria->rs = 0;
      memoria->rt = 0;
      memoria->rd = 0;
      memoria->imm = 0;
      memoria->funct = 0;
      break;
    case 8: //Tipo I (BEQ)
      memoria->tipo = TipoI;
      // OP
      strncpy(auxiliar, memoria->binario, 4);
      auxiliar[4] = '\0';
      memoria->op = strtol(auxiliar, NULL, 2);
      // RS
      strncpy(auxiliar, memoria->binario + 4, 3);
      auxiliar[3] = '\0';
      memoria->rs = strtol(auxiliar, NULL, 2);
      // RT
      strncpy(auxiliar, memoria->binario + 7, 3);
      auxiliar[3] = '\0';
      memoria->rt = strtol(auxiliar, NULL, 2);
      // IMM
      strncpy(auxiliar, memoria->binario + 10, 6);
      auxiliar[6] = '\0';
      memoria->imm = strtol(auxiliar, NULL, 2);
      break;
    case 4: 
      memoria->tipo = TipoI;
      // OP
      strncpy(auxiliar, memoria->binario, 4);
      auxiliar[4] = '\0';
      memoria->op = strtol(auxiliar, NULL, 2);
      // RS
      strncpy(auxiliar, memoria->binario + 4, 3);
      auxiliar[3] = '\0';
      memoria->rs = strtol(auxiliar, NULL, 2);
      // RT
      strncpy(auxiliar, memoria->binario + 7, 3);
      auxiliar[3] = '\0';
      memoria->rt = strtol(auxiliar, NULL, 2);
      // IMM
      strncpy(auxiliar, memoria->binario + 10, 6);
      auxiliar[6] = '\0';
      memoria->imm = BinParaDec(auxiliar);
      break;
      case 11: 
      memoria->tipo = TipoI;
      // OP
      strncpy(auxiliar, memoria->binario, 4);
      auxiliar[4] = '\0';
      memoria->op = strtol(auxiliar, NULL, 2);
      // RS
      strncpy(auxiliar, memoria->binario + 4, 3);
      auxiliar[3] = '\0';
      memoria->rs = strtol(auxiliar, NULL, 2);
      // RT
      strncpy(auxiliar, memoria->binario + 7, 3);
      auxiliar[3] = '\0';
      memoria->rt = strtol(auxiliar, NULL, 2);
      // IMM
      strncpy(auxiliar, memoria->binario + 10, 6);
      auxiliar[6] = '\0';
      memoria->imm = BinParaDec(auxiliar);
      break;
      case 15: 
      memoria->tipo = TipoI;
      // OP
      strncpy(auxiliar, memoria->binario, 4);
      auxiliar[4] = '\0';
      memoria->op = strtol(auxiliar, NULL, 2);
      // RS
      strncpy(auxiliar, memoria->binario + 4, 3);
      auxiliar[3] = '\0';
      memoria->rs = strtol(auxiliar, NULL, 2);
      // RT
      strncpy(auxiliar, memoria->binario + 7, 3);
      auxiliar[3] = '\0';
      memoria->rt = strtol(auxiliar, NULL, 2);
      // IMM
      strncpy(auxiliar, memoria->binario + 10, 6);
      auxiliar[6] = '\0';
      memoria->imm = BinParaDec(auxiliar);
      break;
    }
}



void imprimir_registradores(Multiciclo *multiciclo) {
  printf("=====Banco de Registradores=====\n");
  for (int i = 0; i < NUM_REGISTRADORES; i++) {
    printf("R%d: %d\n", i, multiciclo->registradores[i]);
  }
  printf("================================\n");
}

Ula ULA(int ulaop, int operando1, int operando2) {
  Ula resultado_ula;
  switch (ulaop) {
    case 0: //ADD
      resultado_ula.UlaSaida = operando1 + operando2;
    break;
    case 2: //SUB
      resultado_ula.UlaSaida = operando1 - operando2;
    break;
    case 3: //AND
      resultado_ula.UlaSaida = operando1 & operando2;
    break;
    case 4: //OR
      resultado_ula.UlaSaida = operando1 | operando2;
    break;
  }
  if(resultado_ula.UlaSaida >127 || resultado_ula.UlaSaida<-128){
    resultado_ula.SinalOverflow = 1;
  }else{
    resultado_ula.SinalOverflow = 0;
  }
  if (resultado_ula.UlaSaida == 0){
    resultado_ula.UlaZero = 1;
  }else if (resultado_ula.UlaSaida != 0){
    resultado_ula.UlaZero = 0;
  }
  return resultado_ula;
}

int BinParaDec(char binario[]){
    int resultado;
    int tam_bin = strlen(binario);
    if(binario[0] == '1'){
        resultado = strtol(binario, NULL, 2) - pow(2,tam_bin);
    }else{
        resultado = strtol(binario, NULL, 2);
    }
    return resultado;
}

void identificainstrucao(Multiciclo *multiciclo) {
  if (multiciclo->IR.op == 0) {
    if (multiciclo->IR.funct == 0) {
      printf("ADD $%d, $%d, $%d\n", multiciclo->IR.rd, multiciclo->IR.rs,
             multiciclo->IR.rt);
    }
    if (multiciclo->IR.funct == 2) {
      printf("SUB $%d, $%d, $%d\n", multiciclo->IR.rd, multiciclo->IR.rs,
             multiciclo->IR.rt);
    }
    if (multiciclo->IR.funct == 4) {
      printf("AND $%d, $%d, $%d\n", multiciclo->IR.rd, multiciclo->IR.rs,
             multiciclo->IR.rt);
    }
    if (multiciclo->IR.funct == 5) {
      printf("OR $%d, $%d, $%d\n", multiciclo->IR.rd, multiciclo->IR.rs,
             multiciclo->IR.rt);
    }
  }
  if (multiciclo->IR.op == 4) {
    printf("ADDI $%d, $%d, %d\n", multiciclo->IR.rt, multiciclo->IR.rs,
           multiciclo->IR.imm);
  }
  if (multiciclo->IR.op == 2) {
    printf("J %d\n", multiciclo->IR.end);
  }
  if(multiciclo->IR.op == 8){
      printf("BEQ $%d $%d %d \n",multiciclo->IR.rt, multiciclo->IR.rs, multiciclo->IR.imm);
  }
  if(multiciclo->IR.op == 11){
    printf("LW $%d %d($%d)\n",multiciclo->IR.rt,multiciclo->IR.imm,multiciclo->IR.rs);
  }
  if(multiciclo->IR.op== 15){
    printf("SW $%d %d($%d)\n",multiciclo->IR.rt,multiciclo->IR.imm,multiciclo->IR.rs);
  }
}

void Ler_Arquivo(Memoria *memoria, int *qtd_instrucoes, int *qtd_dados) {
    char nome_arquivo[100];
    char auxiliar[22];
    char auxiliar_end[10];
    char auxiliar_bin[9]; // Para os 8 últimos bits
    char line[Tam_Instrucao + 10]; // espaço extra para o .data
    int cont_inst = 0;
    int cont_dados = 0;
    int flag_data = 0;

    printf("Digite o nome do arquivo a ser carregado (.mem): ");
    scanf("%99s", nome_arquivo);

    FILE *file = fopen(nome_arquivo, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0; // remove \n e \r

        if (strlen(line) == 0){
            continue;
        }
        if (strcmp(line, ".data") == 0) {
            flag_data = 1;
            continue;
        }
        
        if (flag_data == 0) {
            // Instruções
            strncpy(memoria[cont_inst].binario, line, 16);
            memoria[cont_inst].binario[16] = '\0';
            Decodicador(&memoria[cont_inst]);
            cont_inst++;
        } else {
            // Dados
            strncpy(auxiliar, line, sizeof(auxiliar));
            auxiliar[sizeof(auxiliar) - 1] = '\0';

            int cont_data = 0;
            while (auxiliar[cont_data] != ':' && auxiliar[cont_data] != '\0') {
                cont_data++;
            }

            //Cata o endereço
            strncpy(auxiliar_end, auxiliar, cont_data);
            auxiliar_end[cont_data] = '\0';
            int endereco = (int)strtol(auxiliar_end, NULL, 10);
            if (endereco < 128 || endereco >= 256) {
                printf("Erro: endereço fora do intervalo (128-255): %d\n", endereco);
                continue;
                }
                
            // Separar os últimos 8 bits do binário
            char *bin_total = auxiliar + cont_data + 1;
            int tam_total = strlen(bin_total);

            strncpy(auxiliar_bin, bin_total + (tam_total - 8), 8);
            auxiliar_bin[8] = '\0';

            memoria[endereco].dado = BinParaDec(auxiliar_bin);
            cont_dados++;
        }
    }

    fclose(file);

    printf("Arquivo carregado com sucesso. %d instruções carregadas.\n"
           "Dados carregados com sucesso, %d carregados\n",
           cont_inst, cont_dados);

    *qtd_instrucoes = cont_inst;
    *qtd_dados = cont_dados;
}

void step(Memoria *memoria, Multiciclo *multiciclo){
  if (multiciclo->Parada_Estado == multiciclo->Estado){
      printf("Não há o que rodar...\n");
    }
  else{
    multiciclo->Parada_Estado = multiciclo->Estado;
    controle(memoria, multiciclo, multiciclo->Estado);
  }
}

void run(Memoria *memoria, Multiciclo *multiciclo){
  multiciclo->Parada_Estado = -1;
  for(int i = 0;i<200;i++){
    if (multiciclo->Parada_Estado == multiciclo->Estado){
      break;
    }
    multiciclo->Parada_Estado = multiciclo->Estado;
    step(memoria, multiciclo);
  }
}

void controle(Memoria *instrucao, Multiciclo *multiciclo, int estado) {
  char auxiliartipo[4];
  //Ula Auxiliar_Ula;
  //Auxiliar para separar o binário em partes
  char auxiliar[20];
  switch (estado) {

  case 0: // Busca
    multiciclo->IR = instrucao[multiciclo->pc];
    printf("\n\n=====Busca=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    multiciclo->Ula = ULA(0, multiciclo->pc, 1);
    multiciclo->pc = multiciclo->Ula.UlaSaida;
    multiciclo->Estado = 1;
    //printf("PC: %d\n", multiciclo->pc); printar pc+1
    break;

  case 1: // Decodificação
    printf("\n\n=====Decodificação=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    // Verifica o tipo da instrução
    strncpy(auxiliartipo, multiciclo->IR.binario, 4);
    auxiliartipo[4] = '\0';
    multiciclo->IR.op = strtol(auxiliartipo, NULL, 2);

    if (strtol(multiciclo->IR.binario, NULL, 2) == 0) {
      printf("NOP\n");
      break;
    }

    else if (multiciclo->IR.op == 0) { //Tipo R
      multiciclo->Estado = 7;
    } 
    else if (multiciclo->IR.op == 2) { //Tipo J
      multiciclo->Estado = 10;
      
    }else if(multiciclo->IR.op==8){ //Tipo I (BEQ)
      multiciclo->Estado = 9;
      
    }else if (multiciclo->IR.op != 0 && multiciclo->IR.op != 2) { //Tipo I (ADDI, SW, LW)
      multiciclo->Estado = 2;
    }
    multiciclo->Ula = ULA(0, multiciclo->pc, multiciclo->IR.imm);
    printf("UlaSaida: %d\n", multiciclo->Ula.UlaSaida);
    break;

  case 2: //Identifica qual das 3:   ADDI,SW ou LW
    printf("\n\n=====Identifica ADDI, SW, Lw=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    multiciclo->Ula = ULA(0, multiciclo->registradores[multiciclo->IR.rs], multiciclo->IR.imm);
    printf("%d + %d = %d\n", multiciclo->registradores[multiciclo->IR.rs], multiciclo->IR.imm, multiciclo->Ula.UlaSaida);
    if(multiciclo->IR.op == 4){
        multiciclo->Estado = 6;
    }else if(multiciclo->IR.op == 11){//lW provisório
        multiciclo->Estado = 3;
    }else if(multiciclo->IR.op == 15){//SW
        multiciclo->Estado = 5;
    }
    break;
  case 3:
    printf("\n\n=====Acesso a Memória LW=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    multiciclo->Regis_Inter = instrucao[multiciclo->Ula.UlaSaida].dado;
    multiciclo->Estado = 4;
    break;
  case 4:
    printf("\n\n=====Escrita no Rt=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    identificainstrucao(multiciclo);
    multiciclo->registradores[multiciclo->IR.rt] =  multiciclo->Regis_Inter;
    multiciclo->Estado = 0;
    break;
  case 5:
    //A ideia principal
    printf("\n\n=====Acesso a Memória SW=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    identificainstrucao(multiciclo);
    instrucao[multiciclo->Ula.UlaSaida].dado = multiciclo->registradores[multiciclo->IR.rt];
    multiciclo->Estado = 0;
    break;
    
  case 6: //Execução ADDI
    printf("\n\n=====Execução ADDI=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    identificainstrucao(multiciclo);
    printf("UlaSaida: %d\n", multiciclo->Ula.UlaSaida);
    multiciclo->registradores[multiciclo->IR.rt] = multiciclo->Ula.UlaSaida;
    multiciclo->Estado = 0;
    break;

  case 7: //Execução Tipo R
    printf("\n\n=====Execução Tipo R=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    identificainstrucao(multiciclo);
    multiciclo->Ula = ULA(multiciclo->IR.funct, multiciclo->registradores[multiciclo->IR.rs],
        multiciclo->registradores[multiciclo->IR.rt]);
    multiciclo->registradores[multiciclo->IR.rd] = multiciclo->Ula.UlaSaida;
    multiciclo->Estado = 0;
    break;
  
  case 9://Executar BEQ
    printf("\n\n=====Execução BEQ=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    identificainstrucao(multiciclo);
    multiciclo->Regis_Inter = multiciclo->Ula.UlaSaida;
    multiciclo->Ula = ULA(2, multiciclo->registradores[multiciclo->IR.rs], multiciclo->registradores[multiciclo->IR.rt]);
    if(multiciclo->Ula.UlaZero == 1){
        //multiciclo->pc = multiciclo->IR.imm;
        multiciclo->pc = multiciclo->Regis_Inter;
        multiciclo->Estado = 0;
        printf("Ocorreu BEQ\n");
        break;
    }
    printf("Não Ocorreu BEQ\n");
    multiciclo->Estado = 0;
    break;

  case 10: //Execução Jump
    printf("\n\n=====Execução J=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    identificainstrucao(multiciclo);
    multiciclo->pc = multiciclo->IR.end ;
    multiciclo->Estado = 0;
    break;
  }
}

void imprimememoria(Memoria *memoria){
  printf("==========Memoria===========\n");
  for(int i = 0;i<Tam_Memoria;i++){
    if(i < 127){
        printf("Instrução[%i]: %s\n",i,memoria[i].binario);
    }else if(i>=128){
        printf("Dado[%i]: %i\n",i,memoria[i].dado);
    }
  }
  printf("============================\n");
}

int main() {

  Multiciclo *multiciclo = malloc(sizeof(Multiciclo));
  multiciclo->pc = 0;
  multiciclo->Estado = 0;
  multiciclo->Parada_Estado = -1;
  int cont_inst = 0;
  int cont_dados = 0;

  for (int i = 0; i < NUM_REGISTRADORES; i++) {
    multiciclo->registradores[i] = 0;
  }

  Memoria *memoria = malloc(sizeof(Memoria)*Tam_Memoria);

  for (int i=0; i<Tam_Memoria;i++){
    strcpy(memoria[i].binario, "0000000000000000");
    memoria[i].dado = 0;
  }

  int op_menuprincipal = 1;
  while(op_menuprincipal != 0){
    printf("\n\n|=====================Menu Principal=====================|\n|0. Sair                                                 |\n|1. Carregar memória de instruções (.mem)                |\n|2. Imprimir memórias (instruções e dados)               |\n|3. Imprimir banco de registradores                      |\n|4. Imprimir todo o simulador (registradores e memórias) |\n|5. Salvar .asm                                          |\n|6. Salvar .dat                                          |\n|7. Executa Programa (run)                               |\n|8. Executa uma instrução (Step)                         |\n|9. Volta uma instrução (Back)                          |\n|========================================================|\nEscolha uma opção:\n");
    scanf("%d", &op_menuprincipal);
    switch(op_menuprincipal){
        default:
          printf("Opção Inválida...\n");
        break;
        case 0:
          printf("Saindo...");
        break;
        case 1:
          Ler_Arquivo(memoria, &cont_inst, &cont_dados);
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
          //printf("Estado: %i\n",multiciclo->Estado);
          printf("PC: %d\n", multiciclo->pc);
        break;
        case 5:
        //ASM();
        break;
        case 6:
        //GerarMEM();
        break;
        case 7:
          run(memoria, multiciclo);
        break;
        case 8:
          step(memoria, multiciclo);
        break;
        case 9:
        break;
    }
  }
  return 0;
}

