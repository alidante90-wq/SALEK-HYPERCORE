#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Engine/HyperVoice.h"
#include "Engine/HyperSound.h"

static juce::AudioProcessorValueTreeState::ParameterLayout layout() {
 std::vector<std::unique_ptr<juce::RangedAudioParameter>> v;
 auto add=[&](const char* id,const char* name,float d){
   v.push_back(std::make_unique<juce::AudioParameterFloat>(
    id,name,juce::NormalisableRange<float>(0.f,1.f,.0001f),d));};
 add("morph","MORPH",.32f); add("mutation","MUTATION",.48f);
 add("detune","DETUNE",.12f); add("chaos","CHAOS",.20f);
 add("attack","ATTACK",.01f); add("decay","DECAY",.20f);
 add("sustain","SUSTAIN",.80f); add("release","RELEASE",.20f);
 add("lfo_rate","LFO RATE",.15f); add("lfo_depth","LFO DEPTH",.25f);
 add("cutoff","VOID FILTER",.72f); add("resonance","RESONANCE",.15f);
 add("drive","CYBER DRIVE",.25f); add("delay_time","VOID DELAY TIME",.25f);
 add("delay_feedback","VOID FEEDBACK",.30f); add("delay_mix","VOID MIX",.15f);
 add("macro1","MACRO ALIEN",.50f); add("macro2","MACRO LASER",.30f);
 add("output","OUTPUT",.75f);
 return {v.begin(),v.end()};
}

SalekHypercoreAudioProcessor::SalekHypercoreAudioProcessor():
 AudioProcessor(BusesProperties().withOutput("Output",juce::AudioChannelSet::stereo(),true)),
 parameters(*this,nullptr,"HYPERCORE",layout()) {
 for(int i=0;i<16;++i)synth.addVoice(new HyperVoice(parameters));
 synth.addSound(new HyperSound());
}

void SalekHypercoreAudioProcessor::prepareToPlay(double sr,int bs) {
 synth.setCurrentPlaybackSampleRate(sr);
 juce::dsp::ProcessSpec s{sr,(juce::uint32)bs,2};
 filter.prepare(s); filter.reset();
 filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
 delayL.prepare(s); delayR.prepare(s); delayL.reset(); delayR.reset();
 gain.prepare(s); gain.setRampDurationSeconds(.02);
}

bool SalekHypercoreAudioProcessor::isBusesLayoutSupported(const BusesLayout& l)const {
 auto o=l.getMainOutputChannelSet();
 return o==juce::AudioChannelSet::mono()||o==juce::AudioChannelSet::stereo();
}

void SalekHypercoreAudioProcessor::processBlock(juce::AudioBuffer<float>& b,juce::MidiBuffer& m) {
 juce::ScopedNoDenormals n; b.clear();
 synth.renderNextBlock(b,m,0,b.getNumSamples());

 float macroAlien=parameters.getRawParameterValue("macro1")->load();
 float cutoff=120.f+(parameters.getRawParameterValue("cutoff")->load()+macroAlien*.15f)*18000.f;
 filter.setCutoffFrequency(juce::jlimit(80.f,19500.f,cutoff));
 filter.setResonance(parameters.getRawParameterValue("resonance")->load()*.98f);

 auto block=juce::dsp::AudioBlock<float>(b);
 auto ctx=juce::dsp::ProcessContextReplacing<float>(block);
 filter.process(ctx);

 float drive=parameters.getRawParameterValue("drive")->load();
 float laser=parameters.getRawParameterValue("macro2")->load();
 float dt=.01f+parameters.getRawParameterValue("delay_time")->load()*1.2f;
 float fb=parameters.getRawParameterValue("delay_feedback")->load()*.92f;
 float mix=parameters.getRawParameterValue("delay_mix")->load();
 delayL.setDelay(dt*getSampleRate()); delayR.setDelay(dt*getSampleRate()*1.17f);

 for(int i=0;i<b.getNumSamples();++i){
   float L=b.getSample(0,i), R=b.getNumChannels()>1?b.getSample(1,i):L;
   float dl=delayL.popSample(0),dr=delayR.popSample(1);
   delayL.pushSample(0,L+dl*fb); delayR.pushSample(1,R+dr*fb);
   L=std::tanh(L*(1+drive*12+laser*3));
   R=std::tanh(R*(1+drive*12+laser*3));
   b.setSample(0,i,L*(1-mix)+dl*mix);
   if(b.getNumChannels()>1)b.setSample(1,i,R*(1-mix)+dr*mix);
 }
 gain.setGainLinear(parameters.getRawParameterValue("output")->load());
 gain.process(ctx);
}

void SalekHypercoreAudioProcessor::getStateInformation(juce::MemoryBlock& d){
 auto s=parameters.copyState(); if(auto x=s.createXml())copyXmlToBinary(*x,d);
}
void SalekHypercoreAudioProcessor::setStateInformation(const void* d,int n){
 if(auto x=getXmlFromBinary(d,n))parameters.replaceState(juce::ValueTree::fromXml(*x));
}
juce::AudioProcessorEditor* SalekHypercoreAudioProcessor::createEditor(){
 return new SalekHypercoreAudioProcessorEditor(*this);
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new SalekHypercoreAudioProcessor();}
