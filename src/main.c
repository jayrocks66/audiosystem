#define MINIAUDIO_IMPLEMENTATION
#include <stdio.h>
#include <stdbool.h>
#include <miniaudio.h>

struct Playlist
{
    int id;
    int songsSequence[256];
};

struct Song
{
    int id;
    char songPath[256];
    float durationSeconds;
    float playbackCursorSeconds;
};

struct TimeSlot
{
    time_t startRange;
    time_t endRange;
    int startTimeOfDay; // Minutes since midnight
    int endTimeOfDay;   // Minutes since midnight
    int daysOfWeek[7];  // 0 = Sunday, 1 = Monday, ..., 6 = Saturday
    struct Playlist playlist;
};

void checkTimeSlotOverlap()
{
    // This function is a placeholder for checking time slot overlaps.
    // Implement the logic to check for overlapping time slots based on the start and end ranges, time of day, and days of the week.
    // AI Comment!!!
}

ma_engine maEngine;
ma_sound maSound;
int globalVolume = 100;
struct Song songBeingPlayed = {0, "", 0.0f, 0.0f};
struct Playlist playlistBeingPlayed = {0, {0}};

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

float getSongDuration(struct Song song)
{
    // AI wrote this - let's check it further on.

    ma_decoder decoder;

    // 1. Open the file from the path
    if (ma_decoder_init_file(song.songPath, NULL, &decoder) != MA_SUCCESS)
    {
        return -1.0f; // Error opening file
    }

    // 2. Get total PCM frames
    ma_uint64 lengthInFrames = 0;
    ma_result result = ma_decoder_get_length_in_pcm_frames(&decoder, &lengthInFrames);

    // 3. Convert frames to seconds
    float seconds = -1.0f;
    if (result == MA_SUCCESS && decoder.outputSampleRate > 0)
    {
        seconds = (float)lengthInFrames / (float)decoder.outputSampleRate;
    }

    // 4. Clean up
    ma_decoder_uninit(&decoder);

    return seconds;
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

void testLoopWithKeys()
{
    // DEFINE TEST SONGS
    struct Song testSong = {1, "assets/test440.mp3", 0.0f, 0.0f};
    struct Song testSong2 = {2, "assets/test1000.mp3", 0.0f, 0.0f};
    testSong.durationSeconds = getSongDuration(testSong);
    testSong2.durationSeconds = getSongDuration(testSong2);

    char c = 0;
    while (c != 'q')
    {
        printf("\nPress:\n'p' to play the test tone\n'q' to quit:\nfollowed by ENTER\n");
        c = getchar();
        while (getchar() != '\n')
            ; // EMPTY THE INPUT BUFFER
        if (c == 'p')
        {
            if (checkSongExists(testSong))
            {
                printf("Playing song: %s - length: %.2f seconds\n", testSong.songPath, testSong.durationSeconds);
                playSong(testSong);
                while (ma_sound_is_playing(&maSound))
                {
                    printf("\rPlayback cursor: %.2f seconds", getPlaybackCursor());
                    ma_sleep(100);
                }
                printf("\nDone!\n");
            }
        }
    }
}

int main()
{
    ma_engine_init(NULL, &maEngine);
    setVolume(90);

    testLoopWithKeys();

    ma_engine_uninit(&maEngine);
    return 0;
}