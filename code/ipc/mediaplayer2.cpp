/************************** mediaplayer2.cpp ***************************

Code for the MediaPlayer2 interface on DBus.  When registered MBMP
will communicate to other processes.  

Copyright (C) 2013-2020
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
# include <QtDBus/QDBusMessage>
# include <QMap>
# include <QList>
# include <QVariant>

# include "./mediaplayer2.h"

//  header files generated by qmake from the xml file created by qdbuscpp2xml
//# include "./ipcagent_adaptor.h"
//# include "./ipcagent_interface.h"

//  constructor
MediaPlayer2::MediaPlayer2(Mpris2* parent) : QDBusAbstractAdaptor(parent)
{
	// initialize agent properties
	canquit = true;
	fullscreen = false;
	cansetfullscreen = true;
	canraise = false;
	hastracklist = false;
	identity = LONG_NAME;
	desktopentry = APP;
	supportedurischemes << "file" << "http";
	supportedmimetypes << "audio/*" << "video/*";
	
	// data members
	changeditems.clear();
	
  return;
}  
    


////////////////////////////////// Public Slots ////////////////////////
void MediaPlayer2::Quit()
{
	static_cast<Mpris2*>(this->parent())->emitApplicationStop();
}

/////////////////////// Private Functions ///////////////////////////////
//
// Function to emit the org.freedesktop.Dbus.Properties.PropertiesChanged()
// DBus signal.  Called from the local inline setxxx functions
void MediaPlayer2::sendPropertyChanged()
{
	// changed properties
	if (changeditems.size() <= 0) return;  

	// create a map of all changed items. Use for loop because it is possible
	// (but highly unlikely) that this slot could be called with more than 
	// one property needing to be changed 
  QMap<QString, QVariant> vmap;
  for (int i = 0; i < changeditems.size(); ++i) {
		switch (changeditems.at(i) ) {
			case MBMP_MPRIS::CanQuit:
				vmap["CanQuit"] = QVariant(canquit);
				break;
			case MBMP_MPRIS::Fullscreen:
				vmap["Fullscreen"] = QVariant(fullscreen); 
				break;
			case MBMP_MPRIS::CanSetFull:
				vmap["CanSetFullscreen"] = QVariant(cansetfullscreen);
				break;
			case MBMP_MPRIS::CanRaise:
				vmap["CanRaise"] = QVariant(canraise);
				break;
			case MBMP_MPRIS::HasTrackList:
				vmap["HasTrackList"] = QVariant(hastracklist);
				break;
			case MBMP_MPRIS::Identity:
				vmap["Identity"] = QVariant(identity);
				break;
			case MBMP_MPRIS::DesktopEntry:
				vmap["DesktopEntry"] = QVariant(desktopentry);
				break;
			case MBMP_MPRIS::UriSchemes:
				vmap["SupportedUriSchemes"] = QVariant(supportedurischemes);
				break;
			case MBMP_MPRIS::MimeTypes:
				vmap["SupportedMimeTypes"] = QVariant(supportedmimetypes);
				break;
			default:
				break;
		}	// switch				
  }	// for
  changeditems.clear();
  
  // create the message.  We never remove a property so we don't need
  // to deal with that - send an empty qstringlist
  QList<QVariant> vlist;
  vlist << QVariant(IPC_INTERFACE_MEDIAPLAYER2) << vmap << QStringList();
  QDBusMessage msg = QDBusMessage::createSignal(IPC_OBJECT, "org.freedesktop.Dbus.Properties", "PropertiesChanged");  
  msg.setArguments(vlist);																																														
	QDBusConnection::sessionBus().send(msg);
	
	return;
}	

