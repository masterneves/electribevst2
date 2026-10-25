cmake_minimum_required(VERSION 3.22)

project(electribevst2 VERSION 0.1.0)

# Configuração do JUCE (ajusta se necessário a forma como tens o teu JUCE incluído)
find_package(JUCE CONFIG REQUIRED)

juce_add_plugin(electribevst2
    COMPANY_NAME "MasterNeves"
    PLUGIN_NAME "Electribe2Hacktribe"
    IS_SYNTH TRUE               # <--- Define que é um sintetizador (instrumento)
    NEEDS_MIDI_INPUT TRUE       # <--- Permite receber notas MIDI do teclado
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

# Dependências dos módulos do JUCE necessários
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
