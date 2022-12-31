#ifndef __BUTTON_HPP
#define __BUTTON_HPP

#include "settings.hpp"

// A software debounced button that registers click and long clicks.
class Button
{
public:
    enum class Command
    {
        Error = -1,  // something unexpected happened
        Nothing,     // nothing was pressed or released
        Click,       // pressed once, short
        DoubleClick, // pressed twice in short succession
        Hold,        // pressed and held
        Release      // button released
    };

    // Do not move constructor to cpp!
    Button()
        : registeredButtonCommand_{Command::Nothing}, alreadyTriggered_{false}, timeButtonPressed_{0}, timeButtonReleased_{0}, buttonCommandWasProcessed_{true}, startupDelay_{1000}, clickCount_{0}

    {
        pinMode(BUTTON_PIN, INPUT_PULLUP);
    }

    void Update() noexcept;

    Command RegisteredButtonPress() noexcept;

private:
    Command registeredButtonCommand_;

    bool buttonWasPressed_ = false;

    unsigned long timeButtonPressed_;
    unsigned long timeButtonReleased_;
    const unsigned long startupDelay_;

    bool buttonCommandWasProcessed_;

    bool alreadyTriggered_;

    uint8_t clickCount_;
};

#endif
