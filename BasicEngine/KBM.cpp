#include "KBM.h"

KBM::KBM(const int deviceID)
	:InputDevice(deviceID)
{

}

KBM::~KBM()
{
}

//=============================================================================
// Function: const DeviceType getDeviceType() const
// Description:
// Gets the device type.
// Output:
// const DeviceType
// Returns the keyboard device type.
//=============================================================================
const InputDevice::DeviceType KBM::getDeviceType() const
{
	return INPUTDEVICE_GAMEPAD;
}

//=============================================================================
// Function: const Vector2D& getPosition() const
// Description:
// Gets the current mouse position.
// Output:
// const Vector2D&
// Returns a vector containing the mouse coordinates.
//=============================================================================
const Vector2D& KBM::getPosition() const
{
	return m_position;
}

//=============================================================================
// Function: void setPosition(const Vector2D&)
// Description:
// Sets the new mouse position.
// Parameters:
// const Vector2D& position - The new mouse position.
//=============================================================================
void KBM::setPosition(const Vector2D& position)
{
	m_position = position;
}