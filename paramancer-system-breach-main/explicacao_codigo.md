# Explicacao do codigo (.c e .h)

Este documento explica, linha a linha, os arquivos .c e .h do projeto. A biblioteca TextUtils (text_utils.c/.h) nao esta detalhada, uma vez que se trata de uma biblioteca externa.

## Sumario (arquivos)
- [Arquivo tipos.h](#arquivo-tiposh)
- [Arquivo analise.h](#arquivo-analiseh)
- [Arquivo analise.c](#arquivo-analisec)
- [Arquivo historico.h](#arquivo-historicoh)
- [Arquivo historico.c](#arquivo-historicoc)
- [Arquivo perguntas.h](#arquivo-perguntash)
- [Arquivo perguntas.c](#arquivo-perguntasc)
- [Arquivo sorteio.h](#arquivo-sorteioh)
- [Arquivo sorteio.c](#arquivo-sorteioc)
- [Arquivo jogo.h](#arquivo-jogoh)
- [Arquivo jogo.c](#arquivo-jogoc)
- [Arquivo ui_menu.h](#arquivo-ui_menuh)
- [Arquivo ui_menu.c](#arquivo-ui_menuc)
- [Arquivo main.c](#arquivo-mainc)

## Arquivo tipos.h

### Linha a linha
- L1: Inicia o include guard para evitar inclusoes multiplas deste cabecalho.
- L2: Define a macro do include guard.
- L3: Linha em branco para separar blocos.
- L4: Define `MAX_GUESSES` como constante de pre-processador. Insight: `#define` cria uma substituicao de texto em tempo de compilacao.
- L5: Linha em branco para separar definicoes.
- L6: Inicia a declaracao da struct `Session` com `typedef`.
- L7: Abre o bloco da struct `Session`.
- L8: Declara `timestamp` como vetor de `char` para armazenar string de data/hora. Insight: strings em C sao vetores terminados em `\0`.
- L9: Declara `target` para o numero sorteado.
- L10: Declara `attempts_count` para contar tentativas.
- L11: Declara `guesses` como vetor de inteiros com tamanho `MAX_GUESSES`. Insight: vetores armazenam varios valores do mesmo tipo em memoria contigua.
- L12: Declara `low_count` para contar palpites abaixo do alvo.
- L13: Declara `high_count` para contar palpites acima do alvo.
- L14: Fecha a struct e cria o alias `Session`.
- L15: Linha em branco para separar structs.
- L16: Inicia a declaracao da struct `Stats` com `typedef`.
- L17: Abre o bloco da struct `Stats`.
- L18: Declara `average_attempts` para a media de tentativas.
- L19: Declara `best_attempts` para o melhor resultado.
- L20: Declara `worst_attempts` para o pior resultado.
- L21: Declara `stddev_attempts` para o desvio padrao das tentativas.
- L22: Declara `avg_low_bias` para media de palpites baixos.
- L23: Declara `avg_high_bias` para media de palpites altos.
- L24: Fecha a struct e cria o alias `Stats`.
- L25: Linha em branco para separar do fim do cabecalho.
- L26: Encerra o include guard.

## Arquivo analise.h

### Linha a linha
- L1: Inicia o include guard de analise.h.
- L2: Define a macro do include guard.
- L3: Inclui tipos.h para acessar `Session` e `Stats`.
- L4: Linha em branco para separar includes de prototipos.
- L5: Declara `processar_historico` para calcular estatisticas.
- L6: Declara `soma_tentativas_recursiva` para soma recursiva das tentativas.
- L7: Declara `soma_quadrados_recursiva` para soma de quadrados.
- L8: Linha em branco para separar do fim do cabecalho.
- L9: Encerra o include guard.

<a id="funcao-processar_historico-analise-h"></a>
### Funcao processar_historico (declaracao em analise.h)

Codigo da funcao:
```c
Stats processar_historico(Session sessoes[], int n);
```
> Explicacao: Linha 5 declara uma funcao que recebe um vetor de `Session` e retorna um `Stats` com dados de analise.

<a id="funcao-soma_tentativas_recursiva-analise-h"></a>
### Funcao soma_tentativas_recursiva (declaracao em analise.h)

Codigo da funcao:
```c
int soma_tentativas_recursiva(Session sessoes[], int n);
```
> Explicacao: Linha 6 declara uma funcao auxiliar que soma tentativas via recursao.

<a id="funcao-soma_quadrados_recursiva-analise-h"></a>
### Funcao soma_quadrados_recursiva (declaracao em analise.h)

Codigo da funcao:
```c
double soma_quadrados_recursiva(Session sessoes[], int n, double media);
```
> Explicacao: Linha 7 declara uma funcao auxiliar para soma de quadrados baseada em uma media.

## Arquivo analise.c

### Linhas fora de funcoes
- L1: Inclui analise.h para ter as declaracoes das funcoes e tipos usados aqui.
- L2: Inclui math.h para usar `sqrt`.
- L3: Linha em branco para separar includes do codigo.
- L4: Comentario que descreve a funcao recursiva a seguir.
- L11: Linha em branco para separar funcoes.
- L12: Comentario que descreve a soma de quadrados.
- L20: Linha em branco para separar funcoes.

<a id="funcao-soma_tentativas_recursiva-analise-c"></a>
### Funcao soma_tentativas_recursiva (analise.c)

Codigo da funcao:
```c
int soma_tentativas_recursiva(Session sessoes[], int n)
{
    if (n <= 0)
        return 0;
    return sessoes[n - 1].attempts_count + soma_tentativas_recursiva(sessoes, n - 1);
}
```
> Explicacao: Linhas 5-10 somam recursivamente o campo `attempts_count` de cada sessao, reduzindo `n` ate o caso base.
- L5: Define a assinatura com um vetor de `Session` e o tamanho `n`. Insight: `sessoes[]` indica um vetor passado por referencia.
- L6: Abre o bloco da funcao.
- L7: Verifica o caso base da recursao (`n <= 0`).
- L8: Retorna 0 quando nao ha elementos para somar.
- L9: Soma a tentativa da ultima sessao com a chamada recursiva da propria funcao.
- L10: Fecha o bloco da funcao.

<a id="funcao-soma_quadrados_recursiva-analise-c"></a>
### Funcao soma_quadrados_recursiva (analise.c)

Codigo da funcao:
```c
double soma_quadrados_recursiva(Session sessoes[], int n, double media)
{
    if (n <= 0)
        return 0.0;
    double diff = sessoes[n - 1].attempts_count - media;
    return (diff * diff) + soma_quadrados_recursiva(sessoes, n - 1, media);
}
```
> Explicacao: Linhas 13-19 calculam a soma dos quadrados das diferencas em relacao a media.
- L13: Define a assinatura com vetor de sessoes, tamanho `n` e a `media`.
- L14: Abre o bloco da funcao.
- L15: Verifica o caso base da recursao.
- L16: Retorna 0.0 quando nao ha elementos para somar.
- L17: Calcula a diferenca entre a tentativa e a media.
- L18: Soma o quadrado da diferenca com a chamada recursiva.
- L19: Fecha o bloco da funcao.

<a id="funcao-processar_historico-analise-c"></a>
### Funcao processar_historico (analise.c)

Codigo da funcao:
```c
Stats processar_historico(Session sessoes[], int n)
{
    Stats st = {0};
    if (n <= 0)
        return st;

    st.average_attempts = (double)soma_tentativas_recursiva(sessoes, n) / n;
    double soma_q = soma_quadrados_recursiva(sessoes, n, st.average_attempts);
    st.stddev_attempts = sqrt(soma_q / n);
    return st;
}
```
> Explicacao: Linhas 21-31 calculam media e desvio padrao de tentativas usando funcoes recursivas.
- L21: Define a assinatura da funcao que retorna `Stats`.
- L22: Abre o bloco da funcao.
- L23: Inicializa `Stats st` com zeros. Insight: `{0}` zera todos os campos da struct.
- L24: Verifica se `n` e valido.
- L25: Retorna `st` vazio quando nao ha dados.
- L26: Linha em branco para separar validacao de calculos.
- L27: Calcula a media chamando [Funcao soma_tentativas_recursiva](#funcao-soma_tentativas_recursiva-analise-c).
- L28: Calcula a soma dos quadrados chamando [Funcao soma_quadrados_recursiva](#funcao-soma_quadrados_recursiva-analise-c).
- L29: Calcula o desvio padrao usando `sqrt`.
- L30: Retorna a struct `Stats` preenchida.
- L31: Fecha o bloco da funcao.

## Arquivo historico.h

### Linha a linha
- L1: Inicia o include guard de historico.h.
- L2: Define a macro do include guard.
- L3: Linha em branco para separar blocos.
- L4: Inclui tipos.h para usar `Session`.
- L5: Linha em branco para separar definicoes.
- L6: Define `ARQUIVO_NOME` com o caminho do CSV de historico.
- L7: Linha em branco para separar comentarios.
- L8: Comentario de requisito funcional sobre gravacao.
- L9: Declara `salvar_sessao`.
- L10: Linha em branco para separar comentarios.
- L11: Comentario de requisito funcional sobre leitura.
- L12: Declara `carregar_historico`.
- L13: Linha em branco para separar do fim do cabecalho.
- L14: Encerra o include guard.

<a id="funcao-salvar_sessao-historico-h"></a>
### Funcao salvar_sessao (declaracao em historico.h)

Codigo da funcao:
```c
void salvar_sessao(Session s);
```
> Explicacao: Linha 9 declara a funcao que grava uma sessao no arquivo de historico.

<a id="funcao-carregar_historico-historico-h"></a>
### Funcao carregar_historico (declaracao em historico.h)

Codigo da funcao:
```c
int carregar_historico(Session sessoes[], int max_sessoes);
```
> Explicacao: Linha 12 declara a funcao que le varias sessoes para analise.

## Arquivo historico.c

### Linhas fora de funcoes
- L1: Inclui stdio.h para `FILE`, `fopen`, `fprintf` e `fgets`.
- L2: Inclui stdlib.h para utilitarios gerais.
- L3: Inclui string.h para `sscanf`.
- L4: Inicio de compilacao condicional para Windows.
- L5: Inclui direct.h para `_mkdir` no Windows.
- L6: Inicio da alternativa para outros sistemas.
- L7: Inclui sys/stat.h para `mkdir` fora do Windows.
- L8: Fecha o bloco condicional.
- L9: Inclui historico.h para `Session` e prototipos.
- L10: Linha em branco para separar includes do codigo.
- L19: Linha em branco para separar funcoes.
- L41: Linha em branco para separar funcoes.

<a id="funcao-garantir_pasta_dados-historico-c"></a>
### Funcao garantir_pasta_dados (historico.c)

Codigo da funcao:
```c
static void garantir_pasta_dados(void)
{
#ifdef _WIN32
    _mkdir("dados");
#else
    mkdir("dados", 0777);
#endif
}
```
> Explicacao: Linhas 11-18 criam a pasta `dados` de forma portavel entre Windows e outros sistemas.
- L11: Declara funcao estatica, visivel apenas neste arquivo.
- L12: Abre o bloco da funcao.
- L13: Inicia bloco condicional para Windows.
- L14: Cria a pasta `dados` no Windows.
- L15: Alternativa para outros sistemas.
- L16: Cria a pasta `dados` com permissoes 0777.
- L17: Encerra o bloco condicional.
- L18: Fecha o bloco da funcao.

<a id="funcao-salvar_sessao-historico-c"></a>
### Funcao salvar_sessao (historico.c)

Codigo da funcao:
```c
void salvar_sessao(Session s)
{
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

    for (int i = 0; i < s.attempts_count; i++)
    {
        fprintf(f, "%d%s", s.guesses[i], (i == s.attempts_count - 1) ? "" : ",");
    }
    fprintf(f, "\n");
    fclose(f);
}
```
> Explicacao: Linhas 20-40 gravam uma sessao no CSV, anexando o registro ao final do arquivo.
- L20: Define a assinatura da funcao.
- L21: Abre o bloco da funcao.
- L22: Chama [Funcao garantir_pasta_dados](#funcao-garantir_pasta_dados-historico-c) para garantir a pasta.
- L23: Comentario explicando o modo de abertura do arquivo.
- L24: Abre o arquivo em modo append usando `ARQUIVO_NOME`.
- L25: Verifica se a abertura falhou.
- L26: Abre o bloco do `if` de erro.
- L27: Exibe mensagem de erro.
- L28: Retorna para encerrar a funcao em caso de falha.
- L29: Fecha o bloco do `if`.
- L30: Linha em branco para separar o formato CSV.
- L31: Comentario explicando a estrutura do registro.
- L32: Escreve os campos fixos do registro no arquivo.
- L33: Linha em branco para separar o loop de palpites.
- L34: Inicia loop para escrever cada palpite.
- L35: Abre o bloco do `for`.
- L36: Escreve o palpite e a virgula quando necessario.
- L37: Fecha o bloco do `for`.
- L38: Escreve a quebra de linha do registro.
- L39: Fecha o arquivo.
- L40: Fecha o bloco da funcao.

<a id="funcao-carregar_historico-historico-c"></a>
### Funcao carregar_historico (historico.c)

Codigo da funcao:
```c
int carregar_historico(Session sessoes[], int max_sessoes)
{
    garantir_pasta_dados();
    FILE *f = fopen(ARQUIVO_NOME, "r");
    if (f == NULL)
    {
        f = fopen(ARQUIVO_NOME, "a");
        if (f != NULL)
            fclose(f);
        return 0; // Se nao existe, apenas retorna 0 sessoes [cite: 188]
    }

    char linha[1024];
    int count = 0;

    while (fgets(linha, sizeof(linha), f) && count < max_sessoes)
    {
        Session *s = &sessoes[count];
        // Note: sscanf simplificado para o exemplo de estrutura CSV [cite: 133]
        char palpites_raw[512];
        sscanf(linha, "%[^;];%d;%d;%d;%d;%s",
               s->timestamp, &s->target, &s->attempts_count,
               &s->low_count, &s->high_count, palpites_raw);

        count++;
    }

    fclose(f);
    return count;
}
```
> Explicacao: Linhas 42-71 leem o arquivo de historico e preenchem um vetor de `Session`.
- L42: Define a assinatura da funcao.
- L43: Abre o bloco da funcao.
- L44: Chama [Funcao garantir_pasta_dados](#funcao-garantir_pasta_dados-historico-c).
- L45: Tenta abrir o arquivo em modo leitura.
- L46: Verifica falha na abertura.
- L47: Abre o bloco do `if` de erro.
- L48: Tenta criar o arquivo abrindo em modo append.
- L49: Verifica se abriu com sucesso.
- L50: Fecha o arquivo criado.
- L51: Retorna 0 quando nao ha historico.
- L52: Fecha o bloco do `if`.
- L53: Linha em branco para separar variaveis locais.
- L54: Declara buffer para a linha lida.
- L55: Inicializa o contador de sessoes carregadas.
- L56: Linha em branco para separar o loop.
- L57: Le linhas enquanto houver e respeita `max_sessoes`.
- L58: Abre o bloco do `while`.
- L59: Aponta `s` para a sessao atual no vetor.
- L60: Comentario sobre o parse simplificado.
- L61: Declara buffer para o CSV de palpites (nao usado depois).
- L62: Faz o parse da linha usando `sscanf`.
- L63: Continua a lista de variaveis do `sscanf`.
- L64: Finaliza o parse dos campos.
- L65: Linha em branco para separar incremento.
- L66: Incrementa a quantidade de sessoes lidas.
- L67: Fecha o bloco do `while`.
- L68: Linha em branco para separar fechamento do arquivo.
- L69: Fecha o arquivo.
- L70: Retorna o numero de sessoes carregadas.
- L71: Fecha o bloco da funcao.

## Arquivo perguntas.h

### Linha a linha
- L1: Inicia o include guard de perguntas.h.
- L2: Define a macro do include guard.
- L3: Linha em branco para separar blocos.
- L4: Define tamanho maximo do enunciado.
- L5: Define tamanho maximo das alternativas.
- L6: Linha em branco para separar definicoes.
- L7: Inicia a declaracao da struct `Pergunta`.
- L8: Abre o bloco da struct.
- L9: Declara `enunciado` como vetor de `char`.
- L10: Declara `alt_a` como vetor de `char`.
- L11: Declara `alt_b` como vetor de `char`.
- L12: Declara `alt_c` como vetor de `char`.
- L13: Declara `alt_d` como vetor de `char`.
- L14: Declara `correta` como um `char` com a letra correta.
- L15: Fecha a struct e cria o alias `Pergunta`.
- L16: Linha em branco para separar prototipo.
- L17: Declara `carregar_pergunta_por_id`.
- L18: Linha em branco para separar do fim do cabecalho.
- L19: Encerra o include guard.

<a id="funcao-carregar_pergunta_por_id-perguntas-h"></a>
### Funcao carregar_pergunta_por_id (declaracao em perguntas.h)

Codigo da funcao:
```c
int carregar_pergunta_por_id(int id, Pergunta *out);
```
> Explicacao: Linha 17 declara a funcao que busca uma pergunta no CSV e preenche a struct.

## Arquivo perguntas.c

### Linhas fora de funcoes
- L1: Inclui stdio.h para `FILE` e `fopen`.
- L2: Inclui string.h para `strlen`, `strncpy` e `strtok`.
- L3: Inclui perguntas.h para usar `Pergunta` e prototipo.
- L4: Linha em branco para separar includes do codigo.
- L14: Linha em branco para separar funcoes.
- L26: Linha em branco para separar funcoes.

<a id="funcao-copy_text-perguntas-c"></a>
### Funcao copy_text (perguntas.c)

Codigo da funcao:
```c
static void copy_text(char *dst, size_t dst_size, const char *src)
{
    if (!dst || dst_size == 0)
        return;
    if (!src)
        src = "";
    strncpy(dst, src, dst_size - 1);
    dst[dst_size - 1] = '\0';
}
```
> Explicacao: Linhas 5-13 copiam texto com seguranca para evitar overflow.
- L5: Declara funcao estatica para copiar texto com limite.
- L6: Abre o bloco da funcao.
- L7: Valida ponteiro e tamanho do destino.
- L8: Retorna cedo se o destino for invalido.
- L9: Verifica se a fonte e nula.
- L10: Substitui por string vazia se `src` for nula.
- L11: Copia com `strncpy` ate `dst_size - 1`.
- L12: Garante terminador `\0` no final.
- L13: Fecha o bloco da funcao.

<a id="funcao-trim_newline-perguntas-c"></a>
### Funcao trim_newline (perguntas.c)

Codigo da funcao:
```c
static void trim_newline(char *s)
{
    if (!s)
        return;
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r'))
    {
        s[len - 1] = '\0';
        len--;
    }
}
```
> Explicacao: Linhas 15-25 removem quebras de linha no final de uma string.
- L15: Declara funcao estatica para limpar `\n` e `\r`.
- L16: Abre o bloco da funcao.
- L17: Verifica se o ponteiro e nulo.
- L18: Retorna cedo se `s` for nula.
- L19: Calcula o tamanho da string.
- L20: Loop enquanto o ultimo caractere for quebra de linha.
- L21: Abre o bloco do `while`.
- L22: Substitui o ultimo caractere por `\0`.
- L23: Decrementa o tamanho para continuar a limpeza.
- L24: Fecha o bloco do `while`.
- L25: Fecha o bloco da funcao.

<a id="funcao-carregar_pergunta_por_id-perguntas-c"></a>
### Funcao carregar_pergunta_por_id (perguntas.c)

Codigo da funcao:
```c
int carregar_pergunta_por_id(int id, Pergunta *out)
{
    if (id <= 0 || !out)
        return 0;

    FILE *arquivo = fopen("perguntas.csv", "r");
    if (!arquivo)
        return 0;

    char linha[2048];
    int linha_atual = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        if (linha[0] == '\n' || linha[0] == '\r' || linha[0] == '\0')
            continue;

        linha_atual++;
        if (linha_atual != id)
            continue;

        char *fields[6];
        int count = 0;
        char *token = strtok(linha, ",");
        while (token && count < 6)
        {
            fields[count++] = token;
            token = strtok(NULL, ",");
        }

        fclose(arquivo);

        if (count < 6)
            return 0;

        trim_newline(fields[5]);
        copy_text(out->enunciado, sizeof(out->enunciado), fields[0]);
        copy_text(out->alt_a, sizeof(out->alt_a), fields[1]);
        copy_text(out->alt_b, sizeof(out->alt_b), fields[2]);
        copy_text(out->alt_c, sizeof(out->alt_c), fields[3]);
        copy_text(out->alt_d, sizeof(out->alt_d), fields[4]);
        out->correta = fields[5][0];
        return 1;
    }

    fclose(arquivo);
    return 0;
}
```
> Explicacao: Linhas 27-74 buscam a linha `id` no CSV, fazem o parse e preenchem `Pergunta`.
- L27: Define a assinatura da funcao.
- L28: Abre o bloco da funcao.
- L29: Valida `id` e o ponteiro `out`.
- L30: Retorna 0 se os parametros forem invalidos.
- L31: Linha em branco para separar a abertura do arquivo.
- L32: Abre o arquivo `perguntas.csv` em modo leitura.
- L33: Verifica falha na abertura.
- L34: Retorna 0 se o arquivo nao abriu.
- L35: Linha em branco para separar variaveis locais.
- L36: Declara buffer para cada linha do arquivo.
- L37: Inicializa o contador de linha atual.
- L38: Linha em branco para separar o loop.
- L39: Le linhas do arquivo com `fgets`.
- L40: Abre o bloco do `while`.
- L41: Ignora linhas vazias.
- L42: Usa `continue` para pular a linha vazia.
- L43: Linha em branco para separar o controle de indice.
- L44: Incrementa a contagem de linhas validas.
- L45: Verifica se a linha atual e a desejada.
- L46: Continua o loop se ainda nao for a linha `id`.
- L47: Linha em branco para separar o parse.
- L48: Declara vetor de ponteiros para os campos.
- L49: Inicializa contador de campos.
- L50: Inicia o tokenizador `strtok` usando virgula.
- L51: Loop para extrair ate 6 campos.
- L52: Abre o bloco do `while`.
- L53: Armazena o campo atual.
- L54: Avanca para o proximo token.
- L55: Fecha o bloco do `while`.
- L56: Linha em branco para separar o fechamento do arquivo.
- L57: Fecha o arquivo apos encontrar a linha.
- L58: Linha em branco para separar a validacao.
- L59: Verifica se a quantidade de campos e suficiente.
- L60: Retorna 0 se o CSV estiver incompleto.
- L61: Linha em branco para separar preenchimento da struct.
- L62: Chama [Funcao trim_newline](#funcao-trim_newline-perguntas-c) para limpar `\n` do ultimo campo.
- L63: Chama [Funcao copy_text](#funcao-copy_text-perguntas-c) para preencher `enunciado`.
- L64: Copia a alternativa A.
- L65: Copia a alternativa B.
- L66: Copia a alternativa C.
- L67: Copia a alternativa D.
- L68: Guarda a letra correta.
- L69: Retorna 1 indicando sucesso.
- L70: Fecha o bloco do `while`.
- L71: Linha em branco para separar o fallback.
- L72: Fecha o arquivo se nao encontrou a linha.
- L73: Retorna 0 indicando falha.
- L74: Fecha o bloco da funcao.

## Arquivo sorteio.h

### Linha a linha
- L1: Inicia o include guard de sorteio.h.
- L2: Define a macro do include guard.
- L3: Linha em branco para separar prototipos.
- L4: Declara `sortearNumero`.
- L5: Declara `sortearPergunta`.
- L6: Linha em branco para separar do fim do cabecalho.
- L7: Encerra o include guard.

<a id="funcao-sortearnumero-sorteio-h"></a>
### Funcao sortearNumero (declaracao em sorteio.h)

Codigo da funcao:
```c
int sortearNumero(void);
```
> Explicacao: Linha 4 declara a funcao que sorteia o numero alvo.

<a id="funcao-sortearpergunta-sorteio-h"></a>
### Funcao sortearPergunta (declaracao em sorteio.h)

Codigo da funcao:
```c
int sortearPergunta(void);
```
> Explicacao: Linha 5 declara a funcao que sorteia uma pergunta.

## Arquivo sorteio.c

### Linhas fora de funcoes
- L1: Inclui stdio.h para `FILE`.
- L2: Inclui stdlib.h para `rand` e `srand`.
- L3: Inclui time.h para `time`.
- L4: Inclui sorteio.h para os prototipos.
- L5: Linha em branco para separar includes do codigo.
- L15: Linha em branco para separar funcoes.
- L21: Linha em branco para separar funcoes.

<a id="funcao-inicializar_seed-sorteio-c"></a>
### Funcao inicializar_seed (sorteio.c)

Codigo da funcao:
```c
static void inicializar_seed(void)
{
    static int seed_inicializada = 0;
    if (!seed_inicializada)
    {
        srand((unsigned int)time(NULL));
        seed_inicializada = 1;
    }
}
```
> Explicacao: Linhas 6-14 garantem que o seed do gerador aleatorio seja configurado apenas uma vez.
- L6: Declara funcao estatica para inicializar o seed.
- L7: Abre o bloco da funcao.
- L8: Declara variavel estatica que preserva estado entre chamadas.
- L9: Verifica se o seed ja foi inicializado.
- L10: Abre o bloco do `if`.
- L11: Chama `srand` com o tempo atual.
- L12: Marca o seed como inicializado.
- L13: Fecha o bloco do `if`.
- L14: Fecha o bloco da funcao.

<a id="funcao-sortearnumero-sorteio-c"></a>
### Funcao sortearNumero (sorteio.c)

Codigo da funcao:
```c
int sortearNumero(void)
{
    inicializar_seed();
    return (rand() % 100) + 1;
}
```
> Explicacao: Linhas 16-20 sorteiam um numero entre 1 e 100.
- L16: Define a assinatura da funcao.
- L17: Abre o bloco da funcao.
- L18: Chama [Funcao inicializar_seed](#funcao-inicializar_seed-sorteio-c) para garantir seed.
- L19: Retorna um inteiro aleatorio entre 1 e 100.
- L20: Fecha o bloco da funcao.

<a id="funcao-sortearpergunta-sorteio-c"></a>
### Funcao sortearPergunta (sorteio.c)

Codigo da funcao:
```c
int sortearPergunta(void)
{
    inicializar_seed();

    static int total_linhas = -1;
    if (total_linhas < 0)
    {
        FILE *arquivo = fopen("perguntas.csv", "r");
        if (arquivo == NULL)
            return -1;

        total_linhas = 0;
        char linha[2048];

        while (fgets(linha, sizeof(linha), arquivo) != NULL)
        {
            if (linha[0] != '\n' && linha[0] != '\r' && linha[0] != '\0')
                total_linhas++;
        }

        fclose(arquivo);
    }

    if (total_linhas <= 0)
        return -1;

    return (rand() % total_linhas) + 1;
}
```
> Explicacao: Este trecho conta as linhas do CSV uma unica vez e reutiliza o total nas chamadas seguintes.
- Usa `static int total_linhas` para cachear o total.
- Le o arquivo apenas se o cache ainda nao foi preenchido.
- Retorna um id valido entre 1 e `total_linhas`.

## Arquivo jogo.h

### Linha a linha
- L1: Inicia o include guard de jogo.h.
- L2: Define a macro do include guard.
- L3: Linha em branco para separar includes.
- L4: Inclui tipos.h para usar `Session`.
- L5: Linha em branco para separar comentarios.
- L6: Comentario de bloco sobre `executar_partida`.
- L7: Continua o comentario sobre a funcao.
- L8: Continua o comentario detalhando o retorno.
- L9: Fecha o comentario de bloco.
- L10: Declara `executar_partida`.
- L11: Linha em branco para separar do fim do cabecalho.
- L12: Encerra o include guard.

<a id="funcao-executar_partida-jogo-h"></a>
### Funcao executar_partida (declaracao em jogo.h)

Codigo da funcao:
```c
Session executar_partida();
```
> Explicacao: Linha 10 declara a funcao principal do jogo, definida em [Funcao executar_partida](#funcao-executar_partida-jogo-c).

## Arquivo jogo.c

### Linhas fora de funcoes
- L1: Inclui stdio.h para `printf` e `scanf`.
- L2: Inclui stdlib.h para utilitarios gerais.
- L3: Inclui time.h para `time`, `localtime` e `strftime`.
- L4: Inclui ctype.h para `tolower`.
- L5: Inclui string.h para `strlen`.
- L6: Inclui jogo.h para `executar_partida`.
- L7: Inclui historico.h para `salvar_sessao`.
- L8: Inclui sorteio.h para `sortearNumero` e `sortearPergunta`.
- L9: Inclui perguntas.h para `Pergunta` e `carregar_pergunta_por_id`.
- L10: Inclui text_utils.h (biblioteca externa nao detalhada aqui).
- L11: Linha em branco para separar includes do codigo.
- L12: Declara constante `CONSOLE_WIDTH`.
- L13: Declara constante `INITIAL_GUESSES`.
- L14: Linha em branco para separar funcoes.

<a id="funcao-clear_line-jogo-c"></a>
### Funcao clear_line (jogo.c)

Codigo da funcao:
```c
static void clear_line(int y)
{
    gotoxy(1, y);
    for (int i = 0; i < CONSOLE_WIDTH; ++i)
        putchar(' ');
}
```
> Explicacao: Linhas 13-18 limpam uma linha do console usando espacos.
- L13: Declara funcao estatica para limpar uma linha.
- L14: Abre o bloco da funcao.
- L15: Posiciona o cursor na coluna 1 usando TextUtils.
- L16: Loop que percorre a largura do console.
- L17: Imprime espaco para apagar cada coluna.
- L18: Fecha o bloco da funcao.

<a id="funcao-wait_for_enter-jogo-c"></a>
### Funcao wait_for_enter (jogo.c)

Codigo da funcao:
```c
static void wait_for_enter(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
    ch = getchar();
    while (ch != '\n' && ch != EOF)
        ch = getchar();
}
```
> Explicacao: Linhas 20-29 consomem caracteres ate o usuario pressionar ENTER.
- L20: Declara funcao estatica.
- L21: Abre o bloco da funcao.
- L22: Declara variavel para leitura de caracteres.
- L23: Loop que consome ate encontrar `\n` ou EOF.
- L24: Abre o bloco do `while`.
- L25: Bloco vazio; o consumo ocorre na condicao do loop.
- L26: Le o proximo caractere apos o primeiro `\n`.
- L27: Loop que continua consumindo ate o proximo `\n`.
- L28: Le caracteres enquanto nao chega o ENTER.
- L29: Fecha o bloco da funcao.

<a id="funcao-clear_question_area-jogo-c"></a>
### Funcao clear_question_area (jogo.c)

Codigo da funcao:
```c
static void clear_question_area(void)
{
    for (int y = 12; y <= 19; ++y)
        clear_line(y);
}
```
> Explicacao: Linhas 31-35 limpam o bloco de linhas onde a pergunta aparece.
- L31: Declara funcao estatica.
- L32: Abre o bloco da funcao.
- L33: Loop pelas linhas 12 a 19.
- L34: Chama [Funcao clear_line](#funcao-clear_line-jogo-c) para limpar cada linha.
- L35: Fecha o bloco da funcao.

<a id="funcao-print_centered_line-jogo-c"></a>
### Funcao print_centered_line (jogo.c)

Codigo da funcao:
```c
static void print_centered_line(int y, const char *text, const char *color)
{
    if (!text)
        return;

    int len = (int)strlen(text);
    int x = (CONSOLE_WIDTH - len) / 2 + 1;
    if (x < 1)
        x = 1;

    gotoxy(x, y);
    print_colored(text, color);
}
```
> Explicacao: Centraliza um texto em uma linha do console e aplica cor.
- Calcula o comprimento e a posicao X centralizada.
- Protege contra posicao invalida.
- Move o cursor e imprime o texto colorido.

<a id="funcao-draw_centered_banner-jogo-c"></a>
### Funcao draw_centered_banner (jogo.c)

Codigo da funcao:
```c
static void draw_centered_banner(const char *text, const char *color)
{
    if (!text)
        return;

    int len = (int)strlen(text);
    int box_width = len + 12;
    if (box_width < 40)
        box_width = 40;
    if (box_width > CONSOLE_WIDTH - 2)
        box_width = CONSOLE_WIDTH - 2;

    int x = (CONSOLE_WIDTH - box_width) / 2 + 1;
    int y = 8;

    draw_double_box(x, y, box_width, 5, color);
    gotoxy(x + (box_width - len) / 2, y + 2);
    print_colored(text, color);
}
```
> Explicacao: Desenha um banner centralizado com borda dupla e texto no meio.
- Ajusta a largura do banner com limites min/max.
- Centraliza o box e imprime o texto no meio.

<a id="funcao-show_end_screen-jogo-c"></a>
### Funcao show_end_screen (jogo.c)

Codigo da funcao:
```c
static void show_end_screen(const Session *s, int venceu)
{
    clear_screen();
    print_title_bar("LuckyGuess - Resultado", BOLD_BLUE, BOLD_WHITE);

    if (venceu)
    {
        draw_centered_banner("VOCE VENCEU", BOLD_YELLOW);
        char msg[96];
        snprintf(msg, sizeof(msg), "Voce acertou, depois de %d tentativas.", s->attempts_count);
        print_centered_line(14, msg, WHITE);
    }
    else
    {
        draw_centered_banner("GAME OVER", BOLD_RED);
        print_centered_line(14, "Voce esgotou suas chances de palpite.", WHITE);
    }

    print_centered_line(16, "Pressione ENTER para ver as estatisticas...", DARK_GRAY);
    wait_for_enter();
    clear_question_area();
}
```
> Explicacao: Exibe a tela final com banner, mensagem centralizada e pausa do usuario.
- Usa a flag `venceu` para escolher o texto e a cor.
- Mostra a mensagem de tentativas quando ha vitoria.
- Exibe game over quando as chances de palpite acabam.
- Aguarda ENTER antes de seguir para as estatisticas.

<a id="funcao-perguntar_para_dica-jogo-c"></a>
### Funcao perguntar_para_dica (jogo.c)

Codigo da funcao:
```c
static int perguntar_para_dica(void)
{
    Pergunta p = {0};
    int id = sortearPergunta();
    if (id <= 0 || !carregar_pergunta_por_id(id, &p))
    {
        clear_line(12);
        gotoxy(4, 12);
        print_colored("Nao foi possivel carregar perguntas.", YELLOW);
        return 0;
    }

    clear_question_area();
    gotoxy(4, 12);
    print_colored(p.enunciado, WHITE);
    gotoxy(6, 14);
    printf("A) %s", p.alt_a);
    gotoxy(6, 15);
    printf("B) %s", p.alt_b);
    gotoxy(6, 16);
    printf("C) %s", p.alt_c);
    gotoxy(6, 17);
    printf("D) %s", p.alt_d);

    clear_line(19);
    gotoxy(4, 19);
    printf("Resposta (a/b/c/d): ");

    char resposta = '\0';
    if (scanf(" %c", &resposta) != 1)
        return 0;

    resposta = (char)tolower((unsigned char)resposta);
    char correta = (char)tolower((unsigned char)p.correta);
    return resposta == correta;
}
```
> Explicacao: Linhas 37-72 exibem uma pergunta, leem a resposta e informam se esta correta.
- L37: Declara funcao estatica que retorna `int`.
- L38: Abre o bloco da funcao.
- L39: Inicializa `Pergunta p` com zeros.
- L40: Sorteia o id chamando [Funcao sortearPergunta](#funcao-sortearpergunta-sorteio-c).
- L41: Carrega a pergunta chamando [Funcao carregar_pergunta_por_id](#funcao-carregar_pergunta_por_id-perguntas-c).
- L42: Abre o bloco de erro se id invalido ou carregamento falhar.
- L43: Limpa a linha 12.
- L44: Posiciona o cursor para a mensagem.
- L45: Exibe mensagem de erro usando TextUtils.
- L46: Retorna 0 indicando falha.
- L47: Fecha o bloco do `if`.
- L48: Linha em branco para separar exibicao da pergunta.
- L49: Limpa a area da pergunta com [Funcao clear_question_area](#funcao-clear_question_area-jogo-c).
- L50: Posiciona o cursor para o enunciado.
- L51: Imprime o enunciado.
- L52: Posiciona o cursor para a alternativa A.
- L53: Imprime a alternativa A.
- L54: Posiciona o cursor para a alternativa B.
- L55: Imprime a alternativa B.
- L56: Posiciona o cursor para a alternativa C.
- L57: Imprime a alternativa C.
- L58: Posiciona o cursor para a alternativa D.
- L59: Imprime a alternativa D.
- L60: Linha em branco para separar o prompt.
- L61: Limpa a linha do prompt.
- L62: Posiciona o cursor para o prompt.
- L63: Imprime o texto do prompt.
- L64: Linha em branco para separar leitura.
- L65: Declara `resposta` com valor nulo.
- L66: Le um caractere do usuario; retorna 0 se falhar.
- L67: Retorna 0 quando a leitura nao ocorre.
- L68: Linha em branco para separar normalizacao.
- L69: Converte a resposta para minuscula.
- L70: Converte a resposta correta para minuscula.
- L71: Compara e retorna 1 se a resposta estiver correta.
- L72: Fecha o bloco da funcao.

<a id="funcao-executar_partida-jogo-c"></a>
### Funcao executar_partida (jogo.c)

Codigo da funcao:
```c
Session executar_partida()
{
    Session s = {0};
    int remaining_guesses = INITIAL_GUESSES;
    int venceu = 0;
    s.target = sortearNumero(); // RF01 [cite: 19]

    clear_screen();
    print_title_bar("LuckyGuess - Jogo", BOLD_BLUE, BOLD_WHITE);

    // Simulação de timestamp simples [cite: 30]
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(s.timestamp, sizeof(s.timestamp), "%Y-%m-%d %H:%M:%S", &tm);

    gotoxy(4, 5);
    print_colored("Novo jogo iniciado! Tente adivinhar o numero.", WHITE);
    gotoxy(4, 6);
    print_colored("Digite um palpite entre 1 e 100.", DARK_GRAY);

    while (remaining_guesses > 0 && s.attempts_count < MAX_GUESSES)
    {
        int palpite;
        clear_line(8);
        gotoxy(4, 8);
        printf("Palpite (tentativa %d, chances %d): ", s.attempts_count + 1, remaining_guesses);
        scanf("%d", &palpite);

        if (palpite < 1 || palpite > 100)
        {
            clear_line(10);
            gotoxy(4, 10);
            print_colored("Palpite invalido. Tente novamente.", YELLOW);
            clear_question_area();
            continue; // RF02 [cite: 21]
        }

        s.guesses[s.attempts_count++] = palpite;
        remaining_guesses--;

        if (palpite == s.target)
        {
            venceu = 1;
            break;
        }

        int acertou = perguntar_para_dica();
        clear_line(10);
        gotoxy(4, 10);

        if (palpite < s.target)
            s.low_count++;
        else
            s.high_count++;

        if (acertou)
        {
            remaining_guesses++;
            if (palpite < s.target)
                print_colored("Muito baixo!", BRIGHT_CYAN);
            else
                print_colored("Muito alto!", BRIGHT_MAGENTA);
        }
        else
        {
            print_colored("Resposta incorreta. Sem dica.", YELLOW);
        }

        clear_question_area();
    }

    show_end_screen(&s, venceu);

    salvar_sessao(s); // RF03 [cite: 27]
    return s;
}
```
> Explicacao: Executa a partida com 3 chances iniciais e ajusta as chances conforme as perguntas.
- Controle de `remaining_guesses` limita o numero de palpites.
- Cada palpite valido consome 1 chance; resposta correta devolve 1.
- `low_count`/`high_count` sao contabilizados mesmo sem dica exibida.
- Se as chances chegam a zero sem acerto, a tela final mostra game over.
- A tela final e exibida por [Funcao show_end_screen](#funcao-show_end_screen-jogo-c).

## Arquivo ui_menu.h

### Linha a linha
- L1: Inicia o include guard de ui_menu.h.
- L2: Define a macro do include guard.
- L3: Linha em branco para separar prototipos.
- L4: Declara `run_main_menu`.
- L5: Linha em branco para separar do fim do cabecalho.
- L6: Encerra o include guard.

<a id="funcao-run_main_menu-ui_menu-h"></a>
### Funcao run_main_menu (declaracao em ui_menu.h)

Codigo da funcao:
```c
int run_main_menu(void);
```
> Explicacao: Linha 4 declara a funcao principal do menu, definida em [Funcao run_main_menu](#funcao-run_main_menu-ui_menu-c).

## Arquivo ui_menu.c

### Linhas fora de funcoes
- L1: Inclui stdio.h para `printf` e `snprintf`.
- L2: Inclui string.h para `strlen` e `memcpy`.
- L3: Inclui conio.h para `_getch` e `_kbhit`.
- L4: Inclui synchapi.h para `Sleep`.
- L5: Linha em branco para separar includes.
- L6: Inclui ui_menu.h.
- L7: Inclui jogo.h para [Funcao executar_partida](#funcao-executar_partida-jogo-c).
- L8: Inclui analise.h para [Funcao processar_historico](#funcao-processar_historico-analise-c).
- L9: Inclui historico.h para [Funcao carregar_historico](#funcao-carregar_historico-historico-c).
- L10: Inclui text_utils.h (biblioteca externa nao detalhada aqui).
- L11: Linha em branco para separar definicoes.
- L12: Inicia o enum `MenuOption`.
- L13: Abre o bloco do enum.
- L14: Define `MENU_PLAY` com valor 0.
- L15: Define `MENU_HISTORY`.
- L16: Define `MENU_EXIT`.
- L17: Define `MENU_COUNT`.
- L18: Fecha o enum.
- L19: Linha em branco para separar constantes.
- L20: Declara o vetor `MENU_ITEMS`.
- L21: Define o texto do item Jogar.
- L22: Define o texto do item Historico.
- L23: Define o texto do item Sair e fecha o vetor.
- L24: Linha em branco para separar layout.
- L25: Define largura do console.
- L26: Define largura dos itens do menu.
- L27: Define a linha Y inicial do menu.
- L28: Define maximo de sessoes.
- L29: Define maximo de linhas na tabela.
- L30: Define espaco entre colunas.
- L31: Linha em branco para separar teclas.
- L32: Define constante para tecla para cima.
- L33: Define constante para tecla para baixo.
- L34: Define constante para Enter.
- L35: Define constante para nenhuma tecla.
- L36: Linha em branco para separar funcoes.
- L60: Linha em branco para separar funcoes.
- L78: Linha em branco para separar funcoes.
- L110: Linha em branco para separar funcoes.
- L131: Linha em branco para separar funcoes.
- L151: Linha em branco para separar funcoes.
- L170: Linha em branco para separar funcoes.
- L178: Linha em branco para separar funcoes.
- L214: Linha em branco para separar funcoes.
- L276: Linha em branco para separar funcoes.

<a id="funcao-print_cell-ui_menu-c"></a>
### Funcao print_cell (ui_menu.c)

Codigo da funcao:
```c
static void print_cell(const char *text, int width, int alignment, const char *color)
{
    char buf[128];
    const char *src = text ? text : "";
    size_t len = strlen(src);

    if ((int)len > width)
    {
        int copy_len = width < (int)sizeof(buf) - 1 ? width : (int)sizeof(buf) - 1;
        memcpy(buf, src, (size_t)copy_len);
        buf[copy_len] = '\0';
        src = buf;
    }

    if (!color)
        color = WHITE;
    fputs(color, stdout);
    if (alignment == ALIGN_RIGHT)
        printf("%*s", width, src);
    else
        printf("%-*s", width, src);
    fputs(RESET, stdout);
}
```
> Explicacao: Linhas 37-59 imprimem uma celula de tabela com alinhamento e cor.
- L37: Declara funcao estatica para imprimir celula.
- L38: Abre o bloco da funcao.
- L39: Declara buffer temporario.
- L40: Usa `text` ou string vazia se for nulo.
- L41: Calcula o comprimento do texto.
- L42: Linha em branco para separar validacao.
- L43: Verifica se o texto excede a largura.
- L44: Abre o bloco do `if`.
- L45: Calcula o tamanho de copia permitido.
- L46: Copia o texto truncado.
- L47: Finaliza o buffer com `\0`.
- L48: Aponta `src` para o buffer truncado.
- L49: Fecha o bloco do `if`.
- L50: Linha em branco para separar cor.
- L51: Usa cor padrao se `color` for nulo.
- L52: Atribui a cor padrao.
- L53: Imprime a sequencia de cor no stdout.
- L54: Verifica se o alinhamento e a direita.
- L55: Imprime alinhado a direita.
- L56: Caso contrario, entra no `else`.
- L57: Imprime alinhado a esquerda.
- L58: Reseta a cor.
- L59: Fecha o bloco da funcao.

<a id="funcao-print_hyphen_row-ui_menu-c"></a>
### Funcao print_hyphen_row (ui_menu.c)

Codigo da funcao:
```c
static void print_hyphen_row(int cols, const int widths[], const char *color)
{
    if (!color)
        color = DARK_GRAY;
    fputs(color, stdout);
    for (int i = 0; i < cols; ++i)
    {
        for (int j = 0; j < widths[i]; ++j)
            putchar('-');
        if (i < cols - 1)
        {
            for (int k = 0; k < TABLE_GAP; ++k)
                putchar(' ');
        }
    }
    fputs(RESET, stdout);
}
```
> Explicacao: Linhas 61-77 imprimem a linha de separacao da tabela.
- L61: Declara funcao estatica para imprimir tracos.
- L62: Abre o bloco da funcao.
- L63: Usa cor padrao se `color` for nulo.
- L64: Define a cor padrao.
- L65: Imprime a sequencia de cor.
- L66: Loop pelas colunas.
- L67: Abre o bloco do `for`.
- L68: Loop pelos caracteres de largura da coluna.
- L69: Imprime um traco.
- L70: Verifica se nao e a ultima coluna.
- L71: Abre o bloco do `if`.
- L72: Loop para imprimir o gap.
- L73: Imprime espaco do gap.
- L74: Fecha o bloco do `if`.
- L75: Fecha o bloco do `for`.
- L76: Reseta a cor.
- L77: Fecha o bloco da funcao.

<a id="funcao-draw_hyphen_table-ui_menu-c"></a>
### Funcao draw_hyphen_table (ui_menu.c)

Codigo da funcao:
```c
static void draw_hyphen_table(int x, int y, int cols,
                              const char *headers[],
                              const int widths[],
                              int rows,
                              const char *data[rows][cols],
                              const int *alignments)
{
    gotoxy(x, y);
    for (int i = 0; i < cols; ++i)
    {
        int align = alignments ? alignments[i] : ALIGN_LEFT;
        print_cell(headers[i], widths[i], align, BOLD_WHITE);
        if (i < cols - 1)
            printf("%*s", TABLE_GAP, "");
    }

    gotoxy(x, y + 1);
    print_hyphen_row(cols, widths, DARK_GRAY);

    for (int r = 0; r < rows; ++r)
    {
        gotoxy(x, y + 2 + r);
        for (int c = 0; c < cols; ++c)
        {
            int align = alignments ? alignments[c] : ALIGN_LEFT;
            print_cell(data[r][c], widths[c], align, WHITE);
            if (c < cols - 1)
                printf("%*s", TABLE_GAP, "");
        }
    }
}
```
> Explicacao: Linhas 79-109 desenham uma tabela com cabecalho e linhas de dados.
- L79: Declara a funcao com parametros de layout e dados.
- L80: Continua a lista de parametros (cabecalhos).
- L81: Continua a lista de parametros (larguras).
- L82: Continua a lista de parametros (numero de linhas).
- L83: Continua a lista de parametros (dados).
- L84: Continua a lista de parametros (alinhamentos).
- L85: Abre o bloco da funcao.
- L86: Move o cursor para o topo da tabela.
- L87: Loop pelas colunas do cabecalho.
- L88: Abre o bloco do `for`.
- L89: Seleciona o alinhamento da coluna.
- L90: Chama [Funcao print_cell](#funcao-print_cell-ui_menu-c) para imprimir o cabecalho.
- L91: Se nao e a ultima coluna, imprime gap.
- L92: Imprime espacos do gap.
- L93: Fecha o bloco do `for`.
- L94: Linha em branco para separar linha de tracos.
- L95: Move o cursor para a linha de tracos.
- L96: Chama [Funcao print_hyphen_row](#funcao-print_hyphen_row-ui_menu-c).
- L97: Linha em branco para separar linhas de dados.
- L98: Loop pelas linhas de dados.
- L99: Abre o bloco do `for` externo.
- L100: Move o cursor para a linha de dados.
- L101: Loop pelas colunas.
- L102: Abre o bloco do `for` interno.
- L103: Seleciona alinhamento da coluna.
- L104: Chama [Funcao print_cell](#funcao-print_cell-ui_menu-c) para a celula.
- L105: Se nao e a ultima coluna, imprime gap.
- L106: Imprime espacos do gap.
- L107: Fecha o bloco do `for` interno.
- L108: Fecha o bloco do `for` externo.
- L109: Fecha o bloco da funcao.

<a id="funcao-read_key_nonblocking-ui_menu-c"></a>
### Funcao read_key_nonblocking (ui_menu.c)

Codigo da funcao:
```c
static int read_key_nonblocking(void)
{
    if (!_kbhit())
        return KEY_NONE;

    int ch = _getch();
    if (ch == 224 || ch == 0)
    {
        int ext = _getch();
        if (ext == 72)
            return KEY_UP;
        if (ext == 80)
            return KEY_DOWN;
    }

    if (ch == '\r')
        return KEY_ENTER;

    return KEY_NONE;
}
```
> Explicacao: Linhas 111-130 leem teclas do teclado sem bloquear o programa.
- L111: Declara funcao estatica para leitura nao bloqueante.
- L112: Abre o bloco da funcao.
- L113: Se nao houver tecla, retorna `KEY_NONE`.
- L114: Retorna imediatamente quando nao ha entrada.
- L115: Linha em branco para separar leitura.
- L116: Le um caractere com `_getch`.
- L117: Se for tecla especial, entra no bloco.
- L118: Abre o bloco do `if`.
- L119: Le o codigo estendido.
- L120: Se for seta para cima, retorna `KEY_UP`.
- L121: Retorna o valor da tecla cima.
- L122: Se for seta para baixo, retorna `KEY_DOWN`.
- L123: Retorna o valor da tecla baixo.
- L124: Fecha o bloco do `if`.
- L125: Linha em branco para separar Enter.
- L126: Se for Enter, retorna `KEY_ENTER`.
- L127: Retorna o valor de Enter.
- L128: Linha em branco para separar retorno padrao.
- L129: Retorna `KEY_NONE` para outras teclas.
- L130: Fecha o bloco da funcao.

<a id="funcao-draw_menu_frame-ui_menu-c"></a>
### Funcao draw_menu_frame (ui_menu.c)

Codigo da funcao:
```c
static void draw_menu_frame(void)
{
    int menu_x = (CONSOLE_WIDTH - MENU_ITEM_WIDTH) / 2;
    const char *hint = "Use as setas para navegar e Enter para selecionar.";
    int hint_x = (CONSOLE_WIDTH - (int)strlen(hint)) / 2;

    clear_screen();
    print_title_bar("LuckyGuess - Menu Principal", BOLD_BLUE, BOLD_WHITE);

    for (int i = 0; i < MENU_COUNT; ++i)
    {
        gotoxy(menu_x, MENU_Y + i * 2);
        printf("  %-20s  ", MENU_ITEMS[i]);
    }

    gotoxy(hint_x, MENU_Y + MENU_COUNT * 2 + 2);
    print_colored(hint, DARK_GRAY);
    fflush(stdout);
}
```
> Explicacao: Linhas 132-150 desenham a moldura inicial do menu principal.
- L132: Declara funcao estatica.
- L133: Abre o bloco da funcao.
- L134: Calcula a posicao X do menu para centralizar.
- L135: Define a mensagem de dica.
- L136: Calcula a posicao X da dica.
- L137: Linha em branco para separar o desenho.
- L138: Limpa a tela usando TextUtils.
- L139: Desenha a barra de titulo usando TextUtils.
- L140: Linha em branco para separar itens.
- L141: Loop pelos itens do menu.
- L142: Abre o bloco do `for`.
- L143: Posiciona o cursor para cada item.
- L144: Imprime o texto do item.
- L145: Fecha o bloco do `for`.
- L146: Linha em branco para separar a dica.
- L147: Posiciona o cursor para a dica.
- L148: Imprime a dica em cor cinza.
- L149: Forca o flush do stdout.
- L150: Fecha o bloco da funcao.

<a id="funcao-draw_menu_options-ui_menu-c"></a>
### Funcao draw_menu_options (ui_menu.c)

Codigo da funcao:
```c
static void draw_menu_options(int selected)
{
    int menu_x = (CONSOLE_WIDTH - MENU_ITEM_WIDTH) / 2;

    for (int i = 0; i < MENU_COUNT; ++i)
    {
        gotoxy(menu_x, MENU_Y + i * 2);
        if (i == selected)
        {
            printf(BACKGROUND_WHITE BOLD_BLUE "  %-20s  " RESET, MENU_ITEMS[i]);
        }
        else
        {
            printf("  %-20s  ", MENU_ITEMS[i]);
        }
    }
    fflush(stdout);
}
```
> Explicacao: Linhas 152-169 destacam visualmente o item de menu selecionado.
- L152: Declara funcao estatica.
- L153: Abre o bloco da funcao.
- L154: Calcula a posicao X para centralizar.
- L155: Linha em branco para separar o loop.
- L156: Loop pelos itens do menu.
- L157: Abre o bloco do `for`.
- L158: Posiciona o cursor para o item atual.
- L159: Verifica se o item e o selecionado.
- L160: Abre o bloco do `if`.
- L161: Imprime o item com destaque de cor.
- L162: Fecha o bloco do `if`.
- L163: Caso contrario, entra no `else`.
- L164: Abre o bloco do `else`.
- L165: Imprime o item sem destaque.
- L166: Fecha o bloco do `else`.
- L167: Fecha o bloco do `for`.
- L168: Forca o flush do stdout.
- L169: Fecha o bloco da funcao.

<a id="funcao-wait_for_key-ui_menu-c"></a>
### Funcao wait_for_key (ui_menu.c)

Codigo da funcao:
```c
static void wait_for_key(const char *message)
{
    if (!message)
        message = "Pressione qualquer tecla para voltar.";
    print_status_bar(message, BOLD_WHITE);
    _getch();
}
```
> Explicacao: Linhas 171-177 exibem uma mensagem e aguardam uma tecla.
- L171: Declara funcao estatica.
- L172: Abre o bloco da funcao.
- L173: Se mensagem for nula, usa texto padrao.
- L174: Define a mensagem padrao.
- L175: Mostra a mensagem na barra de status (TextUtils).
- L176: Aguarda uma tecla com `_getch`.
- L177: Fecha o bloco da funcao.

<a id="funcao-show_session_summary-ui_menu-c"></a>
### Funcao show_session_summary (ui_menu.c)

Codigo da funcao:
```c
static void show_session_summary(Session s)
{
    clear_screen();
    print_title_bar("LuckyGuess - Resultado", BOLD_BLUE, BOLD_WHITE);

    const int cols = 5;
    const char *headers[] = {"Data/Hora", "Alvo", "Tentativas", "Baixo", "Alto"};
    const int alignments[] = {ALIGN_LEFT, ALIGN_RIGHT, ALIGN_RIGHT, ALIGN_RIGHT, ALIGN_RIGHT};
    const int widths[] = {19, 6, 10, 6, 6};

    char buf_target[16];
    char buf_attempts[16];
    char buf_low[16];
    char buf_high[16];

    snprintf(buf_target, sizeof(buf_target), "%d", s.target);
    snprintf(buf_attempts, sizeof(buf_attempts), "%d", s.attempts_count);
    snprintf(buf_low, sizeof(buf_low), "%d", s.low_count);
    snprintf(buf_high, sizeof(buf_high), "%d", s.high_count);

    const char *data[1][5] = {{s.timestamp, buf_target, buf_attempts, buf_low, buf_high}};

    int table_width = 0;
    for (int i = 0; i < cols; ++i)
        table_width += widths[i];
    table_width += (cols - 1) * TABLE_GAP;

    int table_x = (CONSOLE_WIDTH - table_width) / 2 + 1;
    int table_y = 7;

    draw_hyphen_table(table_x, table_y, cols, headers, widths, 1, data, alignments);

    wait_for_key(NULL);
    clear_screen();
}
```
> Explicacao: Linhas 179-213 mostram o resumo da sessao em formato de tabela.
- L179: Declara funcao estatica.
- L180: Abre o bloco da funcao.
- L181: Limpa a tela.
- L182: Desenha a barra de titulo.
- L183: Linha em branco para separar configuracao.
- L184: Define numero de colunas.
- L185: Define cabecalhos.
- L186: Define alinhamentos.
- L187: Define larguras.
- L188: Linha em branco para separar buffers.
- L189: Declara buffer do alvo.
- L190: Declara buffer de tentativas.
- L191: Declara buffer de baixos.
- L192: Declara buffer de altos.
- L193: Linha em branco para separar conversoes.
- L194: Converte `target` para string.
- L195: Converte `attempts_count` para string.
- L196: Converte `low_count` para string.
- L197: Converte `high_count` para string.
- L198: Linha em branco para separar dados.
- L199: Preenche a matriz de dados.
- L200: Linha em branco para separar calculo de largura.
- L201: Inicializa `table_width`.
- L202: Soma larguras das colunas.
- L203: Continua a soma.
- L204: Soma os gaps entre colunas.
- L205: Linha em branco para separar coordenadas.
- L206: Calcula `table_x` para centralizar.
- L207: Define `table_y`.
- L208: Linha em branco para separar desenho.
- L209: Chama [Funcao draw_hyphen_table](#funcao-draw_hyphen_table-ui_menu-c).
- L210: Linha em branco para separar pausa.
- L211: Chama [Funcao wait_for_key](#funcao-wait_for_key-ui_menu-c).
- L212: Limpa a tela.
- L213: Fecha o bloco da funcao.

<a id="funcao-show_history_screen-ui_menu-c"></a>
### Funcao show_history_screen (ui_menu.c)

Codigo da funcao:
```c
static void show_history_screen(void)
{
    clear_screen();
    print_title_bar("LuckyGuess - Historico", BOLD_BLUE, BOLD_WHITE);

    Session sessoes[MAX_SESSOES];
    int n = carregar_historico(sessoes, MAX_SESSOES);
    if (n <= 0)
    {
        wait_for_key("Nenhuma sessao encontrada.");
        clear_screen();
        return;
    }

    Stats st = processar_historico(sessoes, n);

    char info[128];
    snprintf(info, sizeof(info), "Sessoes: %d | Media: %.2f | Desvio: %.2f",
             n, st.average_attempts, st.stddev_attempts);
    gotoxy(4, 5);
    print_colored(info, BOLD_WHITE);

    const int cols = 5;
    const char *headers[] = {"Data/Hora", "Alvo", "Tentativas", "Baixo", "Alto"};
    const int alignments[] = {ALIGN_LEFT, ALIGN_RIGHT, ALIGN_RIGHT, ALIGN_RIGHT, ALIGN_RIGHT};
    const int widths[] = {19, 6, 10, 6, 6};

    int rows = n < MAX_TABLE_ROWS ? n : MAX_TABLE_ROWS;
    int start = n - rows;

    const char *data[rows][5];
    char cells[rows][5][32];

    for (int i = 0; i < rows; ++i)
    {
        Session *s = &sessoes[start + i];
        snprintf(cells[i][0], sizeof(cells[i][0]), "%s", s->timestamp);
        snprintf(cells[i][1], sizeof(cells[i][1]), "%d", s->target);
        snprintf(cells[i][2], sizeof(cells[i][2]), "%d", s->attempts_count);
        snprintf(cells[i][3], sizeof(cells[i][3]), "%d", s->low_count);
        snprintf(cells[i][4], sizeof(cells[i][4]), "%d", s->high_count);

        for (int c = 0; c < cols; ++c)
        {
            data[i][c] = cells[i][c];
        }
    }

    int table_width = 0;
    for (int i = 0; i < cols; ++i)
        table_width += widths[i];
    table_width += (cols - 1) * TABLE_GAP;

    int table_x = (CONSOLE_WIDTH - table_width) / 2 + 1;
    int table_y = 8;

    draw_hyphen_table(table_x, table_y, cols, headers, widths, rows, data, alignments);

    wait_for_key(NULL);
    clear_screen();
}
```
> Explicacao: Linhas 215-275 exibem o historico e estatisticas das sessoes.
- L215: Declara funcao estatica.
- L216: Abre o bloco da funcao.
- L217: Limpa a tela.
- L218: Desenha a barra de titulo.
- L219: Linha em branco para separar carregamento.
- L220: Declara vetor `sessoes` com tamanho fixo.
- L221: Chama [Funcao carregar_historico](#funcao-carregar_historico-historico-c).
- L222: Verifica se nao ha sessoes.
- L223: Abre o bloco do `if`.
- L224: Mostra mensagem e aguarda tecla.
- L225: Limpa a tela.
- L226: Retorna para sair da funcao.
- L227: Fecha o bloco do `if`.
- L228: Linha em branco para separar analise.
- L229: Chama [Funcao processar_historico](#funcao-processar_historico-analise-c).
- L230: Linha em branco para separar mensagem.
- L231: Declara buffer `info`.
- L232: Monta o texto das estatisticas.
- L233: Continua a lista de argumentos de `snprintf`.
- L234: Posiciona o cursor.
- L235: Imprime a mensagem.
- L236: Linha em branco para separar tabela.
- L237: Define numero de colunas.
- L238: Define cabecalhos.
- L239: Define alinhamentos.
- L240: Define larguras.
- L241: Linha em branco para separar calculo de linhas.
- L242: Calcula quantas linhas mostrar.
- L243: Calcula o indice inicial.
- L244: Linha em branco para separar buffers.
- L245: Declara matriz de ponteiros para dados.
- L246: Declara matriz de buffers para strings.
- L247: Linha em branco para separar preenchimento.
- L248: Loop para preencher linhas da tabela.
- L249: Abre o bloco do `for`.
- L250: Seleciona a sessao atual.
- L251: Copia timestamp para a celula.
- L252: Converte o alvo para string.
- L253: Converte tentativas para string.
- L254: Converte baixos para string.
- L255: Converte altos para string.
- L256: Linha em branco para separar o loop interno.
- L257: Loop pelas colunas para apontar `data`.
- L258: Abre o bloco do `for` interno.
- L259: Associa ponteiro ao buffer.
- L260: Fecha o bloco do `for` interno.
- L261: Fecha o bloco do `for` externo.
- L262: Linha em branco para separar calculo de largura.
- L263: Inicializa `table_width`.
- L264: Soma larguras das colunas.
- L265: Continua a soma.
- L266: Soma gaps entre colunas.
- L267: Linha em branco para separar coordenadas.
- L268: Calcula `table_x`.
- L269: Define `table_y`.
- L270: Linha em branco para separar desenho.
- L271: Chama [Funcao draw_hyphen_table](#funcao-draw_hyphen_table-ui_menu-c).
- L272: Linha em branco para separar pausa.
- L273: Chama [Funcao wait_for_key](#funcao-wait_for_key-ui_menu-c).
- L274: Limpa a tela.
- L275: Fecha o bloco da funcao.

<a id="funcao-run_main_menu-ui_menu-c"></a>
### Funcao run_main_menu (ui_menu.c)

Codigo da funcao:
```c
int run_main_menu(void)
{
    int selected = 0;
    int last_selected = -1;

    draw_menu_frame();
    draw_menu_options(selected);

    while (1)
    {
        int key = read_key_nonblocking();
        if (key == KEY_UP)
        {
            selected = (selected - 1 + MENU_COUNT) % MENU_COUNT;
        }
        else if (key == KEY_DOWN)
        {
            selected = (selected + 1) % MENU_COUNT;
        }
        else if (key == KEY_ENTER)
        {
            if (selected == MENU_PLAY)
            {
                clear_screen();
                Session s = executar_partida();
                show_session_summary(s);
                draw_menu_frame();
                last_selected = -1;
            }
            else if (selected == MENU_HISTORY)
            {
                show_history_screen();
                draw_menu_frame();
                last_selected = -1;
            }
            else if (selected == MENU_EXIT)
            {
                clear_screen();
                return 0;
            }
        }

        if (selected != last_selected)
        {
            draw_menu_options(selected);
            last_selected = selected;
        }

        Sleep(16);
    }
}
```
> Explicacao: Linhas 277-327 controlam a navegacao do menu e disparam as acoes.
- L277: Define a assinatura da funcao.
- L278: Abre o bloco da funcao.
- L279: Define o item inicialmente selecionado.
- L280: Inicializa `last_selected`.
- L281: Linha em branco para separar o desenho inicial.
- L282: Chama [Funcao draw_menu_frame](#funcao-draw_menu_frame-ui_menu-c).
- L283: Chama [Funcao draw_menu_options](#funcao-draw_menu_options-ui_menu-c).
- L284: Linha em branco para separar o loop.
- L285: Loop infinito do menu.
- L286: Abre o bloco do `while`.
- L287: Le uma tecla com [Funcao read_key_nonblocking](#funcao-read_key_nonblocking-ui_menu-c).
- L288: Se tecla for cima, entra no bloco.
- L289: Abre o bloco do `if`.
- L290: Atualiza o item selecionado com wrap-around.
- L291: Fecha o bloco do `if`.
- L292: Se tecla for baixo, entra no bloco.
- L293: Abre o bloco do `else if`.
- L294: Atualiza o item selecionado com wrap-around.
- L295: Fecha o bloco do `else if`.
- L296: Se tecla for Enter, entra no bloco.
- L297: Abre o bloco do `else if`.
- L298: Se opcao Jogar, entra no bloco.
- L299: Abre o bloco do `if`.
- L300: Limpa a tela.
- L301: Chama [Funcao executar_partida](#funcao-executar_partida-jogo-c).
- L302: Chama [Funcao show_session_summary](#funcao-show_session_summary-ui_menu-c).
- L303: Redesenha o menu.
- L304: Reseta `last_selected`.
- L305: Fecha o bloco do `if`.
- L306: Se opcao Historico, entra no bloco.
- L307: Abre o bloco do `else if`.
- L308: Chama [Funcao show_history_screen](#funcao-show_history_screen-ui_menu-c).
- L309: Redesenha o menu.
- L310: Reseta `last_selected`.
- L311: Fecha o bloco do `else if`.
- L312: Se opcao Sair, entra no bloco.
- L313: Abre o bloco do `else if`.
- L314: Limpa a tela.
- L315: Retorna 0 para encerrar o programa.
- L316: Fecha o bloco do `else if`.
- L317: Fecha o bloco do `else if` principal.
- L318: Linha em branco para separar redesenho.
- L319: Se a selecao mudou, entra no bloco.
- L320: Abre o bloco do `if`.
- L321: Redesenha as opcoes.
- L322: Atualiza `last_selected`.
- L323: Fecha o bloco do `if`.
- L324: Linha em branco para separar o delay.
- L325: Aguarda 16 ms usando `Sleep`.
- L326: Fecha o bloco do `while`.
- L327: Fecha o bloco da funcao.

## Arquivo main.c

### Linhas fora de funcoes
- L1: Inclui time.h para `time`.
- L2: Inclui stdlib.h para `srand`.
- L3: Inclui ui_menu.h para [Funcao run_main_menu](#funcao-run_main_menu-ui_menu-c).
- L4: Linha em branco para separar includes do codigo.

<a id="funcao-main-main-c"></a>
### Funcao main (main.c)

Codigo da funcao:
```c
int main()
{
    srand(time(NULL));
    return run_main_menu();
}
```
> Explicacao: Linhas 5-9 iniciam o programa e executam o menu principal.
- L5: Define a funcao `main` (ponto de entrada).
- L6: Abre o bloco da funcao.
- L7: Inicializa o gerador de numeros aleatorios.
- L8: Retorna o resultado de [Funcao run_main_menu](#funcao-run_main_menu-ui_menu-c).
- L9: Fecha o bloco da funcao.
