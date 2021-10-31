# cg_projeto1 - Flappy Bird

Atividade desenvolvida para a disciplina de computação Gráfica da UFABC - ministrada pelos docentes Bruno Augusto e Harlen Batagelo.

## Como foi feito
  Seguindo exemplos passados em aula, foi possível iniciar um esboço de jogo simples em 2D. Por falta de conhecimento técnico por parte do discente, grande
  parte do que foi visto em aula foi reaproveitado e foi pensado um jogo simples para ser desenvolvido e compreendido.
  
  Com isso em mente, foi decidido desenvolver um jogo no estilo Flappy Bird.
  
  ![flappy](images/flappy_bird.jpg)
  
  
  A princípio o que parecia ser extremamente simples, se tornou mais complexo do que o previsto.
  
  O primeiro ponto que era desejado ao desenvolver o jogo, seria ao menos renderizar as imagens representado o pássaro, e os canos (também incluir um plano de fundo diferente). Após um tempo, foi possível carregar a imagem para ser utilizada no plano de fundo, créditos especiais a todos que desenvolveram a biblioteca de cabeçalho único STB, encontrada nesse link - https://github.com/nothings/stb/blob/master/stb_image.h
  Contudo, mesmo com essa biblioteca não foi possível pela parte discente deixá-la apenas no fundo, isto é, sem sobrepor os elementos restantes da tela. Sem conseguir renderizar o plano de fundo, os outros objetos também foram deixados como formas geométricas.
  
  Deixando essa parte de lado foi prosseguido para o desenvolvimento dos objetos de maneira mais visual. Seguindo o padrão Flappy Bird, a tela, e os objetos possuem cores mais vivas, diferentes do jogo Asteroids desenvolvido como exemplo. A parte de desenhar os objetos não foi tão trabalhosa, mas ocorreu um evento inesperado.
  O intuito era ter deixado uma lista de canos desenhados na tela bem menor que o objetivo final (ex. desenhar 5 canos por vez invés de 100). Mas ocorreu algum problema ao tentar implementar a função de checagem dos canos fora da tela, que ela acabava desconfigurando os que ainda estavam por aparecer e travava a pontuação do jogo.
  Isso é um "problema" que deverá ser corrigido futuramente.
  
 Com os objetos desenhados, bastou fazer a parte de detecção de colisões. Foram feitas várias tentativas de utilizar os mesmos métodos que os professores deixaram disponíveis, mas mesmo tentando detectar a distância com a GLM, o raio considerado para a colisão estava muito alto (mesmo que tentasse reduzir multiplicando por uma constante, e acabou perdendo o sentido quando a constante teste ficou com um valor menor que 0.1). Portanto foi decidido uma abordagem mais simples para o jogo proposto. Calcular a distância do pássaro a qualquer um dos canos pelo eixo X *e* pelo eixo Y, assim quando as duas distâncias juntas apresentam um risco ao pássaro, a colisão é tida como verdadeira.

 Um problema encontrado com essa abordagem é que a percisão de colisão é extremamente assertiva para casos de colisão frontal, já se o passáro passou da metade do cano, a detecção de colisão tem um desempenho inferior.
 
 Por fim, alguns ajustes e refinamentos foram feitos para garantir que o jogo funcionasse sem problemas (pelo menos na versão local testada, no navegador houve lentidão no jogo - testado no navegador microsoft edge, a velocidade com que o passáro sobe ou desce é muito inferior a de quando testada localmente).
 
 ## Resultado Final
O Resultado final ficou uma aplicação bem simples, simulando quase que inteiramente o jogo completo Flappy Bird. Como pode ser visto abaixo.

 ### Tela GameOver

  ![gameover](images/gameover.png)

 ### Tela enquanto se está parado

  ![gameover](images/rest.png)

 ### Tela enquanto está jogando

  ![gameover](images/playing.png)

Um detalhe é que nessa versão do jogo, é possível se movimentar para cima e para baixo também invés de aguardar a derrota sem poder fazer nada.

### Desenvolvido por
Thiago Silva das Mercês RA - 1120181014