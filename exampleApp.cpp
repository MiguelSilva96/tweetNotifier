#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <tweetNotifier.h>

using namespace std;

int main(int argc, char *argv[]) {
    char *username = (char*) malloc(512);
    const char *kConsKey = "your-consumer-key";
    const char *kConsSec = "your-consumer-secret";
    const char *kAtokKey = "your-access-token";
    const char *kAtokSec = "your-access-token-secret";

    username = strcpy(username, "realDonaldTrump");
    if(argc > 1)
        username = strcpy(username, argv[1]);
    // you can optionally have another argument with a
    // path to a sound that you wanto to be played
    // instead of "mamma mia" from super mario
    tn::TweetNotifier tweet_not(username,
        kConsKey, kConsSec, kAtokKey, kAtokSec);
    tn::TnStatus status = tweet_not.start_notifier();
    if(status == tn::Get_Id_Error)
        cout << "error getting user id" << endl;
    else if(status == tn::Stream_Connect_Error)
        cout << "error connecting to stream" << endl;
    else; // status is Ok, stream just disconnected, try to reconnect

    free(username);
    return 0;
}
