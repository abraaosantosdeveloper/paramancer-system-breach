#ifndef JOGO_H
#define JOGO_H

#include "tipos.h"

// API do loop principal do jogo em modo console.

/*
 * Inicia uma nova sessão de jogo.
 * Sorteia o número alvo, processa os palpites do utilizador e
 * retorna uma estrutura Session preenchida com os dados da partida.
 */

// Executa uma partida em modo console e retorna os dados da sessao.
Session executar_partida();

#endif