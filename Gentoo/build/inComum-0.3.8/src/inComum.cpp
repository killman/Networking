#if ENABLE_BOOST
  #include <boost/typeof/std/iostream.hpp>
  #include <boost/regex.h>
  #include <boost/algorithm/string.hpp>
  #include <boost/typeof/vector.hpp>
#else
  #include <iostream>
  #include <regex.h>
  #include <string>
  #include <vector>
#endif
#if debug
	#include <fstream>
	#include <sys/types.h>
	#include <sstream>
#endif

/*
 * BSD License
 *
Copyright (c) 2012, inComum Team.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. The names of developers may not be used to endorse or promote
products derived from this software without specific prior written
permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*
* to compile, use following line:
* g++ -Wall -o "inComum" "inComum.cpp"
* If using boost library, use this:
* g++ -DENABLE_BOOST -lboost_regex -o inComum inComum.cpp
* To compile in debug mode (to show URL rewrites on /etc/inComum-*.log)
* g++ -Ddebug -o inComum_debug inComum.cpp
*
* If you add another plugin, please, send us a copy to email:
* luciano.pinheiro@gmail.com
*
* verion 0.3.8
*
*/

using namespace std;

string get_var(const string url,const string var); //get some var from url
string get_path(const string url, const char removeQuery); //path only (optionally remove query string)
string get_domain(const string url); //get domain only (remove path)
int regexMatch(string er, string line); //implement regex by regex.h

int main(int argc, char **argv)
{
	string url, urlf, id, domain;
	string::size_type x;

	#if debug
		pid_t pid;
		pid = getpid();

		stringstream stream;
		stream << "/tmp/inComum-" << pid << ".log";
		string arraystring = stream.str();

		ofstream SaveLine(arraystring.c_str(), ios::out);
		//ofstream SaveLine("/tmp/inComum-" + pid + ".log", ios::out);
	#endif

	if(argc > 1){
		if(argv[1][0] == '-' && argv[1][1] == 'v' ) {
			cout << "inComum 0.3.8 (2012-02-02) http://sourceforge.net/projects/incomum/" << endl;
			cout << "===========" << endl;
			cout << "-youtube" << endl;
			cout << "-googlevideo" << endl;
			cout << "-orkut img/static" << endl;
			cout << "-ggpht" << endl;
			cout << "-tumblr" << endl;
			cout << "-photobucket" << endl;
			cout << "-avast" << endl;
			cout << "-avg" << endl;
			cout << "-vimeo" << endl;
			cout << "-blip.tv" << endl;
			cout << "-globo.com" << endl;
			cout << "-tvuol.uol.com.br" << endl;
			cout << "-redtube.com" << endl;
			cout << "-porntube.com" << endl;
			cout << "-xvideos.com" << endl;
			cout << "-globo.com" << endl;
			cout << "-msn catalog" << endl;
			cout << "-videobb.com" << endl;
			#if debug
			cout << "-=[ Running in debug mode. Check /tmp/inComum-*.log files ]=-" << endl;
			#endif
			return 0;
		}
	}

	while(getline(std::cin, url)){ // main loop receive all redirecting URL

		x = url.find_first_of(" ");
		if(x != string::npos){
			url = url.erase(x);
		}
		urlf = url;
		domain = get_domain(url);

		//https is just tunneling (can't cache) - don't cache ftp either
		if(url.substr(0,8) == "https://" || url.substr(0,6) == "ftp://"){
			cout << url << endl;
			continue;
		}

		//squid may send a blank line to exit
		if(url.empty() || url.substr(0,7) != "http://"){
			cout << url << endl;
			return 0;
		}

		// inComum plugins below

		//youtube/googlevideo plugin
		if(regexMatch("(youtube|googlevideo)\\.com/$", domain)){
			if(regexMatch("/(get_video|videoplayback|videoplay)\\?.*id=", url)){
				if(url.find("noflv=") != string::npos && url.find("ptk=") == string::npos){ //(noflv AND !ptk) => redirecting URL
					urlf = url;
				}else{
					urlf = "http://flv.youtube.inComum/?id="+get_var(url, "id")+get_var(url, "video_id")+"&quality="+get_var(url, "fmt")+get_var(url, "itag")+"&redirect_counter="+get_var(url, "redirect_counter")+get_var(url, "st")+"&begin="+get_var(url,"begin")+"&range="+get_var(url, "range");
				}
			}

		//orkut img/static plugin
		}else if(regexMatch("\\.orkut\\.com/$", domain)){
			if(regexMatch("^http://(img|static)[0-9]\\.", domain)){
				urlf = "http://orkut.inComum/"+get_path(url,'N');
			}

		//ytimg.com -last check: 2011-05-15
        }else if(regexMatch("\\.ytimg\\.com/$", domain)){
            if(regexMatch("^http://i(|.)\\.", domain)){
                urlf = "http://ytimg.inComum/"+get_path(url,'N');
            }

        //ggpht.com -last check: 2011-05-15
        }else if(regexMatch("\\.ggpht\\.com/$", domain)){
            if(regexMatch("^http://lh.\\.", domain)){
                urlf = "http://ggpht.inComum/"+get_path(url,'N');
            }

        //tumblr.com -last check: 2011-05-15
        }else if(regexMatch("\\.media\\.tumblr\\.com/$", domain)){
            if(regexMatch("^http://.{1,2}\\.", domain)){
                urlf = "http://tumblr.inComum/"+get_path(url,'N');
            }

        //photobucket.com -last check: 2011-05-15
        }else if(regexMatch("\\.photobucket\\.com/$", domain)){
            if(regexMatch("^http://(i|th).{1,4}\\.", domain)){
                urlf = "http://photobucket.inComum/"+get_path(url,'N');
            }

		//avast plugin
		}else if(regexMatch("\\.avast\\.com/$", domain)){
			if(regexMatch("^http://download", domain)){
				urlf = "http://download.avast.inComum/"+get_path(url,'N');
			}

		//AVG plugin - last check: 2012-02-02
		//example: http://af.avg.com/softw/80free/update/u8iavi4164u4162uy.bin
		}else if(regexMatch("\\.avg\\.c(om|z)/$", domain)){
			if(regexMatch("^http://(backup|a.|pupdate-aa)\\.avg\\.c(om|z)/softw/", url)){
				urlf = "http://avg.inComum/"+get_path(url,'N');
			}

		//vimeo plugin -last check: 2012-01-11
		// TODO: has start var?
		//example: http://av.vimeo.com/68769/772/77100523.mp4?token=1326296561_d50e78c38a4d49174b08e77c6b2bd0f4
		}else if(regexMatch("\\.vimeo\\.com/$", domain)){
			if(regexMatch("^http://av\\.vimeo\\.com/.*\\?token=", url)){
				urlf = "http://vimeo.inComum/"+get_path(url,'Y');
			}

		//metacafe plugin
		// TODO: has start var?
		}else if(regexMatch("mccont\\.com/$", domain)){
			if(regexMatch("^http://v\\.mccont\\.com/ItemFiles/%5BFrom%20www\\.metacafe\\.com%5D%.{20}flv\\?", url)){
				urlf = "http://metacafe.inComum/"+get_path(url,'Y');
			}

		//blip.tv plugin - last check 2012-02-02
		//example: http://a14.video2.blip.tv/10020009553710/GiselleAchecar-EcoRicoEp007232.m4v?brs=2493&bri=12.2
		//example: http://j14.video2.blip.tv/10020009553710/GiselleAchecar-EcoRicoEp007232.m4v?brs=2493&bri=12.2&showplayer=20120110140747&start=201&referrer=http://blip.tv
		//example: http://j2.video2.blip.tv/12950010298587/Sexynerdgirl-Vlog111Follywood875.m4v?brs=2782&bri=4.5&showplayer=20120110140747&start=3&referrer=http://blip.tv
		}else if(regexMatch("\\.blip\\.tv/$", domain)){
			if(regexMatch("video.*m4v", url)){
				urlf = "http://bliptv.inComum/"+get_path(url,'Y')+"?start="+get_var(url,"start");
			}

		//globo plugin
		// TODO: has start var?
		}else if(regexMatch("^http://flashvideo\\.globo\\.com/$", domain)){
			if(regexMatch("\\.mp4", url)){
				urlf = "http://flashvideo.globo.inComum/"+get_path(url,'Y');
			}

		//TV UOL plugin - last check 2011-12-30
		//example1: http://video7.mais.uol.com.br/12381354.mp4?ver=1
		//example2: http://storage.mais.uol.com.br/12304309.flv?cfmhwNs,bhpIoqWzmeyNg
		//example3: http://thumb.mais.uol.com.br/12093127-medium.jpg?ver=1
		}else if(regexMatch("^http://(video.{1,2}|thumb|storage)\\.mais\\.uol\\.com\\.br/", domain)){
			if(regexMatch("\\.(mp4|flv|jpg)\\?", url)){
				urlf = "http://maisuol.inComum/"+get_path(url,'Y');
			}

		//porntube - last check: 2012-01-04
		//example: http://wpc.porntube.com/main/0/0/0/2/6/9/9/3/6.m4v?8d60b61b4448364398b401494c894bc195a5f1bff1823579e8a03ccc17eec2c348849ab9508b639c426db8550c0166ff7bd490eabaa2ada9e7a5f94da78f4282fcc99e8d32f33a373e18c20d03b5b487ce2dbfd02702293e92cc64a3f31d18c68c2ceaa7272c272136751a6e8ea966b71abdf764538766263f1becd733138414b0fe603fb65537cd1c45b4ef16df2d2354349e98437484ba1be2861354ea04535f9fb37a62be5add9838fc2aba0c3e3d2fdff34af309d6632b06edd5cfd096feeda7&ec_seek=0
		}else if(regexMatch("^http://wpc\\.porntube\\.com/", domain)){
			if(regexMatch("\\.m4v\\?",url)){
				urlf = "http://porntube.inComum/" + get_path(url,'Y') + "?start=" + get_var(url, "ec_seek");
			}

		//redtube.com -last check: 2011-12-30
		//example: http://videos.flv2.redtubefiles.com/_videos_t4vn23s9jc5498tgj49icfj4678/0000090/_h264flv/0090017.flv?47aac1f74dca44b0626f9b5acf21d75cd5dee7e8937c9cd51b27114cbb64d1abdacd57b1c8f7662835b5852858f0556998fb41dad6317b0e2c76&ec_seek=49672846
		}else if(regexMatch("^http://(img..|videos\\.flv.{0,1})\\.redtubefiles\\.com/", domain)){
			urlf = "http://redtube.inComum/" + get_path(url, 'Y') + "?start=" + get_var(url, "ec_seek");

		//xvideos.com -last check: 2011-12-24
		//example: http://porn126.xvideos.com/videos/flv/9/b/4/xvideos.com_9b4f0e80763bb256f1a4e27a32ffcbe6.flv?e=1324741579&ri=1024&rs=85&h=1503fe575697f4dab651d1244ffc18fb
		} else if (regexMatch("^http://porn.{1,3}\\.xvideos\\.com/", domain)) {
			if (regexMatch("\\.flv\\?", url)) {
				urlf = "http://xvideos.inComum/" + get_path(url, 'Y') + "?start=" + get_var(url, "fs");
			}

		//msn catalog videos plugin
		// TODO: has start var?
		}else if(regexMatch("catalog.video.msn.com/.*share", url)){
			urlf = "http://catalog.msn.inComum/"+get_path(url,'N');

		//videobb.com -last check: 2012-01-24
		//example: http://s269.videobb.com/s?v=QeHwaooHQQ5G&t=1327417389&u=&r=2&c=e6db2e6cc7ea3366b4f65ff07e74cc6262ecd39d8727a94e9003e93210f1e4236b0352125b8fbd3af49be21c84030668&start=0
		}else if(regexMatch("^http://s[0-9]{0,3}\\.videobb\\.com/", domain)){
			urlf = "http://videobb.inComum/?id="+get_var(url, "v")+"&quality="+get_var(url, "r")+"&start="+get_var(url, "start");
		}

		#if debug
			if(urlf == url){
				SaveLine << url + " > (same)\n";
			}else{
				SaveLine << url + " > " + urlf + "\n";
			}
			SaveLine.flush();
		#endif

		//send urlf back to squid
		cout << urlf << endl;
	}
	#if debug
		SaveLine.close();
	#endif
	return 0;
}

/* return the value of a URL var */
string get_var(const string url,const string var)
{
	string par, v, valor, vars;

	if(url.find("?") != string::npos){
		vars = url.substr(url.find("?")+1);
	}

	while(!vars.empty()){
		par = vars.substr(vars.find_last_of("&")+1);
		v = par.substr(0,par.find_first_of("="));
		valor = par.substr(par.find_first_of("=")+1);
		if(v == var){
			return par.substr(par.find_first_of("=")+1);
		}
		if(vars.find_last_of("&") != string::npos){
			vars.erase(vars.find_last_of("&"));
		}else{
			vars.erase();
		}
	}
	return "";
}

/* return URL path */
string get_path(const string url, const char removeQuery)
{
	string temp, path;
	string::size_type x;

	temp = url.substr(7);
	x = temp.find_first_of("/");
	if(x != string::npos){
		path = temp.substr(x+1);
		if(removeQuery == 'Y'){
			x = path.find_first_of("?");
			if(x != string::npos){
				path = path.erase(x);
			}
		}
		return path;
	}else{
		return "";
	}
}

/* return URL domain */
string get_domain(const string url)
{
	string retorno;
	string::size_type x;

	x = url.find_first_of("/", 7);
	if(x != string::npos){
		return url.substr(0,x+1);
	}else{
		return url;
	}

}


/* regex */
int regexMatch(string er, string line)
{
    int error;
    regmatch_t match;
    regex_t reg;
    if ((regcomp(&reg, er.c_str(), REG_EXTENDED | REG_NEWLINE)) == 0) {
        error = regexec(&reg, line.c_str(), 1, &match, 0);
        if (error == 0) {
			regfree(&reg);
            return 1;
        }
    }
    regfree(&reg);
	return 0;
}
