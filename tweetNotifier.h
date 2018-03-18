#ifndef TWEETNOTIFIER_H
#define TWEETNOTIFIER_H

#include <stdlib.h>
#include <curl/curl.h>
#include <sstream>
#include <oauth.h>
#include <string>
#include <nlohmann/json.hpp>
#include <SFML/Audio.hpp>

enum TnStatus { Ok, Get_Id_Error, Stream_Connect_Error };

class TweetNotifier {
    private:
        static const std::string ID_URL;
        static const std::string STREAM_URL;
        std::string screen_name;
        const char *cons_key;
        const char *cons_sec;
        const char *atok_key;
        const char *atok_sec;

    public:
        TweetNotifier(
            char *screen_name,
            const char *cons_key, const char *cons_sec,
            const char *atok_key, const char *atok_sec,
            std::string sound_path = "sound/mario_mamma_mia.wav"
        );
        /*
            Starts notifier, connecting to the stream of the user
            given on screen_name.
            If something goes wrong it returns one of the errors.
            Otherwise it will keep running, receiving data from
            twitter api. If connection ends, it returns Ok.
        */
        TnStatus start_notifier(void);

};

#endif
