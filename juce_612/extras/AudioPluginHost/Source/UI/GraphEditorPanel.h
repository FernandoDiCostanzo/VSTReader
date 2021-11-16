/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2020 - Raw Material Software Limited

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 6 End-User License
   Agreement and JUCE Privacy Policy (both effective as of the 16th June 2020).

   End User License Agreement: www.juce.com/juce-6-licence
   Privacy Policy: www.juce.com/juce-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#pragma once

#include "../Plugins/PluginGraph.h"

class MainHostWindow;

//==============================================================================
/**
    A panel that displays and edits a PluginGraph.
*/
class GraphEditorPanel   : public Component,
                           public ChangeListener,
                           private Timer
{
public:
    GraphEditorPanel (PluginGraph& graph);
    ~GraphEditorPanel() override;

    void createNewPlugin (const PluginDescription&, Point<int> position);

    void paint (Graphics&) override;
    void resized() override;

    void mouseDown (const MouseEvent&) override;
    void mouseUp   (const MouseEvent&) override;
    void mouseDrag (const MouseEvent&) override;

    void changeListenerCallback (ChangeBroadcaster*) override;

    //==============================================================================
    void updateComponents();

    //==============================================================================
    void showPopupMenu (Point<int> position);

    //==============================================================================
    void beginConnectorDrag (AudioProcessorGraph::NodeAndChannel source,
                             AudioProcessorGraph::NodeAndChannel dest,
                             const MouseEvent&);
    void dragConnector (const MouseEvent&);
    void endDraggingConnector (const MouseEvent&);

    //==============================================================================
    PluginGraph& graph;

private:
    struct PluginComponent;
    struct ConnectorComponent;
    struct PinComponent;

    OwnedArray<PluginComponent> nodes;
    OwnedArray<ConnectorComponent> connectors;
    std::unique_ptr<ConnectorComponent> draggingConnector;
    std::unique_ptr<PopupMenu> menu;

    PluginComponent* getComponentForPlugin (AudioProcessorGraph::NodeID) const;
    ConnectorComponent* getComponentForConnection (const AudioProcessorGraph::Connection&) const;
    PinComponent* findPinAt (Point<float>) const;

    //==============================================================================
    Point<int> originalTouchPos;

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphEditorPanel)
};


class PluginAutParamsCompWrapper : public Component {
public:

    PluginAutParamsCompWrapper(){}

    inline void setProcessor(AudioProcessor* p) {
        if (p->getNumParameters() != 0) {
            PluginAutParamsComp* pap = new PluginAutParamsComp(p);
            Viewport* vp = new Viewport();
            vp->setScrollBarsShown(false, true);
            vp->setViewedComponent(pap, false);
            addAndMakeVisible(papc.add(pap));
            addAndMakeVisible(viewPorts.add(vp));
            resized();
        }
    }

    void paint(Graphics& g) override {
        g.fillAll(Colour(0xff1e1e1e));
    };

    void resized() override {
        for (int i = 0; i <papc.size(); i++) {
            int allParamsSize = papc[i]->paramNodes.size() * 100;
            papc[i]->setBounds(allParamsSize *i, 0, allParamsSize, getHeight());
            viewPorts[i]->setBounds(0,0,500,getHeight());
            
        }
    }


private:

    class CustomExtentionVisitors : public ExtensionsVisitor{
    public :

        

        void visitUnknown(const Unknown& u) override {
            int a = 1;
        }

        /** Called with VST3-specific information. */
        virtual void visitVST3Client(const VST3Client& vst3) override {
            
            auto x = vst3.getIComponentPtr();
            
            int a = 1;
        }

        /** Called with VST-specific information. */
        virtual void visitVSTClient(const VSTClient& vstClient)override {
            int a = 1;
        }
    };

    class PluginAutParamsComp : public Component , private AudioProcessorParameter::Listener  {
    public:


        PluginAutParamsComp(AudioProcessor* p) {
            
            
            processor = dynamic_cast<AudioPluginInstance*>(p);
            processor->getExtensions(cev);

            processorName.setText(p->getName(),dontSendNotification);
            addAndMakeVisible(processorName);
            auto& paramList = processor->getParameters();
           
            for (int i = 0; i < p->getNumParameters(); i++) {
                AudioProcessorParameter* param = paramList[i];
                if (param->isAutomatable()) {

                    param->addListener(this);

                    ParamNode* pm = new ParamNode();
                    pm->rowParamIndex = param->getParameterIndex();
                    


                   
                    Slider* s = new Slider(Slider::SliderStyle::Rotary, Slider::TextBoxAbove); 
                    s->setRange(0, 1, 0.001);
                    s->setValue(param->getValue(), dontSendNotification);
                    addAndMakeVisible(s);
                    pm->paramSlider = s;

                    Label* l = new Label("", param->getName(1000));
                    addAndMakeVisible(l);
                    pm->paramLabel = l;

                    paramNodes.add(pm);
                    
                    //paramsSliderAttachement.add(new AudioProcessorValueTreeState::SliderAttachment(p , p.getInstanceParamID(i), *paramsSlider.getUnchecked(i)));
                }
            }

            for (int i = 0; i < paramNodes.size(); i++) {
                auto* slider = paramNodes[i]->paramSlider;
                int rowIndex = paramNodes[i]->rowParamIndex;
                slider->onValueChange = [this,i,slider,rowIndex] {
                    processor->setParameter(rowIndex, slider->getValue());
                };
            }

        }

        ~PluginAutParamsComp(){
            for (int i = 0; i < processor->getNumParameters(); i++) {
                AudioProcessorParameter* param = processor->getParameters()[i];
                if (param->isAutomatable()) {
                    param->removeListener(this);
                }
            }
        }

        void parameterValueChanged(int parameterIndex, float newValue) override {
            for (auto* pn : paramNodes) {
                if (pn->rowParamIndex == parameterIndex)
                    pn->paramSlider->setValue(newValue, dontSendNotification);
            }
        }

        void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override{}


        void paint(Graphics& g) override {

            g.setColour(Colour(0xff323232));
            g.fillRoundedRectangle(getLocalBounds().toFloat().reduced(2), 5);
        };

        void resized() override {
            auto area = getLocalBounds();
            auto processorNameArea = area.removeFromTop(20);
            processorName.setBounds(processorNameArea);
            for (int i = 0; i < paramNodes.size(); i++) {
                auto* pn = paramNodes[i];
                pn->paramSlider->setBounds(100 * i, area.getY(), 100, area.getHeight()/2);
                pn->paramLabel->setBounds(100 * i, area.getHeight()/2+5, 100, area.getHeight());
            }
        }

        struct ParamNode {
            Slider* paramSlider;
            Label* paramLabel;
            int rowParamIndex;
        };

        OwnedArray<ParamNode> paramNodes;

        Label processorName;
        AudioPluginInstance * processor;

        CustomExtentionVisitors cev;
    };

    OwnedArray<PluginAutParamsComp> papc;
    OwnedArray<Viewport> viewPorts;
};




//==============================================================================
/**
    A panel that embeds a GraphEditorPanel with a midi keyboard at the bottom.

    It also manages the graph itself, and plays it.
*/
class GraphDocumentComponent  : public Component,
                                public DragAndDropTarget,
                                public DragAndDropContainer,
                                private ChangeListener
{
public:
    GraphDocumentComponent (AudioPluginFormatManager& formatManager,
                            AudioDeviceManager& deviceManager,
                            KnownPluginList& pluginList);

    ~GraphDocumentComponent() override;

    //==============================================================================
    void createNewPlugin (const PluginDescription&, Point<int> position);
    void setDoublePrecision (bool doublePrecision);
    bool closeAnyOpenPluginWindows();

    //==============================================================================
    std::unique_ptr<PluginGraph> graph;

    void resized() override;
    void unfocusKeyboardComponent();
    void releaseGraph();

    //==============================================================================
    bool isInterestedInDragSource (const SourceDetails&) override;
    void itemDropped (const SourceDetails&) override;

    //==============================================================================
    std::unique_ptr<GraphEditorPanel> graphPanel;
    std::unique_ptr<MidiKeyboardComponent> keyboardComp;
    std::unique_ptr<PluginAutParamsCompWrapper> papcw;

    //==============================================================================
    void showSidePanel (bool isSettingsPanel);
    void hideLastSidePanel();

    BurgerMenuComponent burgerMenu;

private:
    //==============================================================================
    AudioDeviceManager& deviceManager;
    KnownPluginList& pluginList;

    AudioProcessorPlayer graphPlayer;
    MidiKeyboardState keyState;
    MidiOutput* midiOutput = nullptr;

    struct TooltipBar;
    std::unique_ptr<TooltipBar> statusBar;

    class TitleBarComponent;
    std::unique_ptr<TitleBarComponent> titleBarComponent;

    //==============================================================================
    struct PluginListBoxModel;
    std::unique_ptr<PluginListBoxModel> pluginListBoxModel;

    ListBox pluginListBox;

    SidePanel mobileSettingsSidePanel { "Settings", 300, true };
    SidePanel pluginListSidePanel    { "Plugins", 250, false };
    SidePanel* lastOpenedSidePanel = nullptr;

    //==============================================================================
    void changeListenerCallback (ChangeBroadcaster*) override;

    void init();
    void checkAvailableWidth();
    void updateMidiOutput();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GraphDocumentComponent)
};
