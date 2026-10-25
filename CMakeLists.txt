cmake_minimum_required(VERSION 3.22)

project(electribevst2 VERSION 0.1.0)

# O JUCE 8 exige C++20 ativado obrigatoriamente
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Inclui a pasta do JUCE descarregada pelo GitHub Actions
add_subdirectory(JUCE)

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

# Adição dos ficheiros fonte do projeto
target_sources(electribevst2
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h)

# Definições de compilação
target_compile_definitions(electribevst2
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

# Dependências dos módulos do JUCE essenciais
target_link_libraries(electribevst2
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_audio_basics
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags)

# Adicionar flags de assinatura apenas no macOS onde são suportadas
if(APPLE)
    target_link_libraries(electribevst2
        PUBLIC
            juce::juce_recommended_code_signing_flags)
endif()
