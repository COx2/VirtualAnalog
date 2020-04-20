#pragma once

#include <JuceHeader.h>

#include "VirtualAnalogVoice.h"

//==============================================================================
class VirtualAnalogAudioProcessor : public gin::GinProcessor,
                                    public MPESynthesiser
{
public:
    //==============================================================================
    VirtualAnalogAudioProcessor();
    ~VirtualAnalogAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    void updateParams (int blockSize);
    void setupModMatrix();

    gin::BandLimitedLookupTables bandLimitedLookupTables;

    //==============================================================================
    void handleMidiEvent (const MidiMessage& m) override;
    void handleController (int ch, int num, int val) override;
    //==============================================================================

    void applyEffects (AudioSampleBuffer& buffer);

    // Voice Params
    struct OSCParams
    {
        OSCParams() = default;

        gin::Parameter::Ptr wave, voices, tune, finetune, level, pulsewidth,
                            detune, spread, pan;

        void setup (VirtualAnalogAudioProcessor& p, int idx);

        JUCE_DECLARE_NON_COPYABLE (OSCParams)
    };

    struct FilterParams
    {
        FilterParams() = default;

        gin::Parameter::Ptr type, slope, keyTracking, velocityTracking,
                            frequency, resonance, amount,
                            attack, decay, sustain, release;

        void setup (VirtualAnalogAudioProcessor& p, int idx);

        JUCE_DECLARE_NON_COPYABLE (FilterParams)
    };

    struct EnvParams
    {
        EnvParams() = default;

        gin::Parameter::Ptr attack, decay, sustain, release;

        void setup (VirtualAnalogAudioProcessor& p, int idx);

        JUCE_DECLARE_NON_COPYABLE (EnvParams)
    };
    
    struct LFOParams
    {
        LFOParams() = default;

        gin::Parameter::Ptr sync, wave, rate, beat, depth, phase, offset, fade, delay;

        void setup (VirtualAnalogAudioProcessor& p, int idx);

        JUCE_DECLARE_NON_COPYABLE (LFOParams)
    };

    struct ADSRParams
    {
        ADSRParams() = default;

        gin::Parameter::Ptr attack, decay, sustain, release, velocityTracking;

        void setup (VirtualAnalogAudioProcessor& p);

        JUCE_DECLARE_NON_COPYABLE (ADSRParams)
    };

    // Global Params
    struct GlobalParams
    {
        GlobalParams() = default;

        gin::Parameter::Ptr mode, legato, level, voices;

        void setup (VirtualAnalogAudioProcessor& p);

        JUCE_DECLARE_NON_COPYABLE (GlobalParams)
    };

    struct ChorusParams
    {
        ChorusParams() = default;

        gin::Parameter::Ptr enable, time, depth, speed, width, mix;

        void setup (VirtualAnalogAudioProcessor& p);

        JUCE_DECLARE_NON_COPYABLE (ChorusParams)
    };

    struct DistortionParams
    {
        DistortionParams() = default;

        gin::Parameter::Ptr enable, amount;

        void setup (VirtualAnalogAudioProcessor& p);

        JUCE_DECLARE_NON_COPYABLE (DistortionParams)
    };

    struct EQParams
    {
        EQParams() = default;

        gin::Parameter::Ptr enable,
                            loFreq, loGain, loQ,
                            mid1Freq, mid1Gain, mid1Q,
                            mid2Freq, mid2Gain, mid2Q,
                            hiFreq, hiGain, hiQ;

        void setup (VirtualAnalogAudioProcessor& p);

        JUCE_DECLARE_NON_COPYABLE (EQParams)
    };

    struct CompressorParams
    {
        CompressorParams() = default;

        gin::Parameter::Ptr enable, attack, release, ratio, threshold, gain;

        void setup (VirtualAnalogAudioProcessor& p);

        JUCE_DECLARE_NON_COPYABLE (CompressorParams)
    };

    struct DelayParams
    {
        DelayParams() = default;

        gin::Parameter::Ptr enable, sync, time, beat, fb, cf, mix, delay;

        void setup (VirtualAnalogAudioProcessor& p);

        JUCE_DECLARE_NON_COPYABLE (DelayParams)
    };

    struct ReverbParams
    {
        ReverbParams() = default;

        gin::Parameter::Ptr enable;

        void setup (VirtualAnalogAudioProcessor& p);

        JUCE_DECLARE_NON_COPYABLE (ReverbParams)
    };

    struct LimiterParams
    {
        LimiterParams() = default;

        gin::Parameter::Ptr enable, attack, release, threshold, gain;

        void setup (VirtualAnalogAudioProcessor& p);

        JUCE_DECLARE_NON_COPYABLE (LimiterParams)
    };

    //==============================================================================
    int modSrcPressure = 0, modSrcTimbre = 0, modScrPitchBend = 0,
        modSrcNote = 0, modSrcVelocity = 0;

    int modSrcCC[119]                                   = {0};
    int modSrcMonoLFO[VirtualAnalogVoice::numLFOs]      = {0};
    int modSrcLFO[VirtualAnalogVoice::numLFOs]          = {0};
    int modSrcFilter[VirtualAnalogVoice::numFilters]    = {0};
    int modSrcEvn[VirtualAnalogVoice::numENVs]          = {0};

    //==============================================================================

    OSCParams oscParams[VirtualAnalogVoice::numOSCs];
    FilterParams filterParams[VirtualAnalogVoice::numFilters];
    EnvParams envParams[VirtualAnalogVoice::numENVs];
    LFOParams lfoParams[VirtualAnalogVoice::numLFOs];

    ADSRParams adsrParams;

    GlobalParams globalParams;
    ChorusParams chorusParams;
    DistortionParams distortionParams;
    EQParams eqParams;
    CompressorParams compressorParams;
    DelayParams delayParams;
    ReverbParams reverbParams;
    LimiterParams limiterParams;

    //==============================================================================
    gin::StereoDelay stereoDelay { 120.1 };
    gin::Dynamics compressor;
    gin::Dynamics limiter;
    gin::EQ eq {4};

    //==============================================================================
    gin::ModMatrix modMatrix;

    gin::LFO modLFOs[VirtualAnalogVoice::numLFOs];

    AudioPlayHead* playhead = nullptr;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VirtualAnalogAudioProcessor)
};
