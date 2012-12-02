/*
 * src/NetworkCore.cpp
 *
 * This file is a heavily modified version of René Nyffenegger's Webserver.cpp
 * Copyright (C) 2003-2004 René Nyffenegger
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#include <ctime>
#include <process.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>


#include "NetworkCore.h"

#include "sws/UrlHelper.h"
#include "sws/base64.h"

unsigned int NetworkCore::Request( Socket* skt, EWGCore *eCore ) {

	std::string line = skt->ReceiveLine();
	if ( line.empty() ) {
			return 1;
		}

	http_request req;

	if ( line.find( "GET" ) == 0 ) {
			req.method_ = "GET";
		}
	else if ( line.find( "POST" ) == 0 ) {
			req.method_ = "POST";
		}

	std::string path;
	std::map<std::string, std::string> params;

	size_t posStartPath = line.find_first_not_of( " ", 3 );

	SplitGetReq( line.substr( posStartPath ), path, params );

	req.status_ = "202 OK";
	req.s_      = skt;
	req.path_   = path;
	req.params_ = params;

	static const std::string authorization   = "Authorization: Basic ";
	static const std::string accept          = "Accept: "             ;
	static const std::string accept_language = "Accept-Language: "    ;
	static const std::string accept_encoding = "Accept-Encoding: "    ;
	static const std::string user_agent      = "User-Agent: "         ;

	while( 1 ) {
			line = skt->ReceiveLine();

			if ( line.empty() ) break;

			unsigned int pos_cr_lf = line.find_first_of( "\x0a\x0d" );
			if ( pos_cr_lf == 0 ) break;

			line = line.substr( 0, pos_cr_lf );

			/*if ( line.substr( 0, authorization.size() ) == authorization ) {
					req.authentication_given_ = true;
					std::string encoded = line.substr( authorization.size() );
					std::string decoded = base64_decode( encoded );

					unsigned int pos_colon = decoded.find( ":" );

					req.username_ = decoded.substr( 0, pos_colon );
					req.password_ = decoded.substr( pos_colon + 1 );
				}
			else*/ if ( line.substr( 0, accept.size() ) == accept ) {
					req.accept_ = line.substr( accept.size() );
				}
			else if ( line.substr( 0, accept_language.size() ) == accept_language ) {
					req.accept_language_ = line.substr( accept_language.size() );
				}
			else if ( line.substr( 0, accept_encoding.size() ) == accept_encoding ) {
					req.accept_encoding_ = line.substr( accept_encoding.size() );
				}
			else if ( line.substr( 0, user_agent.size() ) == user_agent ) {
					req.user_agent_ = line.substr( user_agent.size() );
				}
		}

	ServerProcess(&req, eCore );

	std::stringstream str_str;
	str_str << req.answer_.size();

	time_t ltime;
	time( &ltime );
	tm* gmt = gmtime( &ltime );

	static std::string const serverName = "RenesWebserver (Windows)";

	char* asctime_remove_nl = asctime( gmt );
	asctime_remove_nl[24] = 0;

	skt->SendBytes( "HTTP/1.1 " );

	if ( ! req.auth_realm_.empty() ) {
			skt->SendLine( "401 Unauthorized" );
			skt->SendBytes( "WWW-Authenticate: Basic Realm=\"" );
			skt->SendBytes( req.auth_realm_ );
			skt->SendLine( "\"" );
		}
	else {
			skt->SendLine( req.status_ );
		}
	skt->SendLine( std::string( "Date: " ) + asctime_remove_nl + " GMT" );
	skt->SendLine( std::string( "Server: " ) + serverName );
	skt->SendLine( "Connection: close" );
	skt->SendLine( "Content-Type: text/html; charset=ISO-8859-1" );
	skt->SendLine( "Content-Length: " + str_str.str() );
	skt->SendLine( "" );
	skt->SendLine( req.answer_ );

	skt->Close();


	return 0;
}

NetworkCore::NetworkCore( unsigned int port_to_listen) {
	in.SetSocketServer( port_to_listen, 5, NonBlockingSocket );

	for(int i = 0; i < 4; i++)
    {
        guids[i] = "";
        chosen[i] = false;
    }
}

void NetworkCore::process(EWGCore *eCore) {

	Socket* ptr_s = NULL;
	ptr_s = in.Accept();

	if( ptr_s != NULL )
	{
		Request( ptr_s, eCore );
	}
}
