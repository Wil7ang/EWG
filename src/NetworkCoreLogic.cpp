/*
 * src/NetworkCoreLogic.cpp
 *
 * Copyright (C) 2011 Wilson Tang.
 *
 * This file is part of Extravagant Word Game and is free software, distributed,
 * and licensed under the GNU General Public License, version 3.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/gpl-3.0.html>.
*/

#include "NetworkCore.h"
#include <time.h>
#include <sstream>
#include "Utility.h"

#include <fstream>
using namespace std;


void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    srand ( time(NULL) );
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

std::string choices[4] = { "/A", "/B", "/C", "/D" };

void NetworkCore::ServerProcess( http_request *rqt, EWGCore *eCore )
{
	Socket s = *( rqt->s_ );

	std::string title;
	std::string bgcolor = "#fff";
	std::string meta;
	std::string page;
	std::string style;

    if(!eCore->IsStarted())
    {
        if( rqt->path_ == "/" )
            {
                title = "Pick";
                meta = "<meta http-equiv=\"refresh\" content=\"3\" />";
                style = "<style type=\"text/css\">*{margin:0;padding:0;background:"+bgcolor+";}html,body{height:100%;width:100%;}body { text-align:center;min-height:468px;background:"+bgcolor+";}#xouter{height:100%;width:100%;display:table;vertical-align:middle;}#xcontainer {text-align: center;position:relative;vertical-align:middle;display:table-cell;}	#xinner {background:#fff;text-align: center;margin-left:auto;margin-right:auto;border:0px;}</style>";
                page = "<div id=\"xouter\"><div id=\"xcontainer\"><div id=\"xinner\"><center><table border=5><tr height=85>";
                if(!chosen[0])
                    page += "<td width=85><a href=\"A\"><img border=0 width=85 height=85 src=\"https://sites.google.com/site/wil7ang/projects/extravagant-word-game/A.jpg\"></a></td>";
                else
                    page += "<td width=85>&nbsp;</td>";
                if(!chosen[1])
                    page += "<td width=85><a href=\"B\"><img border=0 width=85 height=85 src=\"https://sites.google.com/site/wil7ang/projects/extravagant-word-game/B.jpg\"></a></td>";
                else
                    page += "<td width=85>&nbsp;</td>";
                if(!chosen[2])
                    page += "<td width=85><a href=\"C\"><img border=0 width=85 height=85 src=\"https://sites.google.com/site/wil7ang/projects/extravagant-word-game/C.jpg\"></a></td>";
                else
                    page += "<td width=85>&nbsp;</td>";
                if(!chosen[3])
                    page += "<td width=85><a href=\"D\"><img border=0 width=85 height=85 src=\"https://sites.google.com/site/wil7ang/projects/extravagant-word-game/D.jpg\"></a></td>";
                else
                    page += "<td width=85>&nbsp;</td>";
                page += "</tr></table><br><br><a href=\"/\"><font size=7>Stuck?</font></a></center></div></div></div></body></html>";
            }

        for(int i = 0; i<4; i++)
        {
            if( rqt->path_ == choices[i] )
            {
                if(!chosen[i])
                {
                    char guid[64];
                    gen_random(guid, 64);
                    guids[i] = guid;

                    title = "Selected A";
                    meta = "<meta http-equiv=\"refresh\" content=\"0;url=/";
                    meta += guids[i];
                    meta += "/Rack\" />";
                    style = "<style type=\"text/css\">* {margin:0;padding:0;background:#fff;}html,body{height:100%;width:100%;}/* end hide */body { text-align:center;min-height:468px;/* for ie7*/background:#fff;}#xouter{height:100%;width:100%;display:table;vertical-align:middle;}#xcontainer {text-align: center;position:relative;vertical-align:middle;display:table-cell;}#xinner {background:#fff;text-align: center;margin-left:auto;margin-right:auto;border:0px;}</style>";
                    page = "<a href=\"/";
                    page += guids[i];
                    page += "/Rack\">";
                    page += "<div id=\"xouter\"><div id=\"xcontainer\"><div id=\"xinner\"><center><font size=7>GO!</font>";
                    page += "</center></div></div></div></a></body></html>";

                    chosen[i] = true;

                    eCore->SetGuid(guid, i);
                    eCore->AddPlayer(guid);

                    break;
                }
                else
                {
                    title = "already chosen!";
                    meta = "<meta http-equiv=\"refresh\" content=\"0;url=/\" />";
                    page = "</body></html>";
                    break;
                }
            }
        }
    }

	for(int i = 0; i<4; i++)
	{
	    std::string path = "/";
	    path += guids[i];
	    path += "/Rack";
	    if( rqt->path_ == path )
	    {
	        bool isTurn = eCore->IsTurn(guids[i]);
	        Player *curPlayer = eCore->GetPlayer(guids[i]);
	        if(curPlayer != NULL)
	        {
                std::string lettersList[27] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "BLANK" };

                if(isTurn)
                    bgcolor = "#009900";

                title = "Rack";
                meta = "<meta http-equiv=\"refresh\" content=\"3\" />";
                style = "<style type=\"text/css\">* {margin:0;padding:0;background:"+bgcolor+";}html,body{height:100%;width:100%;}/* end hide */body { text-align:center;min-height:468px;/* for ie7*/background:#fff;}#xouter{height:100%;width:100%;display:table;vertical-align:middle;}#xcontainer {text-align: center;position:relative;vertical-align:middle;display:table-cell;}#xinner {background:#fff;text-align: center;margin-left:auto;margin-right:auto;border:0px;}</style>";
                page = "<div id=\"xouter\"><div id=\"xcontainer\"><div id=\"xinner\"><center><font size=7>Score: ";
                page += IntToString(curPlayer->GetScore());
                page += "</font><br><br>";
                page += "<table border=5><tr height=85>";
                for(int g = 0; g<7;g++)
                {
                    int letterNumber = curPlayer->GetTile(g).letter;
                    page += "<td width=85>";
                    if(letterNumber != -1 && !curPlayer->IsPlaced(g))
                    {
                        if(isTurn)
                        {
                            page += "<a href=\"";
                            page += path;
                            page += "/";
                            page += IntToString(g);
                            page += "\">";
                        }

                        page += "<img border=0 width=85 height=85 src=\"https://sites.google.com/site/wil7ang/projects/extravagant-word-game/";
                        //page += "<font size=7>";
                        //page += lettersList[letterNumber];
                        //page += "</font>";
                        page += lettersList[letterNumber];
                        page += ".jpg\">";

                        if(isTurn)
                        {
                            page += "</a>";
                        }
                    }
                    page += "</td>";
                }
	        }
            page += "</tr></table><br><br><a href=\"";
	        page += path;
	        page += "\"><font size=7>Stuck?</font></a>";
	        page += "</center></div></div></div></body></html>";
	        break;
	    }
	}

    if(eCore->IsStarted())
    {

        for(int i = 0; i<4; i++)
        {

            std::string path = "/";
            bool isTurn = eCore->IsTurn(guids[i]);
            if(isTurn)
            {
                Player *curPlayer = eCore->GetPlayer(guids[i]);
                path += guids[i];
                path += "/Rack/";
                for(int g = 0; g<7; g++)
                {
                    std::string pth = path;
                    pth += IntToString(g);
                    if( rqt->path_ == pth && !curPlayer->IsPlaced(g))
                    {
                        title = "Place tile: ";
                        title += IntToString(g);

                        eCore->AddTile(curPlayer->PlaceTile(g), g);

                        meta = "<meta http-equiv=\"refresh\" content=\"0;url=/";
                        meta += guids[i];
                        meta += "/Rack\" />";
                        style = "<style type=\"text/css\">* {margin:0;padding:0;background:#fff;}html,body{height:100%;width:100%;}/* end hide */body { text-align:center;min-height:468px;/* for ie7*/background:#fff;}#xouter{height:100%;width:100%;display:table;vertical-align:middle;}#xcontainer {text-align: center;position:relative;vertical-align:middle;display:table-cell;}#xinner {background:#fff;text-align: center;margin-left:auto;margin-right:auto;border:0px;}</style>";
                        page = "<a href=\"/";
                        page += guids[i];
                        page += "/Rack\">";
                        page += "<div id=\"xouter\"><div id=\"xcontainer\"><div id=\"xinner\"><center><font size=7>GO!</font>";
                        page += "</center></div></div></div></a></body></html>";
                        break;
                    }
                }
            }
        }
    }

	rqt->answer_  = "<html><head><title>";
	rqt->answer_ += title;
	rqt->answer_ += "</title>";
	rqt->answer_ += style;
	rqt->answer_ += meta;
	rqt->answer_ += "</head>";
	rqt->answer_ += page;
}
