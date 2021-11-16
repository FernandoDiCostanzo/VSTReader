# ==============================================================================
#
#  This file is part of the JUCE library.
#  Copyright (c) 2020 - Raw Material Software Limited
#
#  JUCE is an open source library subject to commercial or open-source
#  licensing.
#
#  By using JUCE, you agree to the terms of both the JUCE 6 End-User License
#  Agreement and JUCE Privacy Policy (both effective as of the 16th June 2020).
#
#  End User License Agreement: www.juce.com/juce-6-licence
#  Privacy Policy: www.juce.com/juce-privacy-policy
#
#  Or: You may also use this code under the terms of the GPL v3 (see
#  www.gnu.org/licenses).
#
#  JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
#  EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
#  DISCLAIMED.
#
# ==============================================================================

cmake_minimum_required(VERSION 3.12)

set(PRJ_NAME "VSTReader")
project(${PRJ_NAME} VERSION 0.0.1)
add_subdirectory(juce_612)



if(MSVC)
  add_compile_options($<$<CONFIG:Release>:/MT> # Runtime library: Multi-threaded
                      $<$<CONFIG:RelWithDebInfo>:/MT> # Runtime library: Multi-threaded                           
                      $<$<CONFIG:Debug>:/MTd> # Runtime library: Multi-threaded Debug
					   $<$<CONFIG:Debug>:/MP> # Compile with Multiple Core Debug
                      )
endif()



juce_add_gui_app(${PRJ_NAME}
    MICROPHONE_PERMISSION_ENABLED   TRUE
    PLUGINHOST_AU                   TRUE)

juce_generate_juce_header(${PRJ_NAME})

set_property(TARGET ${PRJ_NAME} PROPERTY CXX_STANDARD 17 )


target_sources(${PRJ_NAME} PRIVATE
    Source/HostStartup.cpp
    Source/Plugins/IOConfigurationWindow.cpp
    Source/Plugins/InternalPlugins.cpp
    Source/Plugins/PluginGraph.cpp
    Source/UI/GraphEditorPanel.cpp
    Source/UI/MainHostWindow.cpp)


target_compile_definitions(${PRJ_NAME} PRIVATE
    PIP_JUCE_EXAMPLES_DIRECTORY_STRING="${JUCE_SOURCE_DIR}/examples"
    JUCE_ALSA=1
    JUCE_DIRECTSOUND=1
    JUCE_PLUGINHOST_LADSPA=1
    JUCE_PLUGINHOST_VST3=1
    JUCE_PLUGINHOST_VST=0
    JUCE_USE_CAMERA=0
    JUCE_USE_CDBURNER=0
    JUCE_USE_CDREADER=0
    JUCE_USE_CURL=0
    JUCE_USE_FLAC=0
    JUCE_USE_OGGVORBIS=1
    JUCE_VST3_HOST_CROSS_PLATFORM_UID=1
    JUCE_WASAPI=1
    JUCE_WEB_BROWSER=0)

target_link_libraries(${PRJ_NAME} PRIVATE
    juce::juce_audio_utils
    juce::juce_cryptography
    juce::juce_dsp
    juce::juce_opengl
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

juce_add_bundle_resources_directory(${PRJ_NAME} juce_612/examples/Assets)
