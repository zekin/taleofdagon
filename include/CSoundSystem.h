#ifndef CSOUNDSYSTEM_H
#define CSOUNDSYSTEM_H
#include "CEventManager.h"
#include "enum.h"
#include <SDL/SDL_mixer.h>
#include <map>
#include "logging.h"

class CSoundSystem : public IEventable {
private:
  Mix_Music* intro;
  Mix_Music* town;
  Mix_Music* wandering;
  Mix_Music* desert;
  Mix_Music* forest;
  Mix_Music* combat1; 
  Mix_Music* combat2;
  Mix_Music* combat3;
  Mix_Music* final_battle;
  Mix_Music* credits;
  
  std::map<int, Mix_Chunk*> sound_library;
public:
  CSoundSystem() : intro(0), town(0), wandering(0), combat1(0), combat2(0), combat3(0), final_battle(0), credits(0) {
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    CEventManager::getInstance()->subscribe(0,this);
    intro=Mix_LoadMUS("./music/intro.ogg");
//    desert=Mix_LoadMUS("./music/desert.ogg");
    forest=Mix_LoadMUS("./music/forest.ogg");
//    combat1=Mix_LoadMUS("./music/combat1.ogg");
    if (intro==0) {
      ERROR(LOG) << "Error in loading music : " << Mix_GetError();
    }
//    if (desert==0) {
//      ERROR(LOG) << "Error in loading music : " << Mix_GetError();
//    }
    if (forest==0) {
      std::clog << "Error in loading music : " << Mix_GetError() << std::endl;
    }
//    if (combat1==0) {
//      std::clog << "Error in loading music : " << Mix_GetError() << std::endl;
//    }
    sound_library[SOUND_UI_SELECT]=Mix_LoadWAV("./sounds/select.wav");
    sound_library[SOUND_UI_CLICK]=Mix_LoadWAV("./sounds/damage.wav");
    
  }
  void play_music(std::string name, Mix_Music* music) {
    if (music==0) {
      INFO(LOG) << name << " music request recieved, not loaded; skipping play";
      return;
    }
    if (Mix_PlayMusic(music,-1)==-1) {
      INFO(LOG) << "Error occured, failed to play " << name << " music";
      return;
    }
    INFO(LOG) << "Playing " << name << " music";
  }
  virtual void notify(Event* e) {
    if (e->type==EVENT_PLAY_MUSIC) {
      switch(e->a) {
        case MUSIC_INTRO:       play_music("Intro", intro);              break;
        case MUSIC_TOWN:        play_music("Town", town);                break;
        case MUSIC_WANDERING:   play_music("Wandering", wandering);      break;
        case MUSIC_DESERT:      play_music("Desert", desert);            break;
        case MUSIC_FOREST:      play_music("Forest", forest);            break;
        case MUSIC_COMBAT1:     play_music("Combat1", combat1);          break;
        case MUSIC_COMBAT2:     play_music("Combat2", combat2);          break;
        case MUSIC_COMBAT3:     play_music("Combat3", combat3);          break;
        case MUSIC_FINALBATTLE: play_music("FinalBattle", final_battle); break;
        case MUSIC_CREDITS:     play_music("Credits", credits);          break;
        default:
          INFO(LOG) << "Incorrect music number received, not playing anything";
          break;
      }
    } else if (e->type==EVENT_PLAY_SOUND) {
      if (Mix_PlayChannel(-1, sound_library[e->a],0)==-1) {}
    }
  }
};

#endif