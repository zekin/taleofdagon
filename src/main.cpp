#include <SDL/SDL.h>
#include "CGame.h"
#include "logging.h"
#include "CMapGenerator.h"

int main(int argc, char **argv) {
    CGame game;
    /* check for additional arguments */
    if (argc>1) {
        if (argc==2 && strcmp(argv[1],"map")==0) {
            INFO(LOG) << "Map parameter caught. Generating map.";
            CWorldMapGenerator mapGenerator;
            mapGenerator.generateWorldImage();
            SDL_Quit();
            exit(0);
        }

        if (argc==2 && strcmp(argv[1],"fullscreen")==0)
            game.initialize(GAMEFLAGS_FULLSCREEN);
        else if (argc==3 && strcmp(argv[1],"debug")==0) {
            game.initialize(GAMEFLAGS_DEBUG, argv[2]);
        }
        else if (argc==4 && strcmp(argv[1],"map")==0 && strcmp(argv[2],"debug")==0) {
            CDebugMapGenerator mapGenerator(argv[3]);
            mapGenerator.generateWorldImage();
            SDL_Quit();
            exit(0);
        }
        else{
            INFO(LOG) << "Unknown arguments, pass map or fullscreen only. Launching default game.";
            game.initialize(0);
        }
    } else {
        game.initialize(0);
    }
    
    /* pointers will be signed integers and we will grow oranges in alaska */

    while(game.events()) {
        game.update();
    }
    return 0;
}
