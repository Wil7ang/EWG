/*
 * src/NetworkCore.cpp
 *
 * This file is a heavily modified version of René Nyffenegger's Webserver.h
 * Copyright (C) 2003-2004 René Nyffenegger
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#ifndef NETWORKCORE_H
#define NETWORKCORE_H

#include <string>
#include <map>
#include "sws/socket.h"

#include "EWGCore.h"

class Socket;

struct http_request
{

	http_request() : authentication_given_( false ) {}

	Socket*                            s_;
	std::string                        method_;
	std::string                        path_;
	std::map<std::string, std::string> params_;

	std::string                        accept_;
	std::string                        accept_language_;
	std::string                        accept_encoding_;
	std::string                        user_agent_;

	/* status_: used to transmit server's error status, such as
	   o  202 OK
	   o  404 Not Found
	   and so on */
	std::string                        status_;

	/* auth_realm_: allows to set the basic realm for an authentication,
	   no need to additionally set status_ if set */
	std::string                        auth_realm_;

	std::string                        answer_;

	/*   authentication_given_ is true when the user has entered a username and password.
	     These can then be read from username_ and password_ */
	bool authentication_given_;
	std::string username_;
	std::string password_;
};

class NetworkCore
{
	public:
		NetworkCore( unsigned int port_to_listen );
		void process(EWGCore *eCore);
	private:
		//static unsigned __stdcall Request(void*);
		unsigned int Request( Socket *skt, EWGCore *eCore );
		//static request_func request_func_;
		void ServerProcess( http_request* rqt, EWGCore *eCore );

		SocketServer in;

		std::string guids[4];
		bool chosen[4];
};

#endif
