![Logotipo](/assets/game_arts/logo.png)

### A equipe
|Nome|Perfil do Github|
|---|---|
|Abraão Filipi dos Santos -- DEV |[@abraaosantosdeveloper](https://github.com/abraaosantosdeveloper/)||
|Pedro Pessoa -- DEV |[@pedropessoa](https://github.com/Ppan-droid)||
|Dilvanir Aline -- DESIGNER |[@aline](https://github.com/daacm-cele)|||
|Emanoel Alessandro -- DEV |[@emanoel0106](https://github.com/emanoel0106)||
|Marcio Aureliano -- TECH LEAD |[@marcio]()||
|Maria Larysse -- PO |[@mlylp](https://github.com/mlylp)||
---
### Funcionamento do jogo

Protagonista: Dex
Antagonista: The Entity

Dex é um jovem que ama matemática e estatística, e ele tem um dom extraordinário: enxergar padrões matemáticos em tudo. Dex começa a perceber
falhas no seu mundo: como se a “matrix” estivesse corrompida. Ele descobre que há uma Entidade corruptora, uma IA que rouba parâmetros do universo e, por
isso, as falhas na “matrix” começaram a aparecer. Seu trabalho, é adivinhar estes parâmetros e devolvê-los à “matrix”, o que enfraquecerá a Entidade e restaurará a
estabilidade do seu universo.

A cada rodada, um número aleatório é gerado, e o jogador tem a chance de acertá-lo para ganhar o jogo. Caso nao obtenha sucesso, ele recebe uma pergunta com opçoes de respostas. Se acertar a pergunta, o jogador causa dano ao oponente e recebe uma dica em relaçao ao número gerado pelo sistema que ele está buscando acertar. Caso ele erre a pergunta feita, o personagem protagonista recebe dano, e o jogador ganha mais uma chance de acertar o número alvo, mas sem receber dicas sobre ele.

## Informações técnicas

##### Linguagem de programação utilizada
O jogo utiliza a linguagem de programação C, por ocasião da disciplina de **Programação imperativa e Funcional**, bem como por seu desempenho e controle total de utilização de recursos, tornando-a uma biblioteca extremamente versátil e que consome recursos mínimos de processamento, mantendo um padrão de execução excepcional.

##### Bibliotecas externas
O programa utiliza a biblioteca externa **TextUtils**, desenvolvida pelo discente Abraão F. Dos Santos, inicialmente em python, e portada para a linguagem C.

**Link do repositório:** 
Para acessar a biblioteca utilizada e implementá-la em seus projetos, basta clicar no link: [Text Utils Library](https://github.com/abraaosantosdeveloper/text-utils-c-edition/)

**Nota ao usuário:**

>*O repositório original possui termos de uso descritos no arquivo **LICENSE**. Leia-o com atenção antes de utilizar a biblioteca.*

## Product Backlog - Jira's Board

![Descrição da imagem](./assets/backlog.png)

![Descrição da imagem](./assets/quadro.png)   

## Como compilar

Recomendado:
```bash
gcc -std=c11 -Wall -Wextra -o luckyguess.exe *.c
```

Alternativa (lista explicita de arquivos):
```bash
gcc -std=c11 -Wall -Wextra -o luckyguess.exe main.c ui_menu.c jogo.c analise.c historico.c perguntas.c sorteio.c text_utils.c
```

