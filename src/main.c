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

bool checkSongExists(struct Song song)
{
    FILE *file = fopen(song.songPath, "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

bool playSong(struct Song song)
{
    if (&maSound != NULL)
    {
        ma_sound_uninit(&maSound);
    }
    ma_result isPlayable = ma_sound_init_from_file(&maEngine, song.songPath, 0, NULL, NULL, &maSound);
    if (isPlayable != MA_SUCCESS)
    {
        ma_sound_uninit(&maSound);
        return false;
    }
    ma_sound_start(&maSound);
    return true;
}

/*void playPlaylist(struct Playlist playlist)
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
        struct Song song = {songId, songPath};
        playSong(song);
        while (ma_sound_is_playing(&maSound))
        {
            ma_sleep(100); // Wait for the song to finish
        }
    }
}*/

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
    struct Song testSong = {1, "assets/test440.mp3"};
    // while (ma_sound_is_playing(&maSound)) ma_sleep(100);
    char c = 0;
    while (c != 'q')
    {
        printf("Press 'p' to play the song or 'q' to quit: ");
        c = getchar();
        if (c == 'p')
        {
            if (checkSongExists(testSong))
            {
                playSong(testSong);
            }
        }
    }
    ma_engine_uninit(&maEngine);
    return 0;
}