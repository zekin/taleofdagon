#include <SDL/SDL.h>
#include "CGame.h"
#include "logging.h"


int main(int argc, char **argv) {
  CGame game;

  /* check for additional arguments */
  if (argc>1) {
    if (argc==2 && strcmp(argv[1],"map")==0) {
      INFO(LOG) << "Map parameter caught. Generating map.";
      Globals::generateWorldImage();
      SDL_Quit();
      exit(0);
    }
    
    if (argc==2 && strcmp(argv[1],"fullscreen")==0)
      game.initialize(SDL_FULLSCREEN);
    else {
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
