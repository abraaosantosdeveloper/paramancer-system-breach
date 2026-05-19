#ifndef HISTORICO_H
#define HISTORICO_H

#include "tipos.h"

// Caminho do arquivo CSV de historico.
#define ARQUIVO_NOME "dados/historico.csv"

// Grava uma sessao no CSV de historico.
void salvar_sessao(Session s);

// Carrega sessoes do CSV para analise e exibicao.
int carregar_historico(Session sessoes[], int max_sessoes);

#endif