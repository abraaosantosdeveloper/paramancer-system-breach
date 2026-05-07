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

# Sobre o Jogo
---
## Protagonista: Dex
![Personagem](/assets/game_arts/actors/dex_idle.png)
## Antagonista: The Entity
![Personagem](/assets/game_arts/actors/the_entity.png)


### História
Dex é um jovem que ama matemática e estatística, e ele tem um dom extraordinário: enxergar padrões matemáticos em tudo. Dex começa a perceber
falhas no seu mundo: como se a “matrix” estivesse corrompida. Ele descobre que há uma Entidade corruptora, uma IA que rouba parâmetros do universo e, por
isso, as falhas na “matrix” começaram a aparecer. Seu trabalho, é adivinhar estes parâmetros e devolvê-los à “matrix”, o que enfraquecerá a Entidade e restaurará a
estabilidade do seu universo.

## Fluxo do jogo
A cada rodada, um número aleatório é gerado, e o jogador tem a chance de acertá-lo para ganhar o jogo. Caso nao obtenha sucesso, ele recebe uma pergunta com opçoes de respostas. Se acertar a pergunta, o jogador causa dano ao oponente e recebe uma dica em relaçao ao número gerado pelo sistema que ele está buscando acertar. Caso ele erre a pergunta feita, o personagem protagonista recebe dano, e o jogador ganha mais uma chance de acertar o número alvo, mas sem receber dicas sobre ele.

### Mecânicas adjacentes
> Isto é apenas um conceito; as imagens serão melhoradas.

### Buffers
**Buffer de Hp extra**
![Buffer](/assets/game_arts/buffers/exta_hp.png)
Ao receber, o usuário ganhará uma determinada quantidade de corações extras.

**Buffer de Dano crítico**
![Buffer](/assets/game_arts/buffers/critical.png)
Com este buffer, o usuário dará o dobro de dano ao inimigo.


**Buffer de Imunidade**
![Buffer](/assets/game_arts/buffers/immunity.png)
Com este buffer, ao errar uma pergunta caso o personagem não acerte o número (parâmetro) a ser advinhado, ele não perderá vida.


## Informações técnicas

##### Linguagem de programação utilizada
O jogo utiliza a linguagem de programação C, por ocasião da disciplina de **Programação imperativa e Funcional**, bem como por seu desempenho e controle total de utilização de recursos, tornando-a uma biblioteca extremamente versátil e que consome recursos mínimos de processamento, mantendo um padrão de execução excepcional.

##### Bibliotecas externas
O programa utiliza a biblioteca externa **Raylib** para criação da interface.

## Product Backlog - Jira's Board

![Descrição da imagem](/assets/dev_assets/backlog.png)

![Descrição da imagem](./assets/dev_assets/quadro.png)   

## Histórias do Usuário 
---

### Iniciar Partida
Eu, como jogador, quero iniciar uma partida com um número aleatório oculto, para tentar acertar e testar minha capacidade de adivinhação.

---

### Feedback de Palpite
Eu, como jogador, quero receber feedback se meu palpite está alto ou baixo, para ajustar minha estratégia.

---

### Tentativas Contínuas
Eu, como jogador, quero continuar tentando até acertar o número, para completar o desafio.

---

### Visualizar Pontos de Vida
Eu, como jogador, quero visualizar os pontos de vida restantes meu e do meu adversário.

---

### Tutorial e História do Jogo
Eu, como jogador, quero receber informações sobre como jogar e entender a história que envolve o jogo.

---

### Perguntas e Dicas
Eu, como jogador, quero responder a perguntas curiosas e contextualizadas para desafiar meu nível de conhecimento e conseguir ganhar uma dica sobre meu palpite de número.

---

### Revelar Resposta Correta
Eu, como jogador, quero ser capaz de visualizar qual a resposta correta da pergunta quando eu errar, para que eu possa aprender.

---

### Estatísticas da Sessão
Eu, como jogador, quero ter acesso aos meus dados de média, melhor/pior sessão, desvio, recursão em soma/mín/máx/soma quadrados.

---

### Reiniciar ou Sair da Partida
Eu, como jogador, quero ser capaz de continuar jogando quantas vezes eu quiser, ou sair da partida em qualquer momento.


### Em breve, mais conteúdo
> Fique atento às atualizações futuras.
