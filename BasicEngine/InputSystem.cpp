#include "InputSystem.h"
#include "LogLocator.h"
#include "KBM.h"
#include "Gamepad.h"

InputSystem::InputSystem()
{
	KBM *kbm = new KBM(m_KBM_ID);

	addInputDevice(m_KBM_ID, kbm);
}

InputSystem::~InputSystem()
{
	cleanup();
}

//=============================================================================
// Function: void update(SDL_Event&)
// Description:
// TODO: Make it so the event is pulled inside the input system.
// Updates the input items based on the SDL_Event that is passed through.
// Parameters:
// SDL_Event& e - The event to process.
//=============================================================================
void InputSystem::update(SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_CONTROLLERDEVICEADDED:
	{
		Gamepad* gamepad = new Gamepad(e.cdevice.which);

		if (-1 < gamepad->getDeviceID())
		{
			// This makes sure the device gets added with its 
			// instance ID instead of the joystick id.
			addInputDevice(gamepad->getDeviceID(), gamepad);
		}
		else
		{
			delete gamepad;
			gamepad = nullptr;
		}

		break;
	}
	case SDL_CONTROLLERDEVICEREMOVED:
	{
		removeInputDevice(e.cdevice.which);
		break;
	}
	case SDL_CONTROLLERBUTTONDOWN:
	{
		InputDevice* device = getDevice(e.cbutton.which);

		if (device)
		{
			device->pressButton(e.cbutton.button);
		}

		break;
	}
	case SDL_CONTROLLERBUTTONUP:
	{
		InputDevice* device = getDevice(e.cbutton.which);

		if (device)
		{
			device->releaseButton(e.cbutton.button);
		}

		break;
	}
	case SDL_CONTROLLERAXISMOTION:
	{
		InputDevice* device = getDevice(e.cbutton.which);

		if (device->getDeviceType() == InputDevice::INPUTDEVICE_GAMEPAD)
		{
			Gamepad* gamepad = dynamic_cast<Gamepad*>(device);

			if (gamepad)
			{
				switch (e.caxis.axis)
				{
				case SDL_CONTROLLER_AXIS_LEFTX:
				{
					gamepad->setLeftX(e.caxis.value);
					break;
				}
				case SDL_CONTROLLER_AXIS_LEFTY:
				{
					gamepad->setLeftY(e.caxis.value);
					break;
				}
				case SDL_CONTROLLER_AXIS_RIGHTX:
				{
					gamepad->setRightX(e.caxis.value);
					break;
				}
				case SDL_CONTROLLER_AXIS_RIGHTY:
				{
					gamepad->setRightY(e.caxis.value);
					break;
				}
				case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
				{
					gamepad->setLeftTrigger(e.caxis.value);
					break;
				}
				case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
				{
					gamepad->setRightTrigger(e.caxis.value);
					break;
				}
				}
			}
		}

		break;
	}
	case SDL_KEYDOWN:
	{
		InputDevice* device = getDevice(m_KBM_ID);

		if (device)
		{
			device->pressButton(e.key.keysym.sym);
		}
		break;
	}
	case SDL_KEYUP:
	{
		InputDevice* device = getDevice(m_KBM_ID);

		if (device)
		{
			device->releaseButton(e.key.keysym.sym);
		}

		break;
	}
	case SDL_MOUSEMOTION:
	{
		InputDevice* device = getDevice(m_KBM_ID);

		if (device)
		{
			KBM *kbm = dynamic_cast<KBM*>(device);

			if (kbm)
			{
				kbm->setPosition(Vector2D((float)e.motion.x, (float)e.motion.y));
			}
		}

		break;
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		InputDevice* device = getDevice(m_KBM_ID);

		if (device)
		{
			device->pressButton(e.button.button);
		}

		break;
	}
	case SDL_MOUSEBUTTONUP:
	{
		InputDevice* device = getDevice(m_KBM_ID);

		if (device)
		{
			device->releaseButton(e.button.button);
		}

		break;
	}
	}
}

//=============================================================================
// Function: InputDevice* getDevice(const int) const
// Description:
// Gets the device with the specified ID.
// Parameters:
// const int deviceID - The id of the device to get.
// Output:
// InputDevice*
// On success - Returns the retrieved input device.
// On failure - Returns NULL.
//=============================================================================
InputDevice* InputSystem::getDevice(const int deviceID) const
{
	InputDevice* device = NULL;

	auto mit = m_devices.find(deviceID);

	if (mit != m_devices.end())
	{
		device = mit->second;
	}

	return device;
}

//=============================================================================
// Function: void addInputDevice(const int, InputDevice*)
// Description:
// Adds an input device under the specified ID.
// If there's already a device under that ID, it just deletes the
// input device trying to be assigned.
// Parameters:
// const int deviceID - The device ID to assign it under.
// InputDevice* device - The device to add.
//=============================================================================
void InputSystem::addInputDevice(const int deviceID,
	InputDevice* device)
{
	auto mit = m_devices.find(deviceID);

	if(mit == m_devices.end())
	{
		m_devices.insert(std::make_pair(deviceID, device));
	}
	else
	{
		delete device;
		device = nullptr;
	}
}

//=============================================================================
// Function: void removeInputDevice(const int)
// Description:
// Removes the input device under the specified ID.
// Parameters:
// const int deviceID - The ID of the device to remove.
//=============================================================================
void InputSystem::removeInputDevice(const int deviceID)
{
	auto mit = m_devices.find(deviceID);

	if (mit != m_devices.end())
	{
		delete mit->second;

		m_devices.erase(mit);
	}
}

//=============================================================================
// Function: void cleanup()
// Description:
// Cleans up all of the allocated devices and frees the memory.
//=============================================================================
void InputSystem::cleanup()
{
	auto mit = m_devices.begin();

	while (mit != m_devices.end())
	{
		delete mit->second;

		mit = m_devices.erase(mit);

		if (mit != m_devices.end())
		{
			mit++;
		}
	}
}