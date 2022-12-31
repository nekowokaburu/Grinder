#include "button.hpp"

Button::Command Button::RegisteredButtonPress() noexcept
{
    if (!buttonCommandWasProcessed_)
    {
        buttonCommandWasProcessed_ = true;
        LOG_BUTTON(String("Returning button state: ") + static_cast<int>(registeredButtonCommand_))
        return registeredButtonCommand_;
    }
    else
        return Command::Nothing;
}

void Button::Update() noexcept
{
    auto lastCommand = registeredButtonCommand_;
    registeredButtonCommand_ = Command::Nothing;
    const unsigned long currentTime = millis();

    const int buttonIsPressed = !digitalRead(BUTTON_PIN);
    const auto timeSinceBtnPress = (currentTime - timeButtonPressed_);

    // Button interaction done, reset
    if (!buttonIsPressed && timeSinceBtnPress > BUTTON_PRESS_RECOVERY * clickCount_)
    {
        clickCount_ = 0;
        if (lastCommand == Command::Hold)
        {
            LOG_BUTTON("Button released");
            registeredButtonCommand_ = Command::Release;
            buttonCommandWasProcessed_ = false;
            return;
        }
    }

    // Button pressed for the first time
    if (buttonIsPressed && buttonIsPressed != buttonWasPressed_)
    {
        LOG_BUTTON("button pressed")
        timeButtonPressed_ = currentTime;
        // Start out holding the button so the grinder turns on right away
        registeredButtonCommand_ = Command::Hold;
        buttonCommandWasProcessed_ = false;
    }


    // button released after press
    if (!buttonIsPressed && buttonIsPressed != buttonWasPressed_)
    {
        alreadyTriggered_ = false;

        LOG_BUTTON(String("Button held ") + timeSinceBtnPress + " ms");

        if (timeSinceBtnPress < BUTTON_PRESS_RECOVERY)
        {
            buttonCommandWasProcessed_ = false;
            clickCount_++;

            LOG_BUTTON(String("Button clicked ") + clickCount_ + " times");

            if (clickCount_ == 1)
                registeredButtonCommand_ = Command::Click;
            else if (clickCount_ == 2)
                registeredButtonCommand_ = Command::DoubleClick;
        }
    }
    else if(buttonIsPressed && buttonIsPressed == buttonWasPressed_)
    // if (buttonIsPressed && timeSinceBtnPress > BUTTON_PRESS_RECOVERY &&
    //     static_cast<int>(registeredButtonCommand_) != static_cast<int>(Command::Hold))
     {
         registeredButtonCommand_ = Command::Hold;
         buttonCommandWasProcessed_ = false;
     }


    buttonWasPressed_ = buttonIsPressed;
}
