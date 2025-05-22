#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TAD.h"

//Identifica as instruções e preenche os campos relevantes para cada instrução
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
//Printa os registradores
void imprimir_registradores(Multiciclo *multiciclo) {
  printf("=====Banco de Registradores=====\n");
  for (int i = 0; i < NUM_REGISTRADORES; i++) {
    printf("R%d: %d\n", i, multiciclo->registradores[i]);
  }
  printf("================================\n");
}
//Ula que dá os retornos
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
  if(resultado_ula.UlaSaida > 256 || resultado_ula.UlaSaida<-128){
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
//Complemento de 2
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
//Identifica a instrução atual durante a execução
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
//Recebe o .mem
void Ler_Arquivo(Memoria *memoria, int *qtd_instrucoes) {
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

            //Guardando o dado em binário e decimal e Definindo o tipo
            strncpy(memoria[endereco].binario, line, 16);
            memoria[endereco].dado = BinParaDec(auxiliar_bin);
            memoria[endereco].tipo = Dado;
            cont_dados++;
        }
    }

    fclose(file);

    printf("Arquivo carregado com sucesso. %d instruções carregadas.\n"
           "Dados carregados com sucesso, %d carregados\n",
           cont_inst, cont_dados);

    *qtd_instrucoes = cont_inst;
}
//STEP
void step(Memoria *memoria, Multiciclo *multiciclo){
  if (multiciclo->Parada_Estado == multiciclo->Estado){
      printf("Não há o que rodar...\n");
    }
  else{
    multiciclo->Parada_Estado = multiciclo->Estado;
    controle(memoria, multiciclo, multiciclo->Estado);
  }
}
//Controle
void controle(Memoria *memoria, Multiciclo *multiciclo, int estado) {
  char auxiliartipo[4];
  int Endereco_BEQ;
  //Auxiliar para separar o binário em partes
  char auxiliar[20];
  switch (estado) {

  case 0: // Busca
    multiciclo->IR = memoria[multiciclo->pc];
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
    multiciclo->Regis_Inter = memoria[multiciclo->Ula.UlaSaida].dado;
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
    memoria[multiciclo->Ula.UlaSaida].dado = multiciclo->registradores[multiciclo->IR.rt];
    multiciclo->Estado = 0;
    break;
    
  case 6: //Execução ADDI
    printf("\n\n=====Execução ADDI=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    identificainstrucao(multiciclo);
    printf("UlaSaida: %d\n", multiciclo->Ula.UlaSaida);
    if(multiciclo->Ula.SinalOverflow == 1){
      printf("Overflow");
    }
    multiciclo->registradores[multiciclo->IR.rt] = multiciclo->Ula.UlaSaida;
    multiciclo->Estado = 0;
    break;

  case 7: //Execução Tipo R
    printf("\n\n=====Execução Tipo R=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    identificainstrucao(multiciclo);
    multiciclo->Ula = ULA(multiciclo->IR.funct, multiciclo->registradores[multiciclo->IR.rs],multiciclo->registradores[multiciclo->IR.rt]);
        multiciclo->Estado = 8;
    break;
  case 8:
    printf("\n\n=====Término da Tipo R=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    if(multiciclo->Ula.SinalOverflow == 1){
      printf("Overflow");
    }
    multiciclo->registradores[multiciclo->IR.rd] = multiciclo->Ula.UlaSaida;
    multiciclo->Estado = 0;
    break;
  
  case 9://Executar BEQ
    printf("\n\n=====Execução BEQ=====\nIR: %s\nEstado: %i\n", multiciclo->IR.binario, multiciclo->Estado);
    identificainstrucao(multiciclo);
    Endereco_BEQ = multiciclo->Ula.UlaSaida;
    multiciclo->Ula = ULA(2, multiciclo->registradores[multiciclo->IR.rs], multiciclo->registradores[multiciclo->IR.rt]);
    if(multiciclo->Ula.UlaZero == 1){
        //multiciclo->pc = multiciclo->IR.imm;
        multiciclo->pc = Endereco_BEQ;
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
//Printa memória
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
//Funções da Pilha
//Empilhar estado
void empilhar(No **topo, Multiciclo* multiciclo) {
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->multiciclo = malloc(sizeof(Multiciclo));
    if (novoNo == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }
    novoNo->multiciclo->pc = multiciclo->pc;
    novoNo->multiciclo->Estado = multiciclo->Estado;
    strcpy(novoNo->multiciclo->IR.binario,multiciclo->IR.binario);
    for(int i = 0;i<8;i++){
      novoNo->multiciclo->registradores[i] = multiciclo->registradores[i];
    }
    novoNo->multiciclo->Ula = multiciclo->Ula;
    novoNo->multiciclo->Regis_Inter = multiciclo->Regis_Inter;
    novoNo->multiciclo->Parada_Estado = multiciclo->Parada_Estado;
    novoNo->proximo = *topo;
    *topo = novoNo;
}
// Desempilha estado
void desempilhar(No **topo, Multiciclo* multiciclo) {
    if (*topo == NULL) {
        printf("A pilha está vazia.\n");
         // Valor indicativo de erro
        return;
    }
    printf("Voltando um estado...\n");
    No *temp = *topo;
    multiciclo->pc = temp->multiciclo->pc;
    multiciclo->Estado = temp->multiciclo->Estado;
    strcpy(multiciclo->IR.binario,temp->multiciclo->IR.binario);
    multiciclo->Regis_Inter = temp->multiciclo->Regis_Inter;
    multiciclo->Ula = temp->multiciclo->Ula;
    multiciclo->Parada_Estado = temp->multiciclo->Parada_Estado;
    for (int i = 0; i < 8; i++) {
      multiciclo->registradores[i] = temp->multiciclo->registradores[i];
    }
    
    *topo = temp->proximo;
    free(temp);
}
//Função auxiliar para gerar o .mem
void converterDadoParaBinario(Memoria *mem) {
    unsigned char valor = (unsigned char)mem->dado; 

    // Inicializa os primeiros 8 bits como '0'
    for (int i = 0; i < 16; i++) {
        if (i < 8) {
            mem->binario[i] = '0'; // Bits mais altos zerados 
        } else {
            mem->binario[i] = '0'; // Inicializa todos os bits com 0
        }
    }

    int posicao = 7;
    while (posicao >= 0) {
        if (valor % 2 == 1) {
            mem->binario[8 + posicao] = '1'; // Preenche os últimos 8 bits
        }
        valor = valor / 2;
        posicao--;
    }

    mem->binario[16] = '\0'; // Fecha a string
}
//Função para gerar o .mem
void GerarMEM(Memoria *memoria){
    FILE *arquivo = fopen("memoria.mem", "w");
    if (!arquivo) {
        perror("Erro ao criar arquivo .mem");
        return;
    }
    
    for(int i = 0;i<= 127;i++){
        if(memoria[i].tipo != NOP){
            fprintf(arquivo,"%s\n",memoria[i].binario);
        }
    }
    fprintf(arquivo,".data\n");
    for(int i = 128;i <= 256; i++){
        if(memoria[i].dado != 0){
            converterDadoParaBinario(&memoria[i]);
            fprintf(arquivo,"%i:%s\n",i,memoria[i].binario);
        }
    }
    printf(".mem criado com sucesso!\n");
    fclose(arquivo);
}

void ASM(Memoria *instrucoes, int cont){
    
    FILE *arquivo = fopen("Arquivo_ASM.asm", "w");

    if (!arquivo) {
        perror("Erro ao criar arquivo .asm");
        return;
    }

    for (int i = 0; i < cont; i++) {
        Memoria instrucao = instrucoes[i];

        switch (instrucao.tipo) {
            case TipoR:
                switch (instrucao.funct) {
                    case 0: // ADD
                        fprintf(arquivo, "add $%d, $%d, $%d\n", instrucao.rd, instrucao.rs, instrucao.rt);
                        break;
                    case 2: // SUB
                        fprintf(arquivo, "sub $%d, $%d, $%d\n", instrucao.rd, instrucao.rs, instrucao.rt);
                        break;
                    case 4: // AND
                        fprintf(arquivo, "and $%d, $%d, $%d\n", instrucao.rd, instrucao.rs, instrucao.rt);
                        break;
                    case 5: // OR
                        fprintf(arquivo, "or $%d, $%d, $%d\n", instrucao.rd, instrucao.rs, instrucao.rt);
                        break;
                    default:
                        fprintf(arquivo, "funct invalido: %d\n", instrucao.funct);
                        break;
                }
                break;

            case TipoI:
                switch (instrucao.op) {
                    case 4: // ADDI
                        fprintf(arquivo, "addi $%d, $%d, %d\n", instrucao.rt, instrucao.rs, instrucao.imm);
                        break;
                    case 8: // BEQ
                        fprintf(arquivo, "beq $%d, $%d, %d\n", instrucao.rs, instrucao.rt, instrucao.imm);
                        break;
                    case 11: // LW
                        fprintf(arquivo, "lw $%d, %d($%d)\n", instrucao.rt, instrucao.imm, instrucao.rs);
                        break;
                    case 15: // SW
                        fprintf(arquivo, "sw $%d, %d($%d)\n", instrucao.rt, instrucao.imm, instrucao.rs);
                        break;
                    default:
                        fprintf(arquivo, "opcode invalido: %d\n", instrucao.op);
                        break;
                }
                break;

            case TipoJ:
                switch (instrucao.op) {
                    case 2: // JUMP
                        fprintf(arquivo, "j %d\n", instrucao.end);
                        break;
                    default:
                        fprintf(arquivo, "opcode invalido: %d\n", instrucao.op);
                        break;
                }
                break;

            default:
                fprintf(arquivo, "Instrucao invalida\n");
                break;
        }
    }

    fclose(arquivo);
    printf("Arquivo .asm salvo com sucesso!\n");
}