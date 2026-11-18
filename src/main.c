#define MINIAUDIO_IMPLEMENTATION
#include <stdio.h>
#include <miniaudio.h>

ma_engine maEngine;
ma_sound maSound;
int globalVolume = 100;

struct Playlist
{
    int id;
    char songPath[256];
    int songsSequence[256];
};

struct Song
{
    int id;
    char songPath[256];
};

void playSong(char songPath[])
{
    if (&maSound != NULL)
    {
        ma_sound_uninit(&maSound);
    }
    ma_sound_init_from_file(&maEngine, songPath, 0, NULL, NULL, &maSound);
    ma_sound_start(&maSound);
}

void playPlaylist(struct Playlist playlist)
{
    // TODO! THIS IS A PLACEHOLDER
    for (int i = 0; i < sizeof(playlist.songsSequence) / sizeof(playlist.songsSequence[0]); i++)
    {
        int songId = playlist.songsSequence[i];
        if (songId == -1)
        {
            break; // End of playlist
        }
        char songPath[256];
        sprintf(songPath, "assets/song%d.mp3", songId);
        playSong(songPath);
        while (ma_sound_is_playing(&maSound))
        {
            ma_sleep(100); // Wait for the song to finish
        }
    }
}

bool setVolume(int volume)
{
    if (volume < 0 || volume > 100)
    {
        return false; // Invalid volume range
    }
    globalVolume = volume;
    ma_engine_set_volume(&maEngine, volume / 100.0f);
    return true;
}

int main()
{

    ma_engine_init(NULL, &maEngine);
    setVolume(90);
    // while (ma_sound_is_playing(&maSound)) ma_sleep(100);
    char c = 0;
    while (c != 'q')
    {
        printf("Press 'p' to play the song or 'q' to quit: ");
        c = getchar();
        if (c == 'p')
        {
            playSong("assets/test440.mp3");
        }
    }
    ma_engine_uninit(&maEngine);
    return 0;
}