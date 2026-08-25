#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
class SalekHypercoreAudioProcessorEditor: public juce::AudioProcessorEditor {
public:
 explicit SalekHypercoreAudioProcessorEditor(SalekHypercoreAudioProcessor&);
 void paint(juce::Graphics&) override; void resized() override;
private:
 SalekHypercoreAudioProcessor& processor;
 juce::OwnedArray<juce::Slider> knobs;
 std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> attachments;
 const char* ids[18]={"morph","mutation","detune","chaos","attack","decay","sustain","release","lfo_rate","lfo_depth","cutoff","resonance","drive","delay_time","delay_feedback","delay_mix","macro1","macro2"};
 const char* names[18]={"MORPH","MUTATION","DETUNE","CHAOS","ATTACK","DECAY","SUSTAIN","RELEASE","LFO RATE","LFO DEPTH","VOID FILTER","RESONANCE","DRIVE","DELAY TIME","FEEDBACK","VOID MIX","ALIEN MACRO","LASER MACRO"};
};
