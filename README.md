# Teoria dos Grafos

Biblioteca desenvolvida em C++ para leitura, análise e execução de algoritmos em grafos com e sem peso para a disciplina de Teoria dos Grafos - SIN 430.

## Funcionalidades

- Representação de grafos sem peso por matriz ou lista de adjacência.
- Busca em largura (BFS), busca em profundidade (DFS), componentes conexas e estimativa do diâmetro.
- Algoritmo de Dijkstra em grafos ponderados, com fila baseada em vetor ou heap binário.
- Estudo de caso com uma rede de colaboração e cálculo de caminhos entre pesquisadores.
- Benchmark para comparar as implementações de Dijkstra com vetor e heap.
- Geração de arquivos com informações dos grafos, árvores de busca, componentes e caminhos mínimos.

## Estrutura do projeto


```text
| main.cpp                              Ponto de entrada da aplicação interativa
| algoritmos.cpp / algoritmos.hpp       BFS, DFS, componentes, diâmetro aproximado e Dijkstra 
| arquivos.cpp / arquivos.hpp           Leitura das instâncias, criação de pastas e gravação de resultados
| grafosempeso.cpp / grafosempeso.hpp   Menus e operações para grafos sem peso. 
| grafocompeso.cpp / grafocompeso.hpp   Menus, Dijkstra e estudo da rede de colaboração.
| benchmark_dijkstra.cpp                Executável para comparar as filas do Dijkstra. 
```

## Obter as instâncias

Baixe o arquivo com todas as instâncias pelo [release do projeto](https://github.com/cristeer/teoria-dos-grafos/releases/download/instancias/instancias.rar) e extraia seu conteúdo para uma pasta chamada `instancias` na raiz do projeto.

```text
instancias/
├── grafo_1.txt
├── ...
├── grafo_6.txt
├── grafo_W_1.txt
├── ...
├── grafo_W_5.txt
├── rede_colaboracao.txt
└── rede_colaboracao_vertices.txt
```

## Compilar e executar

```powershell
g++ -std=c++17 main.cpp algoritmos.cpp arquivos.cpp grafosempeso.cpp grafocompeso.cpp -o main.exe
.\main.exe
```

### Benchmark do Dijkstra

```powershell
g++ -std=c++17 benchmark_dijkstra.cpp algoritmos.cpp arquivos.cpp -o benchmark_dijkstra.exe
.\benchmark_dijkstra.exe
```

## Formato dos arquivos de leitura

Cada arquivo de grafo começa com a quantidade de vértices. As linhas seguintes representam arestas `u, v` com vértices numerados de `1` até `N`:

```text
4
1 2
2 3
3 4
```

Para grafos ponderados, cada linha pode incluir o peso `u, v, peso`:

```text
4
1 2 3.5
2 3 1
3 4 2.25
```

O arquivo `rede_colaboracao_vertices.txt` associa IDs a nomes, com um registro por linha no formato `id,nome`; ele é usado junto com `rede_colaboracao.txt` no estudo de caso.

## Arquivos gerados

A aplicação cria as pastas de saída automaticamente:

- `Saidas/Listas/`: informações, árvores BFS/DFS e componentes para a representação por lista.
- `Saidas/Matriz/`: os mesmos resultados para a representação por matriz.
- `Saidas/Dijkstra/Vetor/` e `Saidas/Dijkstra/Heap/`: árvores de caminhos mínimos.
- `resultados_rede_colaboracao.txt`: resultados do estudo de caso.
- `resultados_dijkstra.txt`: resultados do benchmark.

## Autores

- Samuel Criter de Oliveira Aquino
- Lucas Resler Souza