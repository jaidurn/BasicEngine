#include "InputSystem.h"
#include "LogLocator.h"
#include "KBM.h"
#include "Gamepad.h"
#include "MessageLocator.h"

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

			Message *message = new Message(MSG_INPUT_BUTTON);

			message->button.deviceID = e.cbutton.which;
			message->button.button = device->getButtonCode(e.cbutton.button);
			message->button.pressed = true;

			if (!MessageLocator::getMsgSys().pushMessage(message))
			{
				delete message;
			}
		}

		break;
	}
	case SDL_CONTROLLERBUTTONUP:
	{
		InputDevice* device = getDevice(e.cbutton.which);

		if (device)
		{
			device->releaseButton(e.cbutton.button);

			Message *message = new Message(MSG_INPUT_BUTTON);

			message->button.deviceID = e.cbutton.which;
			message->button.button = device->getButtonCode(e.cbutton.button);
			message->button.pressed = false;

			if (!MessageLocator::getMsgSys().pushMessage(message))
			{
				delete message;
			}
		}

		break;
	}
	case SDL_CONTROLLERAXISMOTION:
	{
		InputDevice* device = getDevice(e.caxis.which);

		if (device->getDeviceType() == InputDevice::INPUTDEVICE_GAMEPAD)
		{
			Gamepad* gamepad = dynamic_cast<Gamepad*>(device);

			if (gamepad)
			{
				gamepad->setAxis(e.caxis.axis, e.caxis.value);

				Message *message = new Message(MSG_INPUT_AXIS);

				message->axis.deviceID = e.caxis.which;
				message->axis.axis = gamepad->getAxisCode(e.caxis.axis);
				message->axis.amount = gamepad->getAxisValue(message->axis.axis);

				if (!MessageLocator::getMsgSys().pushMessage(message))
				{
					delete message;
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

			Message *message = new Message(MSG_INPUT_BUTTON);

			message->button.deviceID = m_KBM_ID;
			message->button.button = device->getButtonCode(e.key.keysym.sym);
			message->button.pressed = true;

			if (!MessageLocator::getMsgSys().pushMessage(message))
			{
				delete message;
			}
		}
		break;
	}
	case SDL_KEYUP:
	{
		InputDevice* device = getDevice(m_KBM_ID);

		if (device)
		{
			device->releaseButton(e.key.keysym.sym);
			Message *message = new Message(MSG_INPUT_BUTTON);

			message->button.deviceID = m_KBM_ID;
			message->button.button = device->getButtonCode(e.key.keysym.sym);
			message->button.pressed = false;

			if (!MessageLocator::getMsgSys().pushMessage(message))
			{
				delete message;
			}
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

				Message *message = new Message(MSG_INPUT_BUTTON);

				message->mouse.deviceID = m_KBM_ID;
				message->mouse.position = kbm->getPosition();

				if (!MessageLocator::getMsgSys().pushMessage(message))
				{
					delete message;
				}
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
			
			Message *message = new Message(MSG_INPUT_BUTTON);

			message->button.deviceID = m_KBM_ID;
			message->button.button = device->getButtonCode(e.button.button);
			message->button.pressed = true;

			if (!MessageLocator::getMsgSys().pushMessage(message))
			{
				delete message;
			}
		}

		break;
	}
	case SDL_MOUSEBUTTONUP:
	{
		InputDevice* device = getDevice(m_KBM_ID);

		if (device)
		{
			device->releaseButton(e.button.button);

			Message *message = new Message(MSG_INPUT_BUTTON);

			message->button.deviceID = m_KBM_ID;
			message->button.button = device->getButtonCode(e.button.button);
			message->button.pressed = false;

			if (!MessageLocator::getMsgSys().pushMessage(message))
			{
				delete message;
			}
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