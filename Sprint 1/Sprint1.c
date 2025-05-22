#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 

#define Num_Instrucoes 256 // Limite
#define Tam_Instrucao 17 // 16 bits + 1 para o \0
#define NUM_REGISTRADORES 8 // Numero de Registradores, 3 bits = 8
#define Tam_Memoria 256

typedef enum {
    TipoR,
    TipoI,
    TipoJ,
    Outro
} TipoInstrucao;

typedef struct {
    char binario[17];
    int op, rs, rt, rd, imm, funct, end;
    char sop[5], srs[4], srt[4], srd[4], simm[7], sfunct[4], send[13];
    TipoInstrucao tipo;
} Memoria;

typedef struct {
    int pc;
    int registradores[NUM_REGISTRADORES];
    int Memoria_Dados[Tam_Memoria];
    int UlaZero;
    int SinalOverflow;
    Memoria IR;
    int UlaSaida;
} Multiciclo;

//FUNÇÕES
void imprimir_registradores(Multiciclo *multiciclo);
void ULA(int ulaop, int operando1, int operando2, Multiciclo *multiciclo);
void controle(Memoria *instrucao, Multiciclo *multiciclo, int estado);
void identificainstrucao(Multiciclo *multiciclo);

int main() {
    Multiciclo *multiciclo = malloc(sizeof(Multiciclo));
    multiciclo->pc = 0;

    for(int i=0; i<NUM_REGISTRADORES;i++){
        multiciclo->registradores[i] = 0;
    }

    multiciclo->registradores[1] = 20;
    multiciclo->registradores[2] = 30;
    multiciclo->registradores[3] = 9;

    imprimir_registradores(multiciclo);
    Memoria instrucoes[Num_Instrucoes]; //por enquanto só instruções

    //ADD $3, $2, $1
    strcpy(instrucoes[0].binario, "0000010001011000");
    //ADDi $2, $0, 5
    strcpy(instrucoes[1].binario, "0100000010000101");
    //NOP
    strcpy(instrucoes[2].binario, "0000000000000000");
    //J 1
    strcpy(instrucoes[3].binario, "0010000000000001");

    //0 instrucao
    printf("==========BUSCA==========\n");
    controle(instrucoes, multiciclo, 0);

    printf("==========DECODIFICAÇÃO==========\n");
    controle(instrucoes, multiciclo, 1);

    printf("==========EXECUÇÃO==========\n");
    controle(instrucoes, multiciclo, 7);

    //1 instrucao
    printf("==========BUSCA==========\n");
    controle(instrucoes, multiciclo, 0);

    printf("==========DECODIFICAÇÃO==========\n");
    controle(instrucoes, multiciclo, 1);

    printf("==========EXECUÇÃO==========\n");
    controle(instrucoes, multiciclo, 6);

    //2 instrucao
    printf("==========BUSCA==========\n");
    controle(instrucoes, multiciclo, 0);

    printf("==========DECODIFICAÇÃO==========\n");
    controle(instrucoes, multiciclo, 1);

    //3 instrucao
    printf("==========BUSCA==========\n");
    controle(instrucoes, multiciclo, 0);

    printf("==========DECODIFICAÇÃO==========\n");
    controle(instrucoes, multiciclo, 1);

    printf("==========EXECUÇÃO==========\n");
    controle(instrucoes, multiciclo, 10);

    imprimir_registradores(multiciclo);
    printf("PC Final: %d\n", multiciclo->pc);
}

void imprimir_registradores(Multiciclo *multiciclo) {
    printf("=====Banco de Registradores=====\n");
    for (int i = 0; i < NUM_REGISTRADORES; i++) {
        printf("R%d: %d\n", i, multiciclo->registradores[i]);
    }
    printf("================================\n");
}

void ULA(int ulaop, int operando1, int operando2, Multiciclo *multiciclo){
    int resultado;
    switch (ulaop) {
        case 0: //ADD
            resultado = operando1 + operando2;
        break;
    }
    multiciclo->UlaSaida = resultado;
}

void identificainstrucao(Multiciclo *multiciclo){
    if(multiciclo->IR.op == 0){
        if(multiciclo->IR.funct == 0){
            printf("ADD $%d, $%d, $%d\n", multiciclo->IR.rd, multiciclo->IR.rs, multiciclo->IR.rt);
        }
        if(multiciclo->IR.funct == 2){
            printf("SUB $%d, $%d, $%d\n", multiciclo->IR.rd, multiciclo->IR.rs, multiciclo->IR.rt);
        }
        if(multiciclo->IR.funct == 4){
            printf("AND $%d, $%d, $%d\n", multiciclo->IR.rd, multiciclo->IR.rs, multiciclo->IR.rt);
        }
        if(multiciclo->IR.funct == 5){
            printf("OR $%d, $%d, $%d\n", multiciclo->IR.rd, multiciclo->IR.rs, multiciclo->IR.rt);
        }
    }
    if(multiciclo->IR.op == 4){
        printf("ADDI $%d, $%d, %d\n", multiciclo->IR.rt, multiciclo->IR.rs, multiciclo->IR.imm);
    }
    if(multiciclo->IR.op == 2){
        printf("J %d\n", multiciclo->IR.end);
    }
}

void controle(Memoria *instrucao, Multiciclo *multiciclo, int estado){
    int rs, rt;
    char auxiliartipo[4];
    switch(estado){

        case 0: //Busca
            printf("PC: %d\n", multiciclo->pc);
            multiciclo->IR = instrucao[multiciclo->pc];
            ULA(0, multiciclo->pc, 1, multiciclo);
            multiciclo->pc = multiciclo->UlaSaida;
            printf("IR: %s\n", multiciclo->IR.binario);
            printf("PC: %d\n", multiciclo->pc);
        break;

        case 1: //Decodificação
            //Verifica o tipo da instrução
            strncpy(auxiliartipo, multiciclo->IR.binario, 4);
            auxiliartipo[4] = '\0';
            multiciclo->IR.op = strtol(auxiliartipo, NULL, 2);

            if(strtol(multiciclo->IR.binario, NULL, 2) == 0){
                printf("NOP\n");
            }

            else if(multiciclo->IR.op == 0){//Tipo R
                printf("Instrução Tipo R\n");
                multiciclo->IR.tipo = TipoR;

                strncpy(multiciclo->IR.sop, multiciclo->IR.binario, 4);
                multiciclo->IR.sop[4] = '\0';
                strncpy(multiciclo->IR.srs, multiciclo->IR.binario + 4, 3);
                multiciclo->IR.srs[3] = '\0';
                strncpy(multiciclo->IR.srt, multiciclo->IR.binario + 7, 3);
                multiciclo->IR.srt[3] = '\0';
                strncpy(multiciclo->IR.srd, multiciclo->IR.binario + 10, 3);
                multiciclo->IR.srd[3] = '\0';
                strncpy(multiciclo->IR.sfunct, multiciclo->IR.binario + 13, 3);
                multiciclo->IR.sfunct[3] = '\0';

                multiciclo->IR.op = strtol(multiciclo->IR.sop, NULL, 2);
                multiciclo->IR.rs = strtol(multiciclo->IR.srs, NULL, 2);
                multiciclo->IR.rt = strtol(multiciclo->IR.srt, NULL, 2);
                multiciclo->IR.rd = strtol(multiciclo->IR.srd, NULL, 2);
                multiciclo->IR.funct = strtol(multiciclo->IR.sfunct, NULL, 2);

                printf("OP: %d\n", multiciclo->IR.op);
                printf("RS: %d\n", multiciclo->IR.rs);
                printf("RT: %d\n", multiciclo->IR.rt);
                printf("RD: %d\n", multiciclo->IR.rd);
                printf("FUNCT: %d\n", multiciclo->IR.funct);
            }

            else if(multiciclo->IR.op == 2){//Tipo J
                printf("Instrução Tipo J\n");
                multiciclo->IR.tipo = TipoJ;

                strncpy(multiciclo->IR.sop, multiciclo->IR.binario, 4);
                multiciclo->IR.sop[4] = '\0';
                strncpy(multiciclo->IR.send, multiciclo->IR.binario + 4, 12);
                multiciclo->IR.send[12] = '\0';

                multiciclo->IR.op = strtol(multiciclo->IR.sop, NULL, 2);
                multiciclo->IR.end = strtol(multiciclo->IR.send, NULL, 2);

                printf("OP: %d\n", multiciclo->IR.op);
                printf("END: %d\n", multiciclo->IR.end);
            }

            else if(multiciclo->IR.op != 0 && multiciclo->IR.op != 2){//Iipo I
                printf("Instrução Tipo I\n");
                multiciclo->IR.tipo = TipoI;

                strncpy(multiciclo->IR.sop, multiciclo->IR.binario, 4);
                multiciclo->IR.sop[4] = '\0';
                strncpy(multiciclo->IR.srs, multiciclo->IR.binario + 4, 3);
                multiciclo->IR.srs[3] = '\0';
                strncpy(multiciclo->IR.srt, multiciclo->IR.binario + 7, 3);
                multiciclo->IR.srt[3] = '\0';
                strncpy(multiciclo->IR.simm, multiciclo->IR.binario + 10, 6);
                multiciclo->IR.srt[3] = '\0';

                multiciclo->IR.op = strtol(multiciclo->IR.sop, NULL, 2);
                multiciclo->IR.rs = strtol(multiciclo->IR.srs, NULL, 2);
                multiciclo->IR.rt = strtol(multiciclo->IR.srt, NULL, 2);
                multiciclo->IR.imm = strtol(multiciclo->IR.simm, NULL, 2);

                printf("OP: %d\n", multiciclo->IR.op);
                printf("RS: %d\n", multiciclo->IR.rs);
                printf("RT: %d\n", multiciclo->IR.rt);
                printf("IMM: %d\n", multiciclo->IR.imm);
            }
            ULA(0, multiciclo->pc, multiciclo->IR.imm, multiciclo);
            printf("UlaSaida: %d\n", multiciclo->UlaSaida);
        break;

        case 2: //Execução ADDI,SW,LW
        //identificainstrucao(multiciclo);
        break;

        case 6://Execução ADDI
            identificainstrucao(multiciclo);
            ULA(0, multiciclo->registradores[multiciclo->IR.rs], multiciclo->IR.imm, multiciclo);
            multiciclo->registradores[multiciclo->IR.rt] = multiciclo->UlaSaida;
        break;

        case 7: //Execução Tipo R
            identificainstrucao(multiciclo);
            ULA(multiciclo->IR.funct, multiciclo->registradores[multiciclo->IR.rs], multiciclo->registradores[multiciclo->IR.rt], multiciclo);
            multiciclo->registradores[multiciclo->IR.rd] = multiciclo->UlaSaida;
        break;

        case 10: //Execução Jump
            identificainstrucao(multiciclo);
            multiciclo->pc = multiciclo->IR.end;
        break;
    }
}