#pragma once
#include <JuceHeader.h>

class SalekHypercoreAudioProcessor: public juce::AudioProcessor {
public:
 SalekHypercoreAudioProcessor();
 void prepareToPlay(double,int) override;
 void releaseResources() override {}
 bool isBusesLayoutSupported(const BusesLayout&) const override;
 void processBlock(juce::AudioBuffer<float>&,juce::MidiBuffer&) override;
 juce::AudioProcessorEditor* createEditor() override;
 bool hasEditor() const override{return true;}
 const juce::String getName() const override{return "SALEK HYPERCORE";}
 bool acceptsMidi() const override{return true;}
 bool producesMidi() const override{return false;}
 bool isMidiEffect() const override{return false;}
 bool isSynth() const override{return true;}
 double getTailLengthSeconds() const override{return 5.0;}
 int getNumPrograms() override{return 1;}
 int getCurrentProgram() override{return 0;}
 void setCurrentProgram(int) override{}
 const juce::String getProgramName(int) override{return "SALEK INIT";}
 void changeProgramName(int,const juce::String&) override{}
 void getStateInformation(juce::MemoryBlock&) override;
 void setStateInformation(const void*,int) override;

 juce::AudioProcessorValueTreeState parameters;
private:
 juce::Synthesiser synth;
 juce::dsp::StateVariableTPTFilter<float> filter;
 juce::dsp::DelayLine<float,juce::dsp::DelayLineInterpolationTypes::Linear> delayL{96000},delayR{96000};
 juce::dsp::Gain<float> gain;
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SalekHypercoreAudioProcessor)
};
