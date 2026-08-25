#include "HyperVoice.h"
#include <cmath>

HyperVoice::HyperVoice(juce::AudioProcessorValueTreeState& state):p(state) {}

bool HyperVoice::canPlaySound(juce::SynthesiserSound* s) {
 return dynamic_cast<HyperSound*>(s)!=nullptr;
}

void HyperVoice::startNote(int note,float vel,juce::SynthesiserSound*,int) {
 freq=juce::MidiMessage::getMidiNoteInHertz(note);
 velocity=vel; a=b=lfo=0;
 envParams.attack=p.getRawParameterValue("attack")->load()*2.0f;
 envParams.decay=p.getRawParameterValue("decay")->load()*2.0f;
 envParams.sustain=p.getRawParameterValue("sustain")->load();
 envParams.release=p.getRawParameterValue("release")->load()*4.0f;
 env.setParameters(envParams);
 env.setSampleRate(getSampleRate());
 env.noteOn();
}

void HyperVoice::stopNote(float,bool) { env.noteOff(); }

void HyperVoice::renderNextBlock(juce::AudioBuffer<float>& out,const juce::MidiBuffer&,int start,int n) {
 auto sr=getSampleRate(); if(sr<=0) return;
 float morph=p.getRawParameterValue("morph")->load();
 float mutation=p.getRawParameterValue("mutation")->load();
 float detune=p.getRawParameterValue("detune")->load();
 float chaos=p.getRawParameterValue("chaos")->load();
 float lfoRate=0.1f+p.getRawParameterValue("lfo_rate")->load()*20.0f;
 float lfoDepth=p.getRawParameterValue("lfo_depth")->load();

 for(int i=0;i<n;++i) {
   float e=env.getNextSample();
   if(!env.isActive()) { clearCurrentNote(); break; }

   float L=std::sin(lfo)*lfoDepth;
   float fm=std::sin(b)*(mutation*12.0f+L*4.0f);
   float sine=std::sin(a+fm);
   float saw=float(std::fmod(a/juce::MathConstants<double>::twoPi,1.0)*2.0-1.0);
   float sq=(std::sin(a+fm)>=0)?1.f:-1.f;
   float sample=sine*(1-morph)+saw*morph*(1-chaos*.4f)+sq*chaos*.4f;
   sample*=e*velocity*.20f;

   for(int ch=0;ch<out.getNumChannels();++ch)
      out.addSample(ch,start+i,sample);

   a+=juce::MathConstants<double>::twoPi*freq*(1+L*.03)/sr;
   b+=juce::MathConstants<double>::twoPi*freq*(1+detune*.12)/sr;
   lfo+=juce::MathConstants<double>::twoPi*lfoRate/sr;
   if(a>=juce::MathConstants<double>::twoPi)a-=juce::MathConstants<double>::twoPi;
   if(b>=juce::MathConstants<double>::twoPi)b-=juce::MathConstants<double>::twoPi;
   if(lfo>=juce::MathConstants<double>::twoPi)lfo-=juce::MathConstants<double>::twoPi;
 }
}
