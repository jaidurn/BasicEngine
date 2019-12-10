#pragma once
//==========================================================================================
// File Name: InputSystem.h
// Author: Brian Blackmon
// Date Created: 12/3/2019 
// Purpose: 
// Handles all of the input for the game.
//==========================================================================================
#include <SDL.h>
#include <map>

class InputDevice;

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void update(SDL_Event& e);

	// Device registration
	InputDevice* getDevice(const int deviceID) const;
	void addInputDevice(const int deviceID, 
		InputDevice* device);
	void removeInputDevice(const int deviceID);

private:
	const int m_KBM_ID = -2;

	std::map<int, InputDevice*> m_devices;

	void cleanup();
};

