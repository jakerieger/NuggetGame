// Author: Jake Rieger
// Created: 4/16/2024.
//

#pragma once

#include <RmlUi/Core/Element.h>
#include <RmlUi/Core/EventListener.h>
#include "Engine/AudioContext.h"
#include "Utilities.inl"
#include "Engine/Resources.h"

class IUIListener : public Rml::EventListener {
public:
    void RegisterButton(Rml::Element* element) {
        element->AddEventListener(Rml::EventId::Mousedown, this);
    }

    void UnregisterButton(Rml::Element* element) {
        element->RemoveEventListener("mouse-down", this);
    }

    void ProcessEvent(Rml::Event& event) override {
        if (event.GetId() == Rml::EventId::Mousedown) {
            const auto disabled = event.GetCurrentElement()->GetAttribute("disabled");
            if (disabled) {
                // Audio::PlayOneShot(
                //   Utilities::JoinPath(Resources::GetRoot(), "Assets", "audio", "negative_2.wav")
                //     .string(),
                //   "Negative",
                //   Audio::EAudioTag::UI);
            } else {
                // Audio::PlayOneShot(
                //   Utilities::JoinPath(Resources::GetRoot(), "Assets", "audio", "click_2.wav")
                //     .string(),
                //   "Click",
                //   Audio::EAudioTag::UI);
            }
        }
    }
};