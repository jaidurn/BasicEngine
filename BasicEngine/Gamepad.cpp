#include "Gamepad.h"
#include "LogLocator.h"

Gamepad::Gamepad(const int deviceID)
	:InputDevice(-1),
	m_controller(nullptr),
	m_leftTrigger(0.0f),
	m_rightTrigger(0.0f)
{
	if (!open(deviceID))
	{
		std::string error = "Failed to open the controller for joystickID: " + std::to_string(deviceID);

		std::string sdlError = "SDL_Error: ";
		sdlError += SDL_GetError();

		LogLocator::getLog().log(error);
		LogLocator::getLog().log(sdlError);
	}
}

Gamepad::~Gamepad()
{
	close();
}

//=============================================================================
// Function: const DeviceType getDeviceType() const
// Description:
// Gets the device type.
// Output:
// const DeviceType
// Returns the gamepad device type.
//=============================================================================
const InputDevice::DeviceType Gamepad::getDeviceType() const
{
	return INPUTDEVICE_GAMEPAD;
}

//=============================================================================
// Function: const Vector2D& getLeftStick() const
// Description:
// Gets the vector containing the left stick information.
// Output:
// const Vector2D&
// Returns the current left stick vector.
//=============================================================================
const Vector2D& Gamepad::getLeftStick() const
{
	return m_leftStick;
}

//=============================================================================
// Function: const Vector2D& getRightStick() const
// Description:
// Gets the vector containing the right stick information.
// Output:
// const Vector2D&
// Returns the current right stick vector.
//=============================================================================
const Vector2D& Gamepad::getRightStick() const
{
	return m_rightStick;
}

//=============================================================================
// Function: const float getLeftTrigger() const
// Description:
// Gets the current state of the left trigger.
// Output:
// const float
// Returns a value between 0.0f and 1.0f
//=============================================================================
const float Gamepad::getLeftTrigger() const
{
	return m_leftTrigger;
}

//=============================================================================
// Function: const float getRightTrigger() const
// Description:
// Gets the current state of the right trigger.
// Output:
// const float
// Returns a value between 0.0f and 1.0f
//=============================================================================
const float Gamepad::getRightTrigger() const
{
	return m_rightTrigger;
}

//=============================================================================
// Function: void setLeftX(const Sint16)
// Description:
// Sets the left sticks x position.
// Takes an argument in as a short int and converts it to a
// point between -1.0f and 1.0f
// Parameters:
// const Sint16 leftX - The amount to move the X. 
//=============================================================================
void Gamepad::setLeftX(const Sint16 leftX)
{
	if (leftX < 0)
	{
		m_leftStick.m_x = (float)(leftX / m_MAX_AMOUNT);
		m_leftStick.m_x *= -1.0f;
	}
	else
	{
		m_leftStick.m_x = (float)(leftX / -(m_MAX_AMOUNT + 1));
	}
}

//=============================================================================
// Function: void setLeftY(const Sint16)
// Description:
// Sets the left sticks y position.
// Takes an argument in as a short int and converts it to a
// point between -1.0f and 1.0f. 
// Parameters:
// const Sint16 leftY - The amount to move the Y.
//=============================================================================
void Gamepad::setLeftY(const Sint16 leftY)
{
	if (leftY < 0)
	{
		m_leftStick.m_y = (float)(leftY / m_MAX_AMOUNT);
		m_leftStick.m_y *= -1.0f;
	}
	else
	{
		m_leftStick.m_y = (float)(leftY / -(m_MAX_AMOUNT + 1));
	}
}

//=============================================================================
// Function: void setRightX(const Sint16)
// Description:
// Sets the right sticks x position.
// Takes an argument in as a short int and converts it to a
// point between -1.0f and 1.0f
// Parameters:
// const Sint16 rightX - The amount to move the X. 
//=============================================================================
void Gamepad::setRightX(const Sint16 rightX)
{
	if (rightX < 0)
	{
		m_rightStick.m_x = (float)(rightX / m_MAX_AMOUNT);
		m_rightStick.m_x *= -1.0f;
	}
	else
	{
		m_rightStick.m_x = (float)(rightX / -(m_MAX_AMOUNT + 1));
	}
}

//=============================================================================
// Function: void setRightY(const Sint16)
// Description:
// Sets the right sticks y position.
// Takes an argument in as a short int and converts it to a
// point between -1.0f and 1.0f. 
// Parameters:
// const Sint16 rightY - The amount to move the Y.
//=============================================================================
void Gamepad::setRightY(const Sint16 rightY)
{
	if (rightY < 0)
	{
		m_rightStick.m_y = (float)(rightY / m_MAX_AMOUNT);
		m_rightStick.m_y *= -1.0f;
	}
	else
	{
		m_rightStick.m_y = (float)(rightY / -(m_MAX_AMOUNT + 1));
	}
}

//=============================================================================
// Function: void setLeftTrigger(const Sint16)
// Description:
// Sets the left trigger state.
// Converts the short int into a number between 0.0f and 1.0f
// Parameters:
// const Sint16 leftTrigger - The amount to change the trigger by.
//=============================================================================
void Gamepad::setLeftTrigger(const Sint16 leftTrigger)
{
	m_leftTrigger = (float)(leftTrigger / -(m_MAX_AMOUNT + 1));
}

//=============================================================================
// Function: void setRightTrigger(const Sint16)
// Description:
// Sets the right trigger state.
// Converts the short int into a number between 0.0f and 1.0f
// Parameters:
// const Sint16 rightTrigger - The amount to change the trigger by.
//=============================================================================
void Gamepad::setRightTrigger(const Sint16 rightTrigger)
{
	m_rightTrigger = (float)(rightTrigger / -(m_MAX_AMOUNT + 1));
}

//=============================================================================
// Function: bool open(const int)
// Description:
// Tries to open up the controller for the specified joystick device.
// Parameters:
// const int joystickID - The joystick to use.
// Output:
// bool
// Returns true if the gamepad was opened.
// Returns false if the gamepad failed to open.
//=============================================================================
bool Gamepad::open(const int joystickID)
{
	bool opened = false;

	m_controller = SDL_GameControllerOpen(joystickID);

	if (m_controller)
	{
		m_deviceID = SDL_JoystickGetDeviceInstanceID(joystickID);
		opened = true;
	}

	return opened;
}

//=============================================================================
// Function: void close()
// Description:
// Closes the controller if it's open.
//=============================================================================
void Gamepad::close()
{
	if (m_controller)
	{
		SDL_GameControllerClose(m_controller);
		m_deviceID = -1;
	}
}