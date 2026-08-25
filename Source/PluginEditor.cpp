#include "PluginEditor.h"
SalekHypercoreAudioProcessorEditor::SalekHypercoreAudioProcessorEditor(SalekHypercoreAudioProcessor& p):AudioProcessorEditor(&p),processor(p){
 setSize(1320,760);
 for(int i=0;i<18;++i){
   auto* s=knobs.add(new juce::Slider());
   s->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
   s->setTextBoxStyle(juce::Slider::TextBoxBelow,false,78,18);
   s->setRange(0,1,.001); addAndMakeVisible(s);
   attachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,ids[i],*s));
 }
}
void SalekHypercoreAudioProcessorEditor::paint(juce::Graphics& g){
 g.fillAll(juce::Colour(0xff05070d));
 auto neon=juce::Colour(0xff00a8ff), purple=juce::Colour(0xff8a5cff);
 g.setColour(juce::Colour(0xff091321));g.fillRoundedRectangle(20,18,getWidth()-40.f,90,18);
 g.setColour(neon);g.drawRoundedRectangle(20,18,getWidth()-40.f,90,18,1.5f);
 g.setColour(neon);g.setFont(36.f);g.drawText("SALEK // HYPERCORE",45,34,650,40,juce::Justification::left);
 g.setColour(juce::Colour(0xff9db8d5));g.setFont(12.f);g.drawText("HI-TECH SIGNATURE SYNTHESIS • ISATIS / SALEK HIGHTECH",48,76,760,20,juce::Justification::left);

 const char* sections[4]={"MUTATION CORE","LIFE SYSTEM","VOID PROCESSOR","PERFORMANCE DNA"};
 for(int i=0;i<4;++i){
   float x=30+i*322.f;
   g.setColour(juce::Colour(0xff0a111d));g.fillRoundedRectangle(x,135,300,560,16);
   g.setColour(i==3?purple:neon.withAlpha(.7f));g.drawRoundedRectangle(x,135,300,560,16,1.2f);
   g.setColour(neon);g.setFont(15.f);g.drawText(sections[i],x+18,150,260,24,juce::Justification::left);
 }
 for(int i=0;i<18;++i){
   int group=i/4; if(group>3)group=3;
   int pos=i%4;
   float x=50+group*322.f+(pos%2)*135.f;
   float y=195+(pos/2)*210.f;
   if(i>=16){x=1020+(i-16)*135;y=500;}
   g.setColour(juce::Colour(0xff9db8d5));g.setFont(11.f);
   g.drawText(names[i],x,y+145,110,20,juce::Justification::centred);
 }
 g.setColour(juce::Colour(0xff00a8ff).withAlpha(.45f));g.setFont(11.f);
 g.drawText("ALIEN • LASER • METALLIC • GLITCH • VOID • NO KICK / NO BASS",42,715,getWidth()-84,22,juce::Justification::centred);
}
void SalekHypercoreAudioProcessorEditor::resized(){
 for(int i=0;i<18;++i){
   int group=i/4;if(group>3)group=3;int pos=i%4;
   float x=35+group*322+(pos%2)*135;float y=190+(pos/2)*210;
   if(i>=16){x=1015+(i-16)*135;y=495;}
   knobs[i]->setBounds((int)x,(int)y,120,150);
 }
}
