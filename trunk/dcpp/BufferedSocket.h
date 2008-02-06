/*
 * Copyright (C) 2001-2007 Jacek Sieka, arnetheduck on gmail point com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef DCPLUSPLUS_DCPP_BUFFERED_SOCKET_H
#define DCPLUSPLUS_DCPP_BUFFERED_SOCKET_H

#include "forward.h"
#include "BufferedSocketListener.h"
#include "Semaphore.h"
#include "Thread.h"
#include "Speaker.h"
#include "Util.h"
#include "Socket.h"

namespace dcpp {

class BufferedSocket : public Speaker<BufferedSocketListener>, public Thread
{
public:
	enum Modes {
		MODE_LINE,
		MODE_ZPIPE,
		MODE_DATA
	};

	/**
	 * BufferedSocket factory, each BufferedSocket may only be used to create one connection
	 * @param sep Line separator
	 * @return An unconnected socket
	 */
	static BufferedSocket* getSocket(char sep) throw() {
		return new BufferedSocket(sep);
	}

	static void putSocket(BufferedSocket* aSock) {
		aSock->removeListeners();
		aSock->shutdown();
	}

	static void waitShutdown() {
		while(sockets)
			Thread::sleep(100);
	}

	void accept(const Socket& srv, bool secure, bool allowUntrusted) throw(SocketException, ThreadException);
	void connect(const string& aAddress, uint16_t aPort, bool secure, bool allowUntrusted, bool proxy) throw(SocketException, ThreadException);

	/** Sets data mode for aBytes bytes. Must be called within onLine. */
	void setDataMode(int64_t aBytes = -1) { mode = MODE_DATA; dataBytes = aBytes; }
	/**
	 * Rollback is an ugly hack to solve problems with compressed transfers where not all data received
	 * should be treated as data.
	 * Must be called from within onData.
	 */
	void setLineMode(size_t aRollback) { setMode (MODE_LINE, aRollback);}
	void setMode(Modes mode, size_t aRollback = 0);
	Modes getMode() const { return mode; }
	const string& getIp() const { return sock ? sock->getIp() : Util::emptyString; }
	bool isConnected() const { return sock && sock->isConnected(); }

	bool isSecure() const { return sock && sock->isSecure(); }
	bool isTrusted() const { return sock && sock->isTrusted(); }
	std::string getCipherName() const { return sock ? sock->getCipherName() : Util::emptyString; }
	
	void write(const string& aData) throw() { write(aData.data(), aData.length()); }
	void write(const char* aBuf, size_t aLen) throw();
	/** Send the file f over this socket. */
	void transmitFile(InputStream* f) throw() { Lock l(cs); addTask(SEND_FILE, new SendFileInfo(f)); }

	void disconnect(bool graceless = false) throw() { Lock l(cs); if(graceless) disconnecting = true; addTask(DISCONNECT, 0); }

	string getLocalIp() const { return sock ? sock->getLocalIp() : Util::getLocalIp(); }

	GETSET(char, separator, Separator)
private:
	enum Tasks {
		CONNECT,
		DISCONNECT,
		SEND_DATA,
		SEND_FILE,
		SHUTDOWN,
		ACCEPTED
	};

	struct TaskData {
		virtual ~TaskData() { }
	};
	struct ConnectInfo : public TaskData {
		ConnectInfo(string addr_, uint16_t port_, bool proxy_) : addr(addr_), port(port_), proxy(proxy_) { }
		string addr;
		uint16_t port;
		bool proxy;
	};
	struct SendFileInfo : public TaskData {
		SendFileInfo(InputStream* stream_) : stream(stream_) { }
		InputStream* stream;
	};

	BufferedSocket(char aSeparator) throw();

	// Dummy...
	BufferedSocket(const BufferedSocket&);
	BufferedSocket& operator=(const BufferedSocket&);

	virtual ~BufferedSocket() throw();

	CriticalSection cs;

	Semaphore taskSem;
	vector<pair<Tasks, TaskData*> > tasks;

	Modes mode;
	UnZFilter *filterIn;
	int64_t dataBytes;
	size_t rollback;
	bool failed;
	string line;
	ByteVector inbuf;
	ByteVector writeBuf;
	ByteVector sendBuf;

	Socket* sock;
	bool disconnecting;

	virtual int run();

	void threadConnect(const string& aAddr, uint16_t aPort, bool proxy) throw(SocketException);
	void threadRead() throw(SocketException);
	void threadSendFile(InputStream* is) throw(Exception);
	void threadSendData();
	void threadDisconnect();

	void fail(const string& aError);
	static volatile long sockets;

	bool checkEvents();
	void checkSocket();

	void shutdown();
	void addTask(Tasks task, TaskData* data) { tasks.push_back(make_pair(task, data)); taskSem.signal(); }
};

} // namespace dcpp

#endif // !defined(BUFFERED_SOCKET_H)