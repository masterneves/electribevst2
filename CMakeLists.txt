cmake_minimum_required(VERSION 3.22)

# 1. Definir o projeto e a versão primeiro
project(electribevst2 VERSION 0.1.0)

# 2. Exigir C++20 para o JUCE 8
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. Adicionar o diretório do JUCE descarregado pelo GitHub Actions
add_subdirectory(JUCE)

# 4. Criar o plugin VST3/AU
juce_add_plugin(electribevst2
    COMPANY_NAME "MasterNeves"
    PLUGIN_NAME "Electribe2Hacktribe"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    PLUGIN_MANUFACTURE_CODE MstN
    PLUGIN_CODE E2ht
    FORMATS VST3 AU
    PRODUCT_NAME "Electribe 2 Hacktribe")

# 5. Adicionar os ficheiros fonte
target_sources(electribevst2
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h)

# 6. Definir flags de compilação
target_compile_definitions(electribevst2
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

# 7. Ligar as bibliotecas e módulos do JUCE
target_link_libraries(electribevst2
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_audio_basics
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_code_signing_flags)
