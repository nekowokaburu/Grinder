#include "button.hpp"
#include "communicator.hpp"
#include "eepromMemory.hpp"

Button* button = nullptr;
Communicator* communicator = nullptr;
Eeprom* eeprom = nullptr;

auto buffer = new char[6];
unsigned long grinderStopTime = 0;

void setup()
{
    // Serial.begin(57600);
    // while (!Serial)
    //     ;

    // // This helps to see if the device crashes or resets at some point.
    // Serial.println("Started SmartGrinder!");

    button = new Button();
    communicator = new Communicator();
    eeprom = new Eeprom();


#if INITIALIZE_EEPROM
    eeprom_->Save(Eeprom::Parameter::ShortShotTimeMs, SINGLE_DOSE_TIME);
    eeprom_->Save(Eeprom::Parameter::LongShotTimeMs, DOUBLE_DOSE_TIME);
#endif

    if(LOAD_INITIAL_PARAMETERS_FROM_EEPROM)
    {
        eeprom->Load(Eeprom::Parameter::ShortShotTimeMs, SINGLE_DOSE_TIME);
        eeprom->Load(Eeprom::Parameter::LongShotTimeMs, DOUBLE_DOSE_TIME);
    }

    pinMode(SSR_PIN, OUTPUT);
}

void loop()
{
    // It would be better to up the BT/Serial baud rate or collect the
    // serial data in the main loop and trigger the commands when an
    // identifier is detected (e.g. newline). Since we are talking of 
    // a few milliseconds in a grinding process, this is just ignored for now.
    delay(5);

    button->Update();

    communicator->Update();
    
    HandleButton();

    HandleBluetooth();

}

void HandleButton()
{
    const unsigned long currentTime = millis();
    switch (button->RegisteredButtonPress())
    {
        case Button::Command::Click:
            // If already running some program, stop the grinder
            if (grinderStopTime > currentTime)
            {
                digitalWrite(SSR_PIN, LOW);
                grinderStopTime = currentTime;
            }
            else
            {
                // Start program 1
                grinderStopTime = currentTime + SINGLE_DOSE_TIME;
                digitalWrite(SSR_PIN, HIGH);
            }
            break;

        case Button::Command::DoubleClick:
            // Start program 2
            grinderStopTime = currentTime + DOUBLE_DOSE_TIME;
            digitalWrite(SSR_PIN, HIGH);
            break;
        case Button::Command::Hold:
            digitalWrite(SSR_PIN, HIGH);
            break;
        case Button::Command::Release:
            digitalWrite(SSR_PIN, LOW);
            break;

        case Button::Command::Nothing:
            if (grinderStopTime <= currentTime)
                digitalWrite(SSR_PIN, LOW);

            break;

        case Button::Command::Error:
        default:
            break;
    }
}

void HandleBluetooth()
{
 switch (communicator->Command())
 {
    case Communicator::Command::UpdateApp:
    {        
        sprintf(buffer, "%05lu", SINGLE_DOSE_TIME );
        auto combinedTime = String(buffer);
        buffer[0] = '\0';
        sprintf(buffer, "%05lu", DOUBLE_DOSE_TIME );
        combinedTime += String(buffer);
        communicator->SendMessageOnce(combinedTime);
    }
    break;

    case Communicator::Command::ShortShotTime:
    {
        unsigned long shortShotTime = 0;
        communicator->Value(shortShotTime);
        SINGLE_DOSE_TIME = shortShotTime;
        eeprom->Save(Eeprom::Parameter::ShortShotTimeMs, shortShotTime);
    }
    break;

    case Communicator::Command::LongShotTime:
    {
        unsigned long longShotTime = 0;
        communicator->Value(longShotTime);
        DOUBLE_DOSE_TIME = longShotTime;
        eeprom->Save(Eeprom::Parameter::LongShotTimeMs, longShotTime);
    }
    break;
 
 case Communicator::Command::None:
 default:
    break;
 }   
}
