cmake_minimum_required(VERSION 3.22)

# Desativa explicitamente as ferramentas auxiliares e extras do JUCE antes de declarar o projeto
set(JUCE_BUILD_HELPER_TOOLS OFF CACHE BOOL "" FORCE)
set(JUCE_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(JUCE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

project(Electribe2Hacktribe VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.0 # <--- Atualizar para o JUCE 8 remove completamente a incompatibilidade de compilação do juceaide em macOS recentes
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(Electribe2Hacktribe
    COMPANY_NAME "HacktribeAudio"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT TRUE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    PLUGIN_MANUFACTURER_CODE Hktb
    PLUGIN_CODE E2hk
    FORMATS VST3 AU
    PRODUCT_NAME "Electribe2Hacktribe"
)

juce_generate_juce_header(Electribe2Hacktribe)

target_sources(Electribe2Hacktribe
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
)

target_compile_definitions(Electribe2Hacktribe
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(Electribe2Hacktribe
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
