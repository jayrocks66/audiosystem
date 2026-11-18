#define MINIAUDIO_IMPLEMENTATION
#include <stdio.h>
#include <miniaudio.h>

ma_engine maEngine;
ma_sound maSound;

void playSong()
{
    if (&maSound != NULL)
    {
        ma_sound_uninit(&maSound);
    }
    ma_sound_init_from_file(&maEngine, "assets/test440.mp3", 0, NULL, NULL, &maSound);
    ma_sound_start(&maSound);
}

int main()
{

    ma_engine_init(NULL, &maEngine);

    // while (ma_sound_is_playing(&maSound)) ma_sleep(100);
    char c = 0;
    while (c != 'q')
    {
        printf("Press 'p' to play the song or 'q' to quit: ");
        c = getchar();
        if (c == 'p')
        {
            playSong();
        }
    }
    ma_engine_uninit(&maEngine);
    return 0;
}