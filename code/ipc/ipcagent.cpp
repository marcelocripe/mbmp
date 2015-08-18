/**************************** ipcagent.cpp ***************************

Code for the ipc agentr registered on DBus.  When registered MBMP
will communicate to other processes.  This program and registering on
dbus will be started in the constructor.

Copyright (C) 2013-2015
by: Andrew J. Bibb
License: MIT 

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"),to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions: 

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
***********************************************************************/  

# include <QtCore/QDebug>
# include <QtDBus/QDBusConnection>
//# include <QDir>
//# include <QFile>
//# include <QFileInfo>

# include "./ipcagent.h"

//  header files generated by qmake from the xml file created by qdbuscpp2xml
# include "ipcagent_adaptor.h"
# include "ipcagent_interface.h"

# define IPC_SERVICE "org.mbmp.IPC"
# define IPC_OBJECT "/agent"

//  constructor
IPC_Agent::IPC_Agent(QObject* parent) : QObject(parent)
{
  //  Create ipcagentadaptor  
  new IpcagentAdaptor(this);  

	// Try to register a service on the system bus
	if (! QDBusConnection::sessionBus().registerService(IPC_SERVICE)) {
		#if QT_VERSION >= 0x050400 
			qCritical("Failed to register service %s on the session bus - there may be another instance running.", qUtf8Printable(IPC_SERVICE) );
		# else	
			qCritical("Failed to register service %s on the session bus - there may be another instance running.", qPrintable(IPC_SERVICE) );
		# endif
		QCoreApplication::instance()->exit(1);
	}	// if registering service failed
	
	// try to register an object on the system bus
		if (! QDBusConnection::sessionBus().registerObject(IPC_OBJECT, this)) {
			qDebug() << tr("Failed to register agent object on the session bus.");
			#if QT_VERSION >= 0x050400 
				qCritical("Failed to register object %s on the session bus.", qUtf8Printable(IPC_OBJECT) );
			# else	
				qCritical("Failed to register object %s on the session bus.", qPrintable(IPC_OBJECT) );
			# endif
				QCoreApplication::instance()->exit(2);
		}	// if registering object failed
		
		// data members
		vmap.clear();
		
  return;
}  
    
/////////////////////// Public Functions /////////////////////////////////////
//
// Function to shutdown the agent and unregister the object and service from 
// the session bus.  Called from PlayerCtl in the cleanUp() function.
void IPC_Agent::stopAgent()
{
	QDBusConnection::sessionBus().unregisterObject(IPC_OBJECT);
	
	if (! QDBusConnection::sessionBus().unregisterService(IPC_SERVICE) ) {
		#if QT_VERSION >= 0x050400 
			qCritical("Failed to unregister service %s on the session bus.", qUtf8Printable(IPC_SERVICE) );
		# else	
			qCritical("Failed to unregister service %s on the session bus.", qPrintable(IPC_SERVICE) );
		# endif	
	}	// if
		
	return;
}
    
/////////////////////// Public Slots (Methods) //////////////////////////
//
// Function to stop the player
void IPC_Agent::stopPlayer()
{
	
	qDebug() << "!!!!!!!!!!!!!! INSIDE STOP PLAYER !!!!!!!!!!!!!!!!!!!!!!!";
	
	return;
}
