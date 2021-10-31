# cg_projeto1 - Flappy Bird

Atividade desenvolvida para a disciplina de computação Gráfica da UFABC - ministrada pelos docentes Bruno Augusto e Harlen Batagelo.

## Como foi feito
  Seguindo exemplos passados em aula, foi possível iniciar um esboço de jogo simples em 2D. Por falta de conhecimento técnico por parte do discente, grande
  parte do que foi visto em aula foi reaproveitado e foi pensado um jogo simples para ser desenvolvido e compreendido.
  
  Com isso em mente, foi decidido desenvolver um jogo no estilo Flappy Bird.
  
  ![flappy](images/flappy_bird.jpg)
  
  
  A princípio o que parecia ser extremamente simples, se tornou mais complexo do que o previsto.
  
  O primeiro ponto que era desejado ao desenvolver o jogo, seria ao menos renderizar as imagens representado o pássaro, e os canos (também incluir um plano de fundo diferente). Após um tempo, foi possível carregar a imagem para ser utilizada no plano de fundo. Mas não foi possível pela parte discente deixá-la apenas no fundo, isto é, sem sobrepor os elementos restantes da tela. Sem conseguir renderizar o plano de fundo, os outros objetos também foram deixados como formas geométricas.
  
  Deixando essa parte de lado foi prosseguido para o desenvolvimento dos objetos de maneira mais visual. Seguindo o padrão Flappy Bird, a tela, e os objetos possuem cores mais vivas, diferentes do jogo Asteroids desenvolvido como exemplo. A parte de desenhar os objetos não foi tão trabalhosa, mas ocorreu um evento inesperado.
  O intuito era ter deixado uma lista de canos desenhados na tela bem menor que o objetivo final (ex. desenhar 5 canos por vez invés de 100). Mas ocorreu algum problema ao tentar implementar a função de checagem dos canos fora da tela, que ela acabava desconfigurando os que ainda estavam por aparecer e travava a pontuação do jogo.
  Isso é um "problema" que deverá ser corrigido futuramente.
  
 Com os objetos desenhados, bastou fazer a parte de detecção de colisões. Foram feitas várias tentativas de utilizar os mesmos métodos que os professores deixaram disponíveis, mas mesmo tentando detectar a distância com a GLM, o raio considerado para a colisão estava muito alto (mesmo que tentasse reduzir multiplicando por uma constante, e acabou perdendo o sentido quando a constante teste ficou com um valor menor que 0.1). Portanto foi decidido uma abordagem mais simples para o jogo proposto. Calcular a distância do pássaro a qualquer um dos canos pelo eixo X *e* pelo eixo Y, assim quando as duas distâncias juntas apresentam um risco ao pássaro, a colisão é tida como verdadeira.
 
 Por fim, alguns ajustes e refinamentos foram feitos para garantir que o jogo funcionasse sem problemas (pelo menos na versão local testada, no navegador houve um pouco de lentidão no jogo).
 
 ## Resultado Final
 
