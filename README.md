# Pesquisa-tcc

Primeiro trabalho prático da disciplina de AEDS 2, referente ao semestre de 2023/1. Desenvolvido pelos graduandos em Ciência da Computação: **Eduardo Antunes,
Gabriel Duarte, Lucas Fonseca e Pedro Augusto Martins**, da UFV, *campus* Florestal.

Esse *software* consiste em um buscador de palavras-chave em trabalhos de conclusão de curso da universidade. A partir dos arquivos de texto dos trabalhos, gera-se
um índice invertido que detalha o número de ocorrências de cada palavra em cada arquivo, na forma de uma estrutura de árvore [Patricia](https://pt.wikipedia.org/wiki/%C3%81rvore_Patricia).
A implementação usada como referência para essa estrutura de dados foi a do professor Nívio Ziviani, em seu livro [Projeto de Algoritmos](https://www2.dcc.ufmg.br/livros/algoritmos/)
(versão com implementações em C e Pascal). É possível visualizar o índice invertido após a sua geração.

Então, a partir de uma série de palavras-chave fornecidas pelo usuário, a relevância de cada arquivo para aquela busca é calculada pela métrica [TF-IDF](https://pt.wikipedia.org/wiki/Tf%E2%80%93idf),
que serve de critério para a ordenação dos arquivos na apresentação dos resultados. Desse modo, os resultados mais relevantes aparecem primeiro.

Como destaques do programa podem ser citados o suporte completo a caracteres especiais, como os caracteres acentuados da língua portuguesa. Esse suporte se dá por
meio do uso de UTF-32 com a biblioteca `wchar.h`.
