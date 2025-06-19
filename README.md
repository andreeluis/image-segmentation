# Segmentação de Imagens Baseada em Grafos - Trabalho 02

Este trabalho aborda o problema de segmentação de imagens utilizando abordagens baseadas em grafos. Foram implementados dois métodos: um baseado na Árvore Geradora Mínima (MST), conforme o algoritmo proposto por Felzenszwalb e Huttenlocher (2004), e outro baseado em Caminho Mínimo, inspirado na Transformada Floresta de Imagens (IFT) proposta por Falcão et al. (2004). Ambos os métodos foram aplicados a imagens em tons de cinza e coloridas. Uma análise comparativa destaca as semelhanças e diferenças na segmentação de imagens por meio do particionamento de grafo.

## Estrutura do projeto:

```
/image-segmentation
 ├── include/
 │    ├── ift.hpp
 │    └── mst.hpp
 ├── src/
 │    ├── ift.cpp
 │    ├── mst.cpp
 │    └── main.cpp
 ├── images/
 │    └── cat.jpg
 ├── results/ (vai ser gerado)
 ├── CMakeLists.txt
```

## Como compilar (passo a passo):

### 1. Crie uma pasta para a build:

```bash
mkdir build
cd build
```

### 2. Gere os arquivos de build:

```bash
cmake ..
```

> Isso encontra o OpenCV instalado no seu sistema e configura o projeto.

### 3. Compile o projeto:

```bash
cmake --build .
```

Vai gerar o executável chamado `segmenter` na pasta `build`.

## Como rodar:

```bash
./segmenter ift ../images/cat.jpg 100
```

ou

```bash
./segmenter mst ../images/cat.jpg 5000
```

> As imagens segmentadas serão salvas em `../results/ift_segmented.jpg` e `../results/mst_segmented.jpg`.

---

## Alternativa mais simples (sem CMake) usando g++ direto (Linux/macOS/WLS):

```bash
g++ -std=c++17 src/*.cpp -Iinclude `pkg-config --cflags --libs opencv4` -o segmenter
```

> Não garanto que funcione!

### Antigo:

```bash
g++ mst.cpp -I/usr/include/opencv4 -L/usr/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -o main && ./main
```

E executar normalmente:

```bash
./segmenter ift images/cat.jpg 100
```
