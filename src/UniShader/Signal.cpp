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

#include <UniShader/Signal.h>

using UNISHADER_NAMESPACE;

SignalReceiver::SignalReceiver():
signalPtr(std::shared_ptr<SignalReceiver*>(new SignalReceiver*)){
	(*signalPtr) = this;
}

SignalReceiver::~SignalReceiver(){
	(*signalPtr) = 0;
}

SignalSender::SignalSender(){
}

SignalSender::~SignalSender(){
}

void SignalSender::subscribeReceiver(std::shared_ptr<SignalReceiver*>& ptr){
	for(std::deque<std::shared_ptr<SignalReceiver*>>::iterator it = m_subscReceivers.begin(); it != m_subscReceivers.end(); it++){
		//check for duplicate
		if((*it) == ptr)
			return;
	}
	m_subscReceivers.push_back(ptr);

	return;
}

void SignalSender::unsubscribeReceiver(std::shared_ptr<SignalReceiver*>& ptr){
	for(std::deque<std::shared_ptr<SignalReceiver*>>::iterator it = m_subscReceivers.begin(); it != m_subscReceivers.end(); it++){
		if((*it) == ptr){
			m_subscReceivers.erase(it);
			return;
		}
	}
}

void SignalSender::sendSignal(unsigned int signalID, const ObjectBase* ptr){
	std::deque<std::shared_ptr<SignalReceiver*>>::iterator it = m_subscReceivers.begin();

	while(it != m_subscReceivers.end()){
		if( *it ){
			(*(*it))->handleSignal(signalID, ptr);
			it++;
		}
		else{
			//if subscribed receiver doesn't exist anymore erase it
			it = m_subscReceivers.erase(it);
		}
	}
}
