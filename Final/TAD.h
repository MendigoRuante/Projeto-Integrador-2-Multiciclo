#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Tam_Instrucao 17    // 16 bits + 1 para o \0
#define NUM_REGISTRADORES 8 // Numero de Registradores, 3 bits = 8
#define Tam_Memoria 256 // Limite da Memória

typedef enum { TipoR, TipoI, TipoJ,Dado,NOP } Tipo;

typedef struct {
  char binario[17];
  int op, rs, rt, rd, imm, funct, end;
  Tipo tipo;
  // Memória de dados
  int dado;
} Memoria;

typedef struct {
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

typedef struct No {
	Multiciclo* multiciclo;
	struct No *proximo;
} No;

void Decodicador(Memoria *memoria);
void imprimir_registradores(Multiciclo *multiciclo);
Ula ULA(int ulaop, int operando1, int operando2);
int BinParaDec(char binario[]);
void identificainstrucao(Multiciclo *multiciclo);
void Ler_Arquivo(Memoria *memoria, int *qtd_instrucoes);
void step(Memoria *memoria, Multiciclo *multiciclo);
void controle(Memoria *instrucao, Multiciclo *multiciclo, int estado);
void imprimememoria(Memoria *memoria);
void empilhar(No **topo, Multiciclo* multiciclo);
void desempilhar(No **topo, Multiciclo* multiciclo);
void converterDadoParaBinario(Memoria *mem);
void GerarMEM(Memoria *memoria);
void ASM(Memoria *instrucoes, int cont);
