#define MINIAUDIO_IMPLEMENTATION
#include <stdio.h>
#include <stdbool.h>
#include <miniaudio.h>

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

ma_engine maEngine;
ma_sound maSound;
int globalVolume = 100;
struct Song songBeingPlayed = {0, ""};

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

void stopPlayback()
{
    if (&maSound != NULL)
    {
        ma_sound_uninit(&maSound);
    }
    songBeingPlayed = (struct Song){0, ""};
}

bool playSong(struct Song song)
{
    stopPlayback();
    ma_result isPlayable = ma_sound_init_from_file(&maEngine, song.songPath, 0, NULL, NULL, &maSound);
    if (isPlayable != MA_SUCCESS)
    {
        ma_sound_uninit(&maSound);
        return false;
    }
    ma_sound_start(&maSound);
    songBeingPlayed = song;
    return true;
}

float getPlaybackCursor()
{
    if (songBeingPlayed.id != 0 && ma_sound_is_playing(&maSound))
    {
        float cursorSeconds;
        ma_sound_get_cursor_in_seconds(&maSound, &cursorSeconds);
        return cursorSeconds;
    }
    return -1.0f; // No song is being played
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
    printf("Press:\n'p' to play the test tone\n'q' to quit:\n't' to check the playback cursor (do it FAST!)\nfollowed by ENTER\n");
    while (c != 'q')
    {
        c = getchar();
        if (c == 'p')
        {
            if (checkSongExists(testSong))
            {
                playSong(testSong);
                printf("Playback cursor: %.2f seconds\n", getPlaybackCursor());
            }
        }
        if (c == 't')
        {
            printf("Playback cursor: %.2f seconds\n", getPlaybackCursor());
        }
    }
    ma_engine_uninit(&maEngine);
    return 0;
}