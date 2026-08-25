#pragma once
#include <JuceHeader.h>
#include "HyperSound.h"

class HyperVoice : public juce::SynthesiserVoice {
public:
 explicit HyperVoice(juce::AudioProcessorValueTreeState&);
 bool canPlaySound(juce::SynthesiserSound*) override;
 void startNote(int,float,juce::SynthesiserSound*,int) override;
 void stopNote(float,bool) override;
 void pitchWheelMoved(int) override {}
 void controllerMoved(int,int) override {}
 void renderNextBlock(juce::AudioBuffer<float>&,const juce::MidiBuffer&,int,int) override;
private:
 juce::AudioProcessorValueTreeState& p;
 double a=0,b=0,lfo=0,freq=440;
 float velocity=0;
 juce::ADSR env;
 juce::ADSR::Parameters envParams;
};
