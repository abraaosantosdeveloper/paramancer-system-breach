#ifndef ANALISE_H
#define ANALISE_H
#include "tipos.h"

// API de analise estatistica do historico de sessoes.
Stats processar_historico(Session sessoes[], int n);
// Soma recursiva do numero de tentativas.
int soma_tentativas_recursiva(Session sessoes[], int n);
// Soma recursiva dos desvios ao quadrado.
double soma_quadrados_recursiva(Session sessoes[], int n, double media);

#endif