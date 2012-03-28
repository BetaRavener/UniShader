/*
* UniShader - Interface for GPGPU and working with shader programs
* Copyright (c) 2011-2012 Ivan Sevcik - ivan-sevcik@hotmail.com
*
* This software is provided 'as-is', without any express or
* implied warranty. In no event will the authors be held
* liable for any damages arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented;
*    you must not claim that you wrote the original software.
*    If you use this software in a product, an acknowledgment
*    in the product documentation would be appreciated but
*    is not required.
*
* 2. Altered source versions must be plainly marked as such,
*    and must not be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any
*    source distribution.
*/

#pragma once
#ifndef SIGNAL_H
#define SIGNAL_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>
#include <UniShader/ObjectBase.h>

#include <memory>
#include <deque>
#include <typeinfo>

UNISHADER_BEGIN

class SignalSender;

//! Signal receiver.
/*! 
	Signal receiver is base class for receiving signals produced by other interface classes.
*/

class UniShader_API SignalReceiver{
protected:
	SignalReceiver();
public:
	~SignalReceiver();

	//! Handle incoming signal.
	/*!
		\param signalID Signal identifier.
		\param callerPtr Pointer to object sending signal.
		\return True if handled.
	*/
	virtual bool handleSignal(unsigned int signalID, const ObjectBase* callerPtr) = 0;
protected:
	std::shared_ptr<SignalReceiver*> signalPtr; //<! Shared pointer to receiver.
};

//! Signal sender.
/*!
	Signal sender is base class for sending signals to other interface classes.
*/

class UniShader_API SignalSender{
protected:
	SignalSender();
public:
	~SignalSender();
	
	//! Subscribe receiver
	/*!
		Subscribe new receiver that will receive signals from this sender.
		If receiver is already subscribed, function returns silently.
		\param ptr Pointer to signal receiver.
	*/
	void subscribeReceiver(std::shared_ptr<SignalReceiver*>& ptr);

	//! Unsubscribe receiver.
	/*!
		Unsubscribe subscribed receiver.
		If receiver isn't subscribed, function returns silently.
		\param ptr Pointer to signal receiver.
	*/
	void unsubscribeReceiver(std::shared_ptr<SignalReceiver*>& ptr);
protected:

	//! Send signal.
	/*!
		Send signal to all subscribed receivers.
		\param signalID Signal identifier.
		\param ptr Pointer to sender.
	*/
	void sendSignal(unsigned int signalID, const ObjectBase* ptr);
private:
	std::deque<std::shared_ptr<SignalReceiver*>> m_subscReceivers;
};

UNISHADER_END

#endif
