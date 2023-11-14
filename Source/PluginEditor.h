/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct LookAndFeel : juce::LookAndFeel_V4 
{
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider&) override;
};

struct RotarySliderWithLabels : juce::Slider
{
    RotarySliderWithLabels(juce::RangedAudioParameter& rangedAudioParameter, const juce::String& unitSuffix) : 
        juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
                     juce::Slider::TextEntryBoxPosition::NoTextBox),
        param(&rangedAudioParameter),
        suffix(unitSuffix)
    {
        setLookAndFeel(&lf);
    }
    ~RotarySliderWithLabels() 
    {
        setLookAndFeel(nullptr);
    }

    struct LabelPos 
    {
        float pos;
        juce::String label;
    };

    juce::Array<LabelPos> labels;

    void paint(juce::Graphics& g) override;
    juce::Rectangle<int> getSliderBounds() const;
    int getTextHeight() const { return 14; }
    juce::String getDisplayString() const;
private:
    LookAndFeel lf;
    juce::RangedAudioParameter* param;
    juce::String suffix;
};

struct ResponseCurveComponent : juce::Component, juce::AudioProcessorParameter::Listener, juce::Timer 
{
    ResponseCurveComponent(SimpleeqAudioProcessor&);
    ~ResponseCurveComponent();

    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIdex, bool gestureIsStarting) override;
    void timerCallback() override;
    void paint(juce::Graphics& g) override;
    void updateChain();

private:
    SimpleeqAudioProcessor& audioProcessor;
    juce::Atomic<bool> parametersChanged { false };
    MonoChain monoChain;
};

//==============================================================================
/**
*/
class SimpleeqAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    SimpleeqAudioProcessorEditor (SimpleeqAudioProcessor&);
    ~SimpleeqAudioProcessorEditor() override;

    //==============================================================================
    void resized() override;
    void paint(juce::Graphics&) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleeqAudioProcessor& audioProcessor;

    RotarySliderWithLabels
        peakFreqSlider,
        peakGainSlider,
        peakQualitySlider,
        lowCutFreqSlider,
        highCutFreqSlider,
        lowCutSlopeSlider,
        highCutSlopeSlider;

    ResponseCurveComponent responseCurveComponent;

    using APVTS = juce::AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment
        peakFreqSliderAttachment,
        peakGainSliderAttachment,
        peakQualitySliderAttachment,
        lowCutFreqSliderAttachment,
        highCutFreqSliderAttachment,
        lowCutSlopeSliderAttachment,
        highCutSlopeSliderAttachment;

    std::vector<juce::Component*> getComps();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleeqAudioProcessorEditor)
};
