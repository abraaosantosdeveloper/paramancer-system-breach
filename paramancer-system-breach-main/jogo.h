#ifndef JOGO_H
#define JOGO_H

#include "tipos.h"
#include <raylib.h>
// Additional include for editors that may not see the Makefile include flags
#include "actors.h"
#include "geracao.h"

// Global instances defined in jogo.c
extern Entity entity;
extern Dex dex;
extern Music bgm;
extern char gameMap[MAP_ALTURA][MAP_LARGURA];
extern Texture2D floorTexture;

// API do loop principal do jogo em modo console.

/*
 * Inicia uma nova sessão de jogo.
 * Sorteia o número alvo, processa os palpites do utilizador e
 * retorna uma estrutura Session preenchida com os dados da partida.
 */

// Executa uma partida em modo console e retorna os dados da sessao.

Session executar_partida();

#endif