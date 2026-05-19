#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include "historico.h"

static void garantir_pasta_dados(void)
{
    // Garante que a pasta exista antes de gravar o CSV.
#ifdef _WIN32
    // Cria a pasta no Windows.
    _mkdir("dados");
#else
    // Cria a pasta no Linux/macOS.
    mkdir("dados", 0777);
#endif
}

// Serializa uma sessao e salva no arquivo CSV.
void salvar_sessao(Session s)
{
    // Garante o diretorio antes de abrir o CSV.
    garantir_pasta_dados();
    // Modo "a" (append) abre para anexar. Se não existir, o C cria o arquivo. [cite: 133, 189]
    FILE *f = fopen(ARQUIVO_NOME, "a");
    if (f == NULL)
    {
        printf("Erro ao abrir arquivo para escrita!\n");
        return;
    }

    // Formato: timestamp;alvo;tentativas;baixos;altos;palpites_csv [cite: 123, 125]
    fprintf(f, "%s;%d;%d;%d;%d;", s.timestamp, s.target, s.attempts_count, s.low_count, s.high_count);

    // Serializa os palpites como CSV dentro do campo final.
    for (int i = 0; i < s.attempts_count; i++)
    {
        fprintf(f, "%d%s", s.guesses[i], (i == s.attempts_count - 1) ? "" : ",");
    }
    fprintf(f, "\n");
    fclose(f);
}

// Le o CSV e preenche o vetor de sessoes ate o limite.
int carregar_historico(Session sessoes[], int max_sessoes)
{
    // Garante o diretorio antes de ler o CSV.
    garantir_pasta_dados();
    FILE *f = fopen(ARQUIVO_NOME, "r");
    if (f == NULL)
    {
        // Cria o arquivo se nao existir para futuras gravacoes.
        f = fopen(ARQUIVO_NOME, "a");
        if (f != NULL)
            fclose(f);
        return 0; // Se nao existe, apenas retorna 0 sessoes [cite: 188]
    }

    char linha[1024];
    int count = 0;

    // Le cada linha enquanto houver espaco no vetor.
    while (fgets(linha, sizeof(linha), f) && count < max_sessoes)
    {
        Session *s = &sessoes[count];
        // Note: sscanf simplificado para o exemplo de estrutura CSV [cite: 133]
        char palpites_raw[512];
        // Le campos basicos; os palpites ficam no CSV mas nao sao expandidos aqui.
        sscanf(linha, "%[^;];%d;%d;%d;%d;%s",
               s->timestamp, &s->target, &s->attempts_count,
               &s->low_count, &s->high_count, palpites_raw);

        // Avanca para a proxima sessao.
        count++;
    }

    // Fecha o arquivo e retorna o total lido.
    fclose(f);
    return count;
}