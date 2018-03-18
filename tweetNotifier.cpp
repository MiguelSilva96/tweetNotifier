#include <tweetNotifier.h>
#include <stdlib.h>
using namespace std;
using json = nlohmann::json;

/*
    Private helper functions
*/
namespace {
    typedef struct myData {
        string user_id;
        string data;
        sf::Sound sound;
    } *MyData;
    size_t write_id(char *ptr, size_t size, size_t nmemb, void *data);
    size_t detect_tweets(char *ptr, size_t size, size_t nmemb, void *data);
    int get_request(
        size_t (*writefun)(char*, size_t, size_t, void*),
        void *buffer, char *url
    );
}

namespace tn {
    const string TweetNotifier::ID_URL =
            string("https://api.twitter.com/1.1/users/show.json?screen_name=");
    const string TweetNotifier::STREAM_URL =
            string("https://stream.twitter.com/1.1/statuses/filter.json?follow=");

    // constructor
    TweetNotifier::TweetNotifier(
        char *screen_name,
        const char *cons_key, const char *cons_sec,
        const char *atok_key, const char *atok_sec,
        string sound_path
    ) : screen_name(screen_name),
        cons_key(cons_key),
        cons_sec(cons_sec),
        atok_key(atok_key),
        atok_sec(atok_sec) {
            if(!buffer.loadFromFile(sound_path)) {
                buffer.loadFromFile("sound/mario_mamma_mia.wav");
            }
            sound.setBuffer(buffer);
    }

    // start
    TnStatus TweetNotifier::start_notifier(void) {
        TnStatus status = Ok;
        string id_url = ID_URL + screen_name;
        MyData buffer = new struct myData;
        char *signed_url;
        buffer -> sound = sound;
        curl_global_init(CURL_GLOBAL_ALL);

        signed_url = oauth_sign_url2(
            id_url.c_str(), NULL, OA_HMAC, "GET",
            cons_key, cons_sec, atok_key, atok_sec
        );
        if(get_request(write_id, (void *)buffer, signed_url)) {
            string stream_url = STREAM_URL + (buffer -> user_id);

            signed_url = oauth_sign_url2(
                stream_url.c_str(), NULL, OA_HMAC, "GET",
                cons_key, cons_sec, atok_key, atok_sec
            );

            if(get_request(detect_tweets, (void *)buffer, signed_url));
            else status = Stream_Connect_Error;
        }
        else
            status = Get_Id_Error;
        curl_global_cleanup();
        delete buffer;
        return status;
    }
}

namespace {
    size_t write_id(char *ptr, size_t size, size_t nmemb, void *data) {
        size_t realSize = size * nmemb;
        string str(ptr, realSize);
        json j = json::parse(str);
        MyData my_data = (MyData) data;
        my_data -> user_id.append(j["id_str"]);
        return realSize;
    }

    size_t detect_tweets(char *ptr, size_t size, size_t nmemb, void *data) {
        size_t realSize = size * nmemb;
        string str(ptr, realSize);
        MyData my_data = (MyData) data;
        string user_id = my_data -> user_id;
        if(realSize > 2) {
            (my_data -> data).append(str);
            if(str.find("\r\n") != string::npos) {
                json j = json::parse(my_data -> data);
                if(j["created_at"] != nullptr) {
                    string id = j["user"]["id_str"];
                    if(!id.compare(user_id))
                        (my_data -> sound).play();
                }
                (my_data -> data).clear();
            }
        }
        return realSize;
    }

    int get_request(
        size_t (*writefun)(char*, size_t, size_t, void*),
        void *buffer, char *url
    ) {
        CURL *curl;
        curl = curl_easy_init();
        if(curl) {
            CURLcode code;
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefun);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
            code = curl_easy_perform(curl);
            if(code == CURLE_HTTP_RETURNED_ERROR)
                return 0;
        }
        curl_easy_cleanup(curl);
        return 1;
    }
}
