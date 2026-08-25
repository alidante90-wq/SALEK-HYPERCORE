cmake_minimum_required(VERSION 3.21)
project(SALEK-HYPERCORE VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch JUCE from GitHub
include(FetchContent)
FetchContent_Declare(JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 7.0.12
)
FetchContent_MakeAvailable(JUCE)

# Plugin project
juce_add_plugin(SalekHypercore
  COMPANY_NAME "SALEK"
  PLUGIN_MANUFACTURER_CODE "SALK"
  PLUGIN_CODE "HPRC"
  FORMATS VST3
  PRODUCT_NAME "SALEK HYPERCORE"
  DESCRIPTION "Hybrid polyphonic synthesizer with chaos and mutation engines"
  IS_SYNTH TRUE
  VST3_CATEGORIES "Instrument|Synth"
)

juce_generate_juce_header(SalekHypercore)

# Plugin sources
target_sources(SalekHypercore PRIVATE
  Source/PluginProcessor.cpp
  Source/PluginProcessor.h
  Source/PluginEditor.cpp
  Source/PluginEditor.h
  Source/Engine/HyperVoice.cpp
  Source/Engine/HyperVoice.h
  Source/Engine/HyperSound.h
)

target_compile_definitions(SalekHypercore PRIVATE
  JUCE_VST3_CAN_REPLACE_VST2=0
)

# Link JUCE
target_link_libraries(SalekHypercore PRIVATE
  juce::juce_audio_basics
  juce::juce_audio_devices
  juce::juce_audio_formats
  juce::juce_audio_processors
  juce::juce_audio_utils
  juce::juce_core
  juce::juce_data_structures
  juce::juce_dsp
  juce::juce_events
  juce::juce_graphics
  juce::juce_gui_basics
  juce::juce_gui_extra
)
