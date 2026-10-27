# Electribe 2 (Hacktribe OS Engine) VST3 / AU Plugin


Um emulador/sintetizador VST3 e AU de 16 pistas inspirado na **Korg Electribe 2** rodando o firmware personalizado **Hacktribe OS**.

## 🚀 Funcionalidades
- **Arquitetura Dupla Hacktribe:** Cada uma das 16 pistas pode funcionar alternadamente como **Synth Engine** (síntese e modelação analógica) ou **User Sampler** (amostras WAV do utilizador).
- **Filtros Clássicos:** Emulação dos algoritmos Electribe, KingKORG, MS-20 e Prophet.
- **Sequenciador de 64 Passos:** Suporte para Motion Sequence por passo.
- **Master FX XY Pad:** Controlo de efeitos globais em tempo real estilo Kaoss Pad.

## 🛠️ Como Compilar Localmente

### Requisitos
- CMake 3.22+
- Compilador C++20 (MSVC no Windows, Clang no macOS, GCC no Linux)
- Git

### Instruções
```bash
# 1. Clona o repositório
git clone [https://github.com/teu-usuario/electribe2-hacktribe-vst.git](https://github.com/teu-usuario/electribe2-hacktribe-vst.git)
cd electribe2-hacktribe-vst

# 2. Configura com o CMake
cmake -B build

# 3. Compila o projeto
cmake --build build --config Release
