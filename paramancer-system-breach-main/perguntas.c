#include <stdio.h>
#include <string.h>
#include "perguntas.h"

// Copia texto com limites e garante terminador nulo.
static void copy_text(char *dst, size_t dst_size, const char *src)
{
    // Copia segura com terminador para evitar overflow.
    // Valida destino e tamanho.
    if (!dst || dst_size == 0)
        return;
    // Normaliza ponteiro nulo para string vazia.
    if (!src)
        src = "";
    // Copia ate o limite e finaliza a string.
    strncpy(dst, src, dst_size - 1);
    dst[dst_size - 1] = '\0';
}

// Remove caracteres de nova linha ao fim da string.
static void trim_newline(char *s)
{
    // Remove CR/LF ao final da linha para facilitar o parse.
    // Protege ponteiro nulo.
    if (!s)
        return;
    size_t len = strlen(s);
    // Varre de tras para frente removendo CR/LF.
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r'))
    {
        s[len - 1] = '\0';
        len--;
    }
}

// Busca no CSV a pergunta pela linha correspondente ao id.
int carregar_pergunta_por_id(int id, Pergunta *out)
{
    // Valida parametros basicos.
    if (id <= 0 || !out)
        return 0;

    // Abre o CSV de perguntas.
    FILE *arquivo = fopen("perguntas.csv", "r");
    if (!arquivo)
        return 0;

    char linha[2048];
    int linha_atual = 0;

    // Percorre o arquivo linha a linha.
    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        // Ignora linhas vazias.
        if (linha[0] == '\n' || linha[0] == '\r' || linha[0] == '\0')
            continue;

        linha_atual++;
        // Pula ate chegar ao id desejado.
        if (linha_atual != id)
            continue;

        // Divide a linha CSV em 6 campos: enunciado, A, B, C, D, correta.
        char *fields[6];
        int count = 0;
        char *token = strtok(linha, ",");
        // Tokeniza a linha pelos delimitadores.
        while (token && count < 6)
        {
            fields[count++] = token;
            token = strtok(NULL, ",");
        }

        // Fecha o arquivo assim que a linha alvo for lida.
        fclose(arquivo);

        // Garante que todos os campos existam.
        if (count < 6)
            return 0;

        // Copia os campos para a struct de saida.
        trim_newline(fields[5]);
        copy_text(out->enunciado, sizeof(out->enunciado), fields[0]);
        copy_text(out->alt_a, sizeof(out->alt_a), fields[1]);
        copy_text(out->alt_b, sizeof(out->alt_b), fields[2]);
        copy_text(out->alt_c, sizeof(out->alt_c), fields[3]);
        copy_text(out->alt_d, sizeof(out->alt_d), fields[4]);
        out->correta = fields[5][0];
        return 1;
    }

    // Linha nao encontrada.
    fclose(arquivo);
    return 0;
}
