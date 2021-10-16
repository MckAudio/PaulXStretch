cmake_minimum_required(VERSION 3.15)

project(PAUL_X_STRETCH VERSION 0.0.1)

add_subdirectory(JUCE)

juce_add_plugin(PaulStretchPlugin
    VERSION 1.2.5
    PLUGIN_MANUFACTURER_CODE MckA
    PLUGIN_CODE PaXs
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "PaulXStretch")

juce_generate_juce_header(PaulStretchPlugin)


target_sources(PaulStretchPlugin
    PRIVATE
        ./Source/envelope_component.cpp
        ./Source/envelope_component.h
        ./Source/jcdp_envelope.h
        ./Source/PluginEditor.cpp
        ./Source/PluginEditor.h
        ./Source/PluginProcessor.cpp
        ./Source/PluginProcessor.h
        ./Source/ps3_BufferingAudioSource.cpp
        ./Source/ps3_BufferingAudioSource.h
        ./Source/RenderSettingsComponent.cpp
        ./Source/RenderSettingsComponent.h
        ./Source/PS_Source/BinauralBeats.cpp
        ./Source/PS_Source/BinauralBeats.h
        ./Source/PS_Source/fftw3.h
        ./Source/PS_Source/FreeEdit.cpp
        ./Source/PS_Source/FreeEdit.h
        ./Source/PS_Source/globals.cpp
        ./Source/PS_Source/globals.h
        ./Source/PS_Source/PaulStretchControl.cpp
        ./Source/PS_Source/PaulStretchControl.h
        ./Source/PS_Source/Player.cpp
        ./Source/PS_Source/Player.h
        ./Source/PS_Source/ProcessedStretch.cpp
        ./Source/PS_Source/ProcessedStretch.h
        ./Source/PS_Source/Stretch.cpp
        ./Source/PS_Source/Stretch.h
        ./Source/PS_Source/StretchSource.cpp
        ./Source/PS_Source/StretchSource.h
        ./Source/PS_Source/version.h
        ./Source/WDL/denormal.h
        ./Source/WDL/heapbuf.h
        ./Source/WDL/resample.cpp
        ./Source/WDL/resample.h
        ./Source/WDL/wdltypes.h
        ./Source/PS_Source/Input/AInputS.h
        ./Source/PS_Source/Input/InputS.h)

target_compile_definitions(PaulStretchPlugin
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_MODAL_LOOPS_PERMITTED=1)

target_link_libraries(PaulStretchPlugin
    PRIVATE
        juce::juce_audio_utils
        juce::juce_cryptography
        juce::juce_dsp
        fftw3f
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)