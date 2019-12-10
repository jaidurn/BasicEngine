#include "InputDevice.h"
#include "LogLocator.h"

InputDevice::InputDevice(const int deviceID)
	:m_deviceID(deviceID)
{

}

InputDevice::~InputDevice()
{
}

//=============================================================================
// Function: void pressButton(const Uint32)
// Description:
// Gets the button code related to the Uint32 code and if it's found
// sets the button to pressed.
// Parameters:
// const Uint32 button - The SDL button code to press.
//=============================================================================
void InputDevice::pressButton(const Uint32 button)
{
	ButtonCode code = getButtonCode(button);

	if (code != BTN_ERROR)
	{
		m_buttonStates[code] = true;
	}
}

//=============================================================================
// Function: void releaseButton(const Uint32)
// Description:
// Gets the button code related to the Uint32 code and if it's found
// sets the button to released.
// Parameters:
// const Uint32 button - The SDL button code to release.
//=============================================================================
void InputDevice::releaseButton(const Uint32 button)
{
	ButtonCode code = getButtonCode(button);

	if (code != BTN_ERROR)
	{
		m_buttonStates[code] = false;
	}
}

//=============================================================================
// Function: bool buttonPressed(const ButtonCode) const
// Description:
// Gets the pressed state of the provided buttonCode.
// Parameters:
// const ButtonCode buttonCode - The button code to check.
// Output:
// bool
// Returns true if the button was found and it is pressed.
// Returns false if the button wasn't pressed.
//=============================================================================
bool InputDevice::buttonPressed(const ButtonCode buttonCode) const
{
	if (buttonCode != BTN_ERROR)
	{
		auto mit = m_buttonStates.find(buttonCode);

		if (mit != m_buttonStates.end())
		{
			return mit->second;
		}
	}

	return false;
}

//=============================================================================
// Function: void mapButton(const Uint32, const ButtonCode)
// Description:
// Maps the SDL button code to the specified button code of mine.
// If it already exists, it overwrites the current mapping.
// Parameters:
// const Uint32 button - The SDL button code to map.
// const ButtonCode buttonCode - The button code to map it to.
// Output:
//=============================================================================
void InputDevice::mapButton(const Uint32 button,
	const ButtonCode buttonCode)
{
	auto mit = m_buttonMap.find(button);

	if (mit != m_buttonMap.end())
	{
		mit->second = buttonCode;
	}
	else
	{
		m_buttonMap.insert(std::make_pair(button, buttonCode));
	}
}

//=============================================================================
// Function: bool unmapButton(const Uint32)
// Description:
// Unbinds the mapping for the specified SDL button code.
// Parameters:
// const Uint32 button - The SDL button code to unmap.
// Output:
// bool
// Returns true if the button was unmapped.
// Returns false if the button couldn't be unmapped.
//=============================================================================
bool InputDevice::unmapButton(const Uint32 button)
{
	auto mit = m_buttonMap.find(button);

	if (mit != m_buttonMap.end())
	{
		m_buttonMap.erase(mit);
		return true;
	}

	return false;
}

//=============================================================================
// Function: const ButtonCode getButtonCode(const Uint32) const
// Description:
// Gets the current button code bound to the specified SDL button code.
// Parameters:
// const Uint32 button - The SDL button code to get the binding of.
// Output:
// const ButtonCode
// On success - Returns the found button code.
// On failure - Returns button code BTN_ERROR.
//=============================================================================
const ButtonCode InputDevice::getButtonCode(const Uint32 button) const
{
	auto mit = m_buttonMap.find(button);

	if (mit != m_buttonMap.end())
	{
		return mit->second;
	}

	return BTN_ERROR;
}

//=============================================================================
// Function: const int getDeviceID() const
// Description:
// Gets the device ID.
// Output:
// const int
// Returns the current device ID
//=============================================================================
const int InputDevice::getDeviceID() const
{
	return m_deviceID;
}