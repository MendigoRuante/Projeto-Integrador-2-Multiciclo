#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Tam_Instrucao 17    // 16 bits + 1 para o \0
#define NUM_REGISTRADORES 8 // Numero de Registradores, 3 bits = 8
#define Tam_Memoria 256 // Limite da Memória

typedef enum { TipoR, TipoI, TipoJ, Outro } TipoInstrucao;

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
  int Memoria_Dados[Tam_Memoria];
  Memoria IR;
  Ula Ula;
  //Para Teste
  int Estado;
} Multiciclo;

// FUNÇÕES
void controle(Memoria *instrucao, Multiciclo *multiciclo, int estado);

void imprimir_registradores(Multiciclo *multiciclo) {
  printf("=====Banco de Registradores=====\n");
  for (int i = 0; i < NUM_REGISTRADORES; i++) {
    printf("R%d: %d\n", i, multiciclo->registradores[i]);
  }
  printf("================================\n");
}

void ULA(int ulaop, int operando1, int operando2, Multiciclo *multiciclo) {
  multiciclo->Ula.Ula_A = operando1;
  multiciclo->Ula.Ula_B = operando2;
  int resultado;
  switch (ulaop) {
    case 0: //ADD
      resultado = multiciclo->Ula.Ula_A + multiciclo->Ula.Ula_B;
    break;
    case 2: //SUB
      resultado = multiciclo->Ula.Ula_A - multiciclo->Ula.Ula_B;
    break;
    case 3: //AND
      resultado = multiciclo->Ula.Ula_A & multiciclo->Ula.Ula_B;
    break;
    case 4: //OR
      resultado = multiciclo->Ula.Ula_A | multiciclo->Ula.Ula_B;
    break;
  }
  if(resultado >127 || resultado<-128){
    multiciclo->Ula.SinalOverflow = 1;
  }else{
    multiciclo->Ula.SinalOverflow = 0;
  }
  if (resultado == 0){
    multiciclo->Ula.UlaZero = 1;
  }else if (resultado != 0){
    multiciclo->Ula.UlaZero = 0;
  }
  multiciclo->Ula.UlaSaida = resultado;
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
      printf("BEQ $%d, $%d, %d \n",multiciclo->IR.rt, multiciclo->IR.rs, multiciclo->IR.imm);
  }
  if(multiciclo->IR.op == 11){
    printf("LW $%d, %d($%d)\n",multiciclo->IR.rt,multiciclo->IR.imm,multiciclo->IR.rs);
  }
  if(multiciclo->IR.op== 15){
    printf("SW $%d, %d($%d)\n",multiciclo->IR.rt,multiciclo->IR.imm,multiciclo->IR.rs);
  }
}

void Ler_Arquivo(Memoria *memoria, int *qtd_instrucoes, int *qtd_dados) {
  char nome_arquivo[100];
  char line[Tam_Instrucao + 1];
  int cont_inst = 0;
  int cont_dados = 0;

  printf("Digite o nome do arquivo a ser carregado (.mem): ");
  scanf("%99s", nome_arquivo);

  FILE *file = fopen(nome_arquivo, "r");
  if (!file) {
    perror("Erro ao abrir o arquivo");
    return;
  }

  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\r\n")] = 0;
    if (strlen(line) == 0) {
      continue;
    }
    if (strlen(line) == 16) {
      strncpy(memoria[cont_inst].binario, line, 16);
      cont_inst++;

    }else if(strlen(line) == 8) {
        char auxliar[8];
        strncpy(auxliar, line, 8);
        auxliar[8] = '\0';
        if(auxliar[0] == '1'){
            memoria[cont_dados].dado = BinParaDec(auxliar);
            cont_dados++;
        }else{
            memoria[cont_dados].dado = BinParaDec(auxliar);
            cont_dados++;
    }
  }
}
  fclose(file);
  printf("Arquivo carregado com sucesso. %d instruções carregadas.\nDados "
         "carregados com sucesso, %d carregados\n",
         cont_inst, cont_dados);
  *qtd_instrucoes = cont_inst;
  *qtd_dados = cont_dados;
}

void step(Memoria *memoria, Multiciclo *multiciclo){
  if (multiciclo->Estado == 0){
    printf("\n==========Início instrução==========\n");
  }
  controle(memoria, multiciclo, multiciclo->Estado);
  //printf("Fim instrução\n\n");
}

void run(Memoria *memoria, Multiciclo *multiciclo, int cont_inst){
  int auxparada = -1;
  for(int i = 0;i<200;i++){
    if (auxparada == multiciclo->Estado){
      break;
    }
    auxparada = multiciclo->Estado;
    step(memoria, multiciclo);
  }
}

void controle(Memoria *instrucao, Multiciclo *multiciclo, int estado) {
  int rs, rt;
  char auxiliartipo[4];
  //Auxiliar para separar o binário em partes
  char auxiliar[20];
  switch (estado) {

  case 0: // Busca
    //printf("PC: %d\n", multiciclo->pc);
    multiciclo->IR = instrucao[multiciclo->pc];
    ULA(0, multiciclo->pc, 1, multiciclo);
    multiciclo->pc = multiciclo->Ula.UlaSaida;
    multiciclo->Estado = 1;
    printf("IR: %s\n", multiciclo->IR.binario);
    //printf("PC: %d\n", multiciclo->pc);
    break;

  case 1: // Decodificação
    // Verifica o tipo da instrução
    strncpy(auxiliartipo, multiciclo->IR.binario, 4);
    auxiliartipo[4] = '\0';
    multiciclo->IR.op = strtol(auxiliartipo, NULL, 2);

    if (strtol(multiciclo->IR.binario, NULL, 2) == 0) {
      printf("NOP\n");
    }

    else if (multiciclo->IR.op == 0) { //Tipo R
      //printf("Instrução Tipo R\n");
      multiciclo->IR.tipo = TipoR;
       //Separa o binário em partes
      // OP
      strncpy(auxiliar, multiciclo->IR.binario, 4);
      auxiliar[4] = '\0';
      multiciclo->IR.op = strtol(auxiliar, NULL, 2);
      // RS
      strncpy(auxiliar, multiciclo->IR.binario + 4, 3);
      auxiliar[3] = '\0';
      multiciclo->IR.rs = strtol(auxiliar, NULL, 2);
      // RT
      strncpy(auxiliar, multiciclo->IR.binario + 7, 3);
      auxiliar[3] = '\0';
      multiciclo->IR.rt = strtol(auxiliar, NULL, 2);
      // RD
      strncpy(auxiliar, multiciclo->IR.binario + 10, 3);
      auxiliar[3] = '\0';
      multiciclo->IR.rd = strtol(auxiliar, NULL, 2);
      //Funct
      strncpy(auxiliar, multiciclo->IR.binario + 13, 3);
      auxiliar[3] = '\0';
      multiciclo->IR.funct = strtol(auxiliar, NULL, 2);

      //printf("OP: %d\n", multiciclo->IR.op);
      //printf("RS: %d\n", multiciclo->IR.rs);
      //printf("RT: %d\n", multiciclo->IR.rt);
      //printf("RD: %d\n", multiciclo->IR.rd);
      //printf("FUNCT: %d\n", multiciclo->IR.funct);
      multiciclo->Estado = 7;
    } 
    else if (multiciclo->IR.op == 2) { //Tipo J
      //printf("Instrução Tipo J\n");
      multiciclo->IR.tipo = TipoJ;
      // OP
      strncpy(auxiliar, multiciclo->IR.binario, 4);
      auxiliar[4] = '\0';
      multiciclo->IR.op = strtol(auxiliar, NULL, 2);
      // END
      strncpy(auxiliar, multiciclo->IR.binario + 4, 12);
      auxiliar[12] = '\0';
      multiciclo->IR.end = strtol(auxiliar, NULL, 2);


      //printf("OP: %d\n", multiciclo->IR.op);
      //printf("END: %d\n", multiciclo->IR.end);
      multiciclo->Estado = 10;
      
    }else if(multiciclo->IR.op==8){ //Tipo I (BEQ)
      //printf("Instrução Tipo I\n");
      multiciclo->IR.tipo = TipoI;
      // OP
      strncpy(auxiliar, multiciclo->IR.binario, 4);
      auxiliar[4] = '\0';
      multiciclo->IR.op = strtol(auxiliar, NULL, 2);
      // RS
      strncpy(auxiliar, multiciclo->IR.binario + 4, 3);
      auxiliar[3] = '\0';
      multiciclo->IR.rs = strtol(auxiliar, NULL, 2);
      // RT
      strncpy(auxiliar, multiciclo->IR.binario + 7, 3);
      auxiliar[3] = '\0';
      multiciclo->IR.rt = strtol(auxiliar, NULL, 2);
      // IMM
      strncpy(auxiliar, multiciclo->IR.binario + 10, 6);
      auxiliar[6] = '\0';
      multiciclo->IR.imm = strtol(auxiliar, NULL, 2);
      
      //printf("OP: %d\n", multiciclo->IR.op);
      //printf("RS: %d\n", multiciclo->IR.rs);
      //printf("RT: %d\n", multiciclo->IR.rt);
      //printf("IMM: %d\n", multiciclo->IR.imm);
      multiciclo->Estado = 9;
      
    }else if (multiciclo->IR.op != 0 && multiciclo->IR.op != 2) { //Tipo I (ADDI, SW, LW)
      //printf("Instrução Tipo I\n");
      multiciclo->IR.tipo = TipoI;
      // OP
      strncpy(auxiliar, multiciclo->IR.binario, 4);
      auxiliar[4] = '\0';
      multiciclo->IR.op = strtol(auxiliar, NULL, 2);
      // RS
      strncpy(auxiliar, multiciclo->IR.binario + 4, 3);
      auxiliar[3] = '\0';
      multiciclo->IR.rs = strtol(auxiliar, NULL, 2);
      // RT
      strncpy(auxiliar, multiciclo->IR.binario + 7, 3);
      auxiliar[3] = '\0';
      multiciclo->IR.rt = strtol(auxiliar, NULL, 2);
      // IMM
      strncpy(auxiliar, multiciclo->IR.binario + 10, 6);
      auxiliar[6] = '\0';
      multiciclo->IR.imm = BinParaDec(auxiliar);


      //printf("OP: %d\n", multiciclo->IR.op);
      //printf("RS: %d\n", multiciclo->IR.rs);
      //printf("RT: %d\n", multiciclo->IR.rt);
      //printf("IMM: %d\n", multiciclo->IR.imm);
      multiciclo->Estado = 2;
    }
    ULA(0, multiciclo->pc, multiciclo->IR.imm, multiciclo);
    //printf("UlaSaida: %d\n", multiciclo->UlaSaida);
    break;

  case 2: //Identifica qual das 3:   ADDI,SW ou LW
    ULA(0, multiciclo->registradores[multiciclo->IR.rs], multiciclo->IR.imm, multiciclo);
    if(multiciclo->IR.op == 4){
        multiciclo->Estado = 6;
    }else if(multiciclo->IR.op == 11){//lW provisório
        multiciclo->Estado = 4;
    }else if(multiciclo->IR.op == 15){//SW
        multiciclo->Estado = 5;
    }
    break;
  case 3:
    //Ainda a pensar
    break;
  case 4:
    identificainstrucao(multiciclo);
    multiciclo->registradores[multiciclo->IR.rt] =  instrucao[multiciclo->Ula.UlaSaida].dado;
    multiciclo->Estado = 0;
    break;
  case 5:
    //A ideia principal
    identificainstrucao(multiciclo);
    instrucao[multiciclo->Ula.UlaSaida].dado = multiciclo->registradores[multiciclo->IR.rt];
    multiciclo->Estado = 0;
    break;
    
  case 6: //Execução ADDI
    identificainstrucao(multiciclo);
    // é feito no case 2ULA(0, multiciclo->registradores[multiciclo->IR.rs], multiciclo->UlaSaida,multiciclo);
    multiciclo->registradores[multiciclo->IR.rt] = multiciclo->Ula.UlaSaida;
    multiciclo->Estado = 0;
    break;

  case 7: //Execução Tipo R
    identificainstrucao(multiciclo);
    ULA(multiciclo->IR.funct, multiciclo->registradores[multiciclo->IR.rs],
        multiciclo->registradores[multiciclo->IR.rt], multiciclo);
    multiciclo->registradores[multiciclo->IR.rd] = multiciclo->Ula.UlaSaida;
    multiciclo->Estado = 0;
    break;
  
  case 9://Executar BEQ
    identificainstrucao(multiciclo);
    int aux = multiciclo->Ula.UlaSaida;
    ULA(2, multiciclo->registradores[multiciclo->IR.rs], multiciclo->registradores[multiciclo->IR.rt], multiciclo);
    if(multiciclo->Ula.UlaZero == 1){
        //multiciclo->pc = multiciclo->IR.imm;
        multiciclo->pc = aux;
        multiciclo->Estado = 0;
        printf("Ocorreu BEQ\n");
        break;
    }
    multiciclo->Estado = 0;
    break;

  case 10: //Execução Jump
    identificainstrucao(multiciclo);
    multiciclo->pc = multiciclo->IR.end ;
    multiciclo->Estado = 0;
    break;
  }
}

int main() {
  Multiciclo *multiciclo = malloc(sizeof(Multiciclo));
  multiciclo->pc = 0;
  multiciclo->Estado = 0;
  int cont_inst = 0;
  int cont_dados = 0;

  for (int i = 0; i < NUM_REGISTRADORES; i++) {
    multiciclo->registradores[i] = 0;
  }

  multiciclo->registradores[1] = 20;
  multiciclo->registradores[2] = 30;
  multiciclo->registradores[3] = 0;

  imprimir_registradores(multiciclo);
  Memoria *memoria = malloc(sizeof(Memoria)*Tam_Memoria);

  for (int i=0; i<Tam_Memoria;i++){
    strcpy(memoria[i].binario, "0000000000000000");
    memoria[i].dado = 0;
  }

  Ler_Arquivo(memoria, &cont_inst, &cont_dados);

  run(memoria, multiciclo, cont_inst);

  imprimir_registradores(multiciclo);
  printf("PC Final: %d\n", multiciclo->pc);
  
  printf("==========Memoria===========\n");
    for(int i = 0;i<Tam_Memoria;i++){
      printf("Instrução[%i] = %s  Dado[%i] = %i \n",i,memoria[i].binario,i,memoria[i].dado);
    //  if(memoria[i].dado == 0){
    //    printf("[%i] = %s\n",i,memoria[i].binario);
    //  }
    //  else{
    //    printf("[%i] = %i\n",i,memoria[i].dado);
    //  }
    }
  printf("============================\n");
  return 0;
}