#ifndef __COMMUNICATOR_HPP
#define __COMMUNICATOR_HPP

#include "eepromMemory.hpp"
#include "settings.hpp"

// Opens a serial connection and sends/retrieves data
class Communicator
{
  public:
    // Communication commands
    enum class Command
    {
        None = 0,
        UpdateApp,       // send all interesting parameters to the connected application
        ShortShotTime,
        LongShotTime        
    };

    Communicator();

    // Gets the last received command
    Command Command() noexcept;

    // Gets the last received value
    template <class T>
    void Value(T& Value) const noexcept;

    // Update loop checks for a new message and retrieves it completely in one go.
    void Update() noexcept;

    // Send a message once, not in a loop
    void SendMessageOnce(String Message) const noexcept;

    // Sends a message once, formatted as command
    // void SendMessageOnce(String Message, double Value) const noexcept;

  private:
    String receivedMessage_;
    enum Command receivedCommand_;
};

template <class T>
void Communicator::Value(T& Value) const noexcept
{
    const auto numberStartsAfter = receivedMessage_.indexOf(String(":"));
    Value = static_cast<T>(strtoul(receivedMessage_.substring(numberStartsAfter + 1).c_str(), nullptr, 10));
    LOG_COMM(String("returning value from message: ") + receivedMessage_ + "; extracted value: " + Value)
}

#endif
