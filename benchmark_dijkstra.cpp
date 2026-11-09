#include "algoritmos.hpp"

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <vector>

namespace {
constexpr int MAX_AMOSTRAS = 1;

struct opcoes_benchmark {
    int k = MAX_AMOSTRAS;
    int limite_vertices = 10000000;
    int limite_minutos = 60;
    uint32_t semente = random_device{}();
    bool somente_heap = false;
    string arquivo_saida = "resultados_dijkstra.txt";
    vector<string> grafos;
};

void imprimir_uso(const char* programa) {
    cout << "Uso: " << programa
         << " [--k 1..20] [--seed numero] [--max-vertices numero]"
            " [--time-limit-minutes numero]"
            " [--heap-only] [--output arquivo.txt] [arquivo_grafo ...]\n"
         << "Sem arquivos informados, usa instancias/grafo_W_1.txt ate"
            " instancias/grafo_W_5.txt.\n"
         << "--max-vertices 0 remove o limite (nao recomendado para a fila"
            " por vetor em grafos grandes).\n"
         << "--time-limit-minutes define o tempo maximo por grafo"
            " (padrao: 60 minutos).\n"
         << "--heap-only executa somente a fila heap, sem dividir o limite"
            " com o vetor.\n";
}

bool ler_opcoes(int argc, char* argv[], opcoes_benchmark& opcoes) {
    for (int i = 1; i < argc; ++i) {
        const string argumento = argv[i];
        if (argumento == "--heap-only") {
            opcoes.somente_heap = true;
            continue;
        }
        if (argumento == "--k" || argumento == "--seed" ||
            argumento == "--max-vertices" ||
            argumento == "--time-limit-minutes" || argumento == "--output") {
            if (i + 1 >= argc) {
                cerr << "Falta o valor para " << argumento << ".\n";
                return false;
            }
            const string valor = argv[++i];
            try {
                if (argumento == "--k") {
                    size_t lidos = 0;
                    const int k = stoi(valor, &lidos);
                    if (lidos != valor.size() || k < 1 || k > MAX_AMOSTRAS) {
                        cerr << "--k deve estar entre 1 e " << MAX_AMOSTRAS << ".\n";
                        return false;
                    }
                    opcoes.k = k;
                } else if (argumento == "--seed") {
                    size_t lidos = 0;
                    const unsigned long long semente = stoull(valor, &lidos);
                    if (lidos != valor.size() || semente > UINT32_MAX) {
                        cerr << "--seed deve estar entre 0 e " << UINT32_MAX << ".\n";
                        return false;
                    }
                    opcoes.semente = static_cast<uint32_t>(semente);
                } else if (argumento == "--max-vertices") {
                    size_t lidos = 0;
                    const int limite = stoi(valor, &lidos);
                    if (lidos != valor.size() || limite < 0) {
                        cerr << "--max-vertices deve ser maior ou igual a zero.\n";
                        return false;
                    }
                    opcoes.limite_vertices = limite;
                } else if (argumento == "--time-limit-minutes") {
                    size_t lidos = 0;
                    const int limite = stoi(valor, &lidos);
                    if (lidos != valor.size() || limite < 1 || limite > 1440) {
                        cerr << "--time-limit-minutes deve estar entre 1 e 1440.\n";
                        return false;
                    }
                    opcoes.limite_minutos = limite;
                } else {
                    opcoes.arquivo_saida = valor;
                }
            } catch (const exception&) {
                cerr << "Valor invalido para " << argumento << ": " << valor << ".\n";
                return false;
            }
        } else if (!argumento.empty() && argumento[0] == '-') {
            cerr << "Opcao desconhecida: " << argumento << ".\n";
            return false;
        } else {
            opcoes.grafos.push_back(argumento);
        }
    }

    if (opcoes.grafos.empty()) {
        for (int i = 1; i <= 5; ++i) {
            opcoes.grafos.push_back("instancias/grafo_W_" + to_string(i) + ".txt");
        }
    }
    return true;
}

bool ler_quantidade_vertices(const string& caminho, int& vertices) {
    ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o grafo: " << caminho << "\n";
        return false;
    }
    if (!(arquivo >> vertices) || vertices <= 0) {
        cerr << "Quantidade de vertices invalida em: " << caminho << "\n";
        return false;
    }
    return true;
}

bool distancias_equivalentes(const vector<double>& a, const vector<double>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 1; i < a.size(); ++i) {
        if (isinf(a[i]) || isinf(b[i])) {
            if (a[i] != b[i]) return false;
        } else if (abs(a[i] - b[i]) >
                   1e-9 * max(1.0, max(abs(a[i]), abs(b[i])))) {
            return false;
        }
    }
    return true;
}

bool executar_amostra(int origem, int vertices, const lista_adj_ponderada& adj,
                      int tipo_fila, vector<double>& dist,
                      chrono::nanoseconds& duracao,
                      chrono::steady_clock::time_point prazo) {
    vector<int> pai;
    fila_distancias fila;
    if (tipo_fila == FILA_HEAP) fila_heap_iniciar(fila, vertices);
    else                        fila_vetor_iniciar(fila, vertices);

    const auto inicio = chrono::steady_clock::now();
    if (inicio >= prazo) {
        duracao = chrono::nanoseconds{0};
        return false;
    }
    fila.prazo = prazo;
    fila.limite_tempo_ativo = true;
    dijkstra_generico(origem, vertices, adj, fila, pai);
    const auto fim = chrono::steady_clock::now();
    duracao = chrono::duration_cast<chrono::nanoseconds>(fim - inicio);
    dist = move(fila.chave);
    return !fila.limite_tempo_atingido;
}

bool processar_grafo(const string& caminho, int vertices_declarados,
                     const opcoes_benchmark& opcoes,
                     const vector<int>& origens, const string& origens_formatadas,
                     ofstream& saida,
                     chrono::steady_clock::time_point prazo_grafo) {
    if (opcoes.limite_vertices > 0 &&
        vertices_declarados > opcoes.limite_vertices) {
        cerr << "Ignorando " << caminho << " (" << vertices_declarados
             << " vertices; limite atual: " << opcoes.limite_vertices << ").\n";
        return true;
    }

    int vertices = 0;
    lista_adj_ponderada adj;
    bool limite_atingido = false;
    cout << "Carregando e preparando " << caminho << "...\n" << flush;
    if (!ler_grafo_lista_ponderada_ate(caminho, vertices, adj,
                                       prazo_grafo, limite_atingido)) {
        if (limite_atingido) {
            saida << "Grafo: " << caminho
                  << " | limite deste grafo atingido durante leitura/construcao;"
                     " nenhuma busca foi executada.\n"
                  << "----------------------------------------\n";
            saida.flush();
            cout << "Limite global atingido durante a leitura de "
                 << caminho << ".\n";
            return true;
        }
        return false;
    }
    if (vertices != vertices_declarados) {
        cerr << "A quantidade de vertices mudou durante a leitura de "
             << caminho << ".\n";
        return false;
    }
    if (grafo_tem_peso_negativo(vertices, adj)) {
        cerr << "Dijkstra nao aceita pesos negativos: " << caminho << ".\n";
        return false;
    }

    const int k = static_cast<int>(origens.size());

    chrono::nanoseconds total_vetor{0}, total_heap{0};
    chrono::nanoseconds total_vetor_completo{0}, total_heap_completo{0};
    int vetor_completas = 0, heap_completas = 0;
    int vetor_interrompidas = 0, heap_interrompidas = 0;
    int vetor_nao_iniciadas = 0, heap_nao_iniciadas = 0;

    for (int indice = 0; indice < k; ++indice) {
        if (chrono::steady_clock::now() >= prazo_grafo) {
            vetor_nao_iniciadas += k - indice;
            heap_nao_iniciadas += k - indice;
            break;
        }

        vector<double> dist_vetor, dist_heap;
        chrono::nanoseconds tempo_vetor{0}, tempo_heap{0};
        const int origem = origens[indice];

        bool vetor_completo = false, heap_completo = false;
        if (opcoes.somente_heap) {
            heap_completo = executar_amostra(
                origem, vertices, adj, FILA_HEAP, dist_heap, tempo_heap,
                prazo_grafo);
        } else {
            const auto agora = chrono::steady_clock::now();
            const auto restantes = prazo_grafo - agora;
            const auto intervalo_por_execucao =
                restantes / (2 * (k - indice));
            const auto prazo_primeiro = min(
                prazo_grafo, agora + intervalo_por_execucao);
            const bool vetor_primeiro = (indice % 2 == 0);
            if (vetor_primeiro) {
                vetor_completo = executar_amostra(
                    origem, vertices, adj, FILA_VETOR, dist_vetor, tempo_vetor,
                    prazo_primeiro);
            } else {
                heap_completo = executar_amostra(
                    origem, vertices, adj, FILA_HEAP, dist_heap, tempo_heap,
                    prazo_primeiro);
            }

            const auto agora_segundo = chrono::steady_clock::now();
            if (agora_segundo < prazo_grafo) {
                const auto prazo_segundo = min(
                    prazo_grafo, agora_segundo + intervalo_por_execucao);
                if (vetor_primeiro) {
                    heap_completo = executar_amostra(
                        origem, vertices, adj, FILA_HEAP, dist_heap, tempo_heap,
                        prazo_segundo);
                } else {
                    vetor_completo = executar_amostra(
                        origem, vertices, adj, FILA_VETOR, dist_vetor,
                        tempo_vetor, prazo_segundo);
                }
            }
        }

        total_vetor += tempo_vetor;
        total_heap += tempo_heap;
        if (!opcoes.somente_heap && tempo_vetor.count() == 0) {
            ++vetor_nao_iniciadas;
        } else if (!opcoes.somente_heap && vetor_completo) {
            ++vetor_completas;
            total_vetor_completo += tempo_vetor;
        } else if (!opcoes.somente_heap) {
            ++vetor_interrompidas;
        }
        if (tempo_heap.count() == 0) {
            ++heap_nao_iniciadas;
        } else if (heap_completo) {
            ++heap_completas;
            total_heap_completo += tempo_heap;
        } else {
            ++heap_interrompidas;
        }

        if (!opcoes.somente_heap && vetor_completo && heap_completo &&
            !distancias_equivalentes(dist_vetor, dist_heap)) {
            cerr << "Resultados diferentes entre vetor e heap para origem "
                 << origem << " em " << caminho << ".\n";
            return false;
        }
    }

    const double media_vetor_ms = vetor_completas > 0
        ? chrono::duration<double, milli>(total_vetor_completo).count() /
              vetor_completas
        : 0.0;
    const double media_heap_ms = heap_completas > 0
        ? chrono::duration<double, milli>(total_heap_completo).count() /
              heap_completas
        : 0.0;
    size_t quantidade_arcos = 0;
    for (int u = 1; u <= vertices; ++u) quantidade_arcos += adj[u].size();
    const size_t arestas = quantidade_arcos / 2;
    const double aceleracao = !opcoes.somente_heap &&
                                      vetor_interrompidas == 0 &&
                                      heap_interrompidas == 0 &&
                                      vetor_nao_iniciadas == 0 &&
                                      heap_nao_iniciadas == 0 &&
                                      media_heap_ms > 0.0
        ? media_vetor_ms / media_heap_ms : 0.0;

    saida << "Grafo: " << caminho << '\n'
          << "Vertices: " << vertices << " | Arestas: " << arestas
          << " | Origens solicitadas: " << k << '\n'
          << "Origens sorteadas (ordem das buscas): "
          << origens_formatadas << '\n';
    if (opcoes.somente_heap) {
        saida << "Vetor: nao executado (--heap-only)\n";
    } else {
        saida << fixed << setprecision(3)
              << "Vetor: " << vetor_completas << " completas, "
              << vetor_interrompidas << " interrompidas, "
              << vetor_nao_iniciadas << " nao iniciadas | ";
        if (vetor_completas > 0) saida << "media das completas = " << media_vetor_ms << " ms";
        else                     saida << "media = NA (nenhuma busca completa)";
        saida << " | tempo total gasto = "
              << chrono::duration<double, milli>(total_vetor).count() << " ms\n";
    }
    saida << fixed << setprecision(3)
          << "Heap: " << heap_completas << " completas, "
          << heap_interrompidas << " interrompidas, "
          << heap_nao_iniciadas << " nao iniciadas | ";
    if (heap_completas > 0) saida << "media das completas = " << media_heap_ms << " ms";
    else                    saida << "media = NA (nenhuma busca completa)";
    saida << " | tempo total gasto = "
          << chrono::duration<double, milli>(total_heap).count() << " ms\n";
    if (opcoes.somente_heap) {
        saida << "Aceleracao: nao aplicavel (--heap-only)\n";
    } else if (vetor_interrompidas == 0 && heap_interrompidas == 0 &&
        vetor_nao_iniciadas == 0 && heap_nao_iniciadas == 0) {
        saida << "Aceleracao do heap: " << setprecision(2) << aceleracao << "x\n";
    } else {
        saida << "Aceleracao: NA (uma ou mais buscas atingiram o limite)\n";
    }
    saida << "----------------------------------------\n";

    cout << caminho << ": V=" << vertices << ", E=" << arestas << ", k=" << k
         << ", origens=[" << origens_formatadas << "]";
    if (!opcoes.somente_heap) {
        cout << ", vetor=" << vetor_completas << "/" << k << " completas";
    }
    cout << ", heap=" << heap_completas << "/" << k << " completas\n";
    saida.flush();
    return true;
}
}

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "--help" || string(argv[i]) == "-h") {
            imprimir_uso(argv[0]);
            return 0;
        }
    }

    opcoes_benchmark opcoes;
    if (!ler_opcoes(argc, argv, opcoes)) return 1;

    vector<int> vertices_grafos(opcoes.grafos.size());
    int menor_grafo = numeric_limits<int>::max();
    for (size_t i = 0; i < opcoes.grafos.size(); ++i) {
        if (!ler_quantidade_vertices(opcoes.grafos[i], vertices_grafos[i])) {
            return 1;
        }
        if (opcoes.limite_vertices == 0 ||
            vertices_grafos[i] <= opcoes.limite_vertices) {
            menor_grafo = min(menor_grafo, vertices_grafos[i]);
        }
    }

    vector<int> origens_comuns;
    if (menor_grafo != numeric_limits<int>::max()) {
        origens_comuns.resize(menor_grafo);
        iota(origens_comuns.begin(), origens_comuns.end(), 1);
        mt19937 gerador(opcoes.semente);
        shuffle(origens_comuns.begin(), origens_comuns.end(), gerador);
        origens_comuns.resize(min(opcoes.k, menor_grafo));
    }
    string origens_formatadas;
    for (size_t i = 0; i < origens_comuns.size(); ++i) {
        if (i > 0) origens_formatadas += ", ";
        origens_formatadas += to_string(origens_comuns[i]);
    }

    ofstream saida(opcoes.arquivo_saida);
    if (!saida.is_open()) {
        cerr << "Erro ao criar o arquivo de resultados: "
             << opcoes.arquivo_saida << ".\n";
        return 1;
    }
    saida << "BENCHMARK DO DIJKSTRA\n"
          << "Semente aleatoria: " << opcoes.semente << '\n'
          << "k solicitado: " << opcoes.k << '\n'
          << "Origens comuns (ordem das buscas): "
          << origens_formatadas << "\n"
          << "Limite: " << opcoes.limite_minutos
          << " minutos por grafo (inclui leitura, preparacao e buscas)\n"
          << "Unidade de tempo: milissegundos (ms)\n\n"
          << "Media considera somente buscas completas. O tempo total inclui"
             " buscas interrompidas.\n"
          << "Aceleracao so e calculada quando todas as buscas dos dois metodos"
             " terminam.\n"
          << "Cada grafo tem seu proprio limite; o tempo nao e compartilhado"
             " entre grafos.\n"
          << "A inicializacao das filas esta dentro da medicao; nao ha busca"
             " extra de aquecimento.\n\n";
    saida.flush();

    cout << "Semente aleatoria: " << opcoes.semente << "\n";
    cout << "Resultados: " << opcoes.arquivo_saida << "\n";
    cout << "Limite por grafo: "
         << opcoes.limite_minutos << " minutos\n";
    for (size_t i = 0; i < opcoes.grafos.size(); ++i) {
        const auto prazo_grafo = chrono::steady_clock::now() +
                                 chrono::minutes(opcoes.limite_minutos);
        if (!processar_grafo(opcoes.grafos[i], vertices_grafos[i], opcoes,
                             origens_comuns, origens_formatadas, saida,
                             prazo_grafo)) {
            return 1;
        }
    }
    return 0;
}
