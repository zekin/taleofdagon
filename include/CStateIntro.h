#ifndef CSTATEINTRO_H
#define CSTATEINTRO_H
#include <SDL/SDL.h>

#include "CEventManager.h"
#include "enum.h"
#include "CMap.h"
#include "CGUI.h"
#include "CRenderable.h"
#include "CDialogManager.h"
class IState : public IEventable {
private:
    int state_id;
public:
    IState() {
    }
    virtual int get_state_id() {
        return state_id;
    }
};
class CStateIntro : public IState {
    CStateIntro() {

    }
    virtual void notify(Event* e) {
//    if (e->type == EVENT_KEYPRESS_DOWN) {
//      if (e->a=='w')
//        CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_NORTH));
//      if (e->a=='s')
//        CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_SOUTH));
//      if (e->a=='e')
//        CEventManager::getInstance()->notify(Event(EVENT_COMMAND_USE_START));
//    }
//    if (e->type == EVENT_KEYPRESS_UP) {
//      if (e->a=='e')
//        CEventManager::getInstance()->notify(Event(EVENT_COMMAND_USE_END));
////      if (e->a==SDLK_ESCAPE)
//    }
//    if (e->type == EVENT_RENDER_FRAME) {
//      CIntroCamera::getInstance()->update();
//    }
//    if (e->type == EVENT_INITIALIZE) {
//      CEventManager::getInstance()->notify(Event(EVENT_PLAY_MUSIC,MUSIC_INTRO));
//      CEventManager::getInstance()->notify(Event(EVENT_CAMERA_TARGET,(long)CIntroCamera::getInstance()));
//      CEventManager::getInstance()->subscribe(0, CMapDemo::getInstance());
//      CMapDemo::getInstance()->notify(e);
//      CEventManager::getInstance()->subscribe(0, &GUI);
//    }
//    if (e->type == EVENT_CLEANUP) {
//    }
    }
};
class CStateMainMenu : public IState {
private:
    CGUIMainMenu gui;
    CGUI* current_gui;
    CRenderable camera_pan;
public:
    CStateMainMenu() : gui(this), camera_pan(750,1400,0), current_gui(0) {
    }
    virtual void notify(Event* e) {

        //by decoupling the keypress down event from the actual enum "logic" of the event, we can make keybinds easier. theoretically, no time.
        if (e->type == EVENT_KEYPRESS_DOWN) {
            if (e->a=='w')
                CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_NORTH));
            if (e->a=='s')
                CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_SOUTH));
            if (e->a==SDLK_SPACE)
                CEventManager::getInstance()->notify(Event(EVENT_COMMAND_USE_START));
        }
        if (e->type == EVENT_KEYPRESS_UP) {
            if (e->a==SDLK_SPACE)
                CEventManager::getInstance()->notify(Event(EVENT_COMMAND_USE_END));
//      if (e->a==SDLK_ESCAPE)
        }
        if (e->type == EVENT_RENDER_FRAME) {
            CIntroCamera::getInstance()->update();
        }
        if (e->type == EVENT_INITIALIZE) {
            CEventManager::getInstance()->notify(Event(EVENT_PLAY_MUSIC,MUSIC_INTRO));
            CEventManager::getInstance()->notify(Event(EVENT_CAMERA_TARGET,(long)CIntroCamera::getInstance()));
            CEventManager::getInstance()->subscribe(0, CWorldMap::getInstance());
            CWorldMap::getInstance()->notify(e);
            CEventManager::getInstance()->subscribe(0, &gui);
        }
        if (e->type == EVENT_CLEANUP) {
            CEventManager::getInstance()->unsubscribe(0, &gui);
        }
    }
};
class CStateCharacterSelect : public IState {
private:
    CGUICharacterSelect GUISelect;
public:

    CStateCharacterSelect() : GUISelect(this) {
    }
    virtual void notify(Event* e) {
        //by decoupling the keypress down event from the actual enum "logic" of the event, we can make keybinds easier. theoretically, no time.
        if (e->type == EVENT_KEYPRESS_DOWN) {
            if (e->a=='w')
                CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_NORTH));
            if (e->a=='s')
                CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_SOUTH));
            if (e->a==SDLK_SPACE)
                CEventManager::getInstance()->notify(Event(EVENT_COMMAND_USE_START));
        }
        if (e->type == EVENT_KEYPRESS_UP) {
            if (e->a==SDLK_SPACE)
                CEventManager::getInstance()->notify(Event(EVENT_COMMAND_USE_END));
            if (e->a==SDLK_ESCAPE)
                CEventManager::getInstance()->notify(Event(EVENT_COMMAND_CANCEL));
//       if (e->a==SDLK_ESCAPE)
        }
        if (e->type == EVENT_RENDER_FRAME) {
            CIntroCamera::getInstance()->update();
        }
        if (e->type == EVENT_INITIALIZE) {
//       CEventManager::getInstance()->subscribe(0, &IntroMap);
//       Event ev = Event(EVENT_GAME_STARTED);
//       IntroMap.notify(&ev);
            CEventManager::getInstance()->subscribe(0, &GUISelect);
//       CEventManager::getInstance()->unsubscribe(0, &IntroMap);
        }
        if (e->type == EVENT_CLEANUP) {
            CEventManager::getInstance()->unsubscribe(0, &GUISelect);
//     CEventManager::getInstance()->unsubscribe(0, CWorldMap::getInstance());
        }
        if (e->type == EVENT_COMMAND_CANCEL) {
            CEventManager::getInstance()->unsubscribe(0, &GUISelect);
//     CEventManager::getInstance()->unsubscribe(0, CMapDemo::getInstance());
            CEventManager::getInstance()->notify(Event(EVENT_SWITCH_STATES,STATE_MENU));
        }
    }
};
class CStateGame : public IState {
private:
    bool enabled;
    bool game_camera_on;
//  CMap WorldMap;
    CRenderable GameCamera;
    bool directions[4];
    CDialogManager DialogManager;
    CGameConsole console;
    bool world_map_on;

    CGUIWorldMap WorldMap;
public:
    
    CStateGame() :
        enabled(false),
        GameCamera(700,1200,0),
        DialogManager(),
        WorldMap(&GameCamera),
//	directions(false}), can't do this in MSVC without latest C++ i guess
        world_map_on(false),
        console(),
        game_camera_on(false) {//, WorldMap(100,100) {
        for (int i=0; i<4; i++)
            directions[i]=false;
    }
    void update() {
        if (directions[DIRECTION_NORTH])
            GameCamera.y-=CClock::getInstance()->deltaT()*5.0;
        if (directions[DIRECTION_EAST])
            GameCamera.x+=CClock::getInstance()->deltaT()*5.0;
        if (directions[DIRECTION_WEST])
            GameCamera.x-=CClock::getInstance()->deltaT()*5.0;
        if (directions[DIRECTION_SOUTH])
            GameCamera.y+=CClock::getInstance()->deltaT()*5.0;

    }
    virtual void notify(Event* e) {
        static int day_flags=0;
        switch(e->type) {
        case EVENT_INITIALIZE: {
            CEventManager::getInstance()->notify(Event(EVENT_PLAY_MUSIC,MUSIC_FOREST));
            CEventManager::getInstance()->notify(Event(EVENT_CAMERA_TARGET,(long)CPlayerRenderable::getInstance()));
            CEventManager::getInstance()->subscribe(0, CWorldMap::getInstance());
            CLocator::setMap((IMap*)CWorldMap::getInstance());
            CWorldMap::getInstance()->notify(e);
            CEventManager::getInstance()->subscribe(0, &DialogManager);
            CEventManager::getInstance()->notify(Event(EVENT_SPAWN_DIALOG, DIALOG_1_START_DAGON));
            CEventManager::getInstance()->subscribe(0, &console);


            Event ev=Event(EVENT_CAMERA_TARGET,(long)CPlayerRenderable::getInstance());
            WorldMap.notify(&ev);
            CEventManager::getInstance()->notify(Event(EVENT_ADD_CONSOLE, "Welcome to the ~4Tale of Dagon!"));

            CEventManager::getInstance()->notify(Event(EVENT_ADD_CONSOLE, "~3W-S-A-D ~0to move."));
            CEventManager::getInstance()->notify(Event(EVENT_ADD_CONSOLE, "~3Spacebar ~0to confirm dialogues."));
            CEventManager::getInstance()->notify(Event(EVENT_ADD_CONSOLE, "~3C ~0to toggle ~2free roam ~0camera."));
            break;
        }
        case EVENT_CLEANUP:
            CEventManager::getInstance()->unsubscribe(0, &DialogManager);
            CEventManager::getInstance()->unsubscribe(0, CWorldMap::getInstance());
            break;
        case EVENT_KEYPRESS_DOWN:
            if (e->a=='w') {
                if (game_camera_on)
                    directions[DIRECTION_NORTH]=true;
                else {
                    CPlayerRenderable::getInstance()->ai_state=AI_WALKING;
                    CPlayerRenderable::getInstance()->direction=DIRECTION_NORTH;
                }
            }
//          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_NORTH));
            if (e->a=='s') {
                if (game_camera_on) {
                    directions[DIRECTION_SOUTH]=true;
                }
                else {
                    CPlayerRenderable::getInstance()->ai_state=AI_WALKING;
                    CPlayerRenderable::getInstance()->direction=DIRECTION_SOUTH;
                }
            }

//          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_SOUTH));
            if (e->a=='a') {
                if (game_camera_on) {
                    directions[DIRECTION_WEST]=true;
                } else {
                    CPlayerRenderable::getInstance()->ai_state=AI_WALKING;
                    CPlayerRenderable::getInstance()->direction=DIRECTION_WEST;
                }
            }
//          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_WEST));
            if (e->a=='d') {
                if (game_camera_on) {
                    directions[DIRECTION_EAST]=true;
                } else {
                    CPlayerRenderable::getInstance()->ai_state=AI_WALKING;
                    CPlayerRenderable::getInstance()->direction=DIRECTION_EAST;
                }
            }

//          CEventManager::getInstance()->notify(Event(EVENT_CAMERA_MOVE_START, DIRECTION_EAST));
            if (e->a=='m')
                CEventManager::getInstance()->notify(Event(EVENT_COMMAND_MAP));
            if (e->a==SDLK_c) {
                game_camera_on ^= 1;
                if (game_camera_on) {
                    Event ev=Event(EVENT_CAMERA_TARGET,(long)&GameCamera);
                    WorldMap.notify(&ev);
                    CEventManager::getInstance()->notify(Event(EVENT_CAMERA_TARGET,(long)&GameCamera));
                    CEventManager::getInstance()->notify(Event(EVENT_ADD_CONSOLE,"Switching to Free Camera mode"));
                }
                else {
                    Event ev=Event(EVENT_CAMERA_TARGET,(long)CPlayerRenderable::getInstance());
                    CEventManager::getInstance()->notify(Event(EVENT_CAMERA_TARGET,(long)CPlayerRenderable::getInstance()));
                    CEventManager::getInstance()->notify(Event(EVENT_ADD_CONSOLE,"Switching to Player"));

                    WorldMap.notify(&ev);
                }
            }
            break;
        case EVENT_KEYPRESS_UP:
            if (e->a=='w') {
                directions[DIRECTION_NORTH]=false;
                CPlayerRenderable::getInstance()->ai_state=AI_STAND_AROUND;
                CPlayerRenderable::getInstance()->direction=DIRECTION_NORTH;
            }

            if (e->a=='s') {
                directions[DIRECTION_SOUTH]=false;
                CPlayerRenderable::getInstance()->ai_state=AI_STAND_AROUND;
                CPlayerRenderable::getInstance()->direction=DIRECTION_SOUTH;

            }
            if (e->a=='a') {
                directions[DIRECTION_WEST]=false;
                CPlayerRenderable::getInstance()->ai_state=AI_STAND_AROUND;
                CPlayerRenderable::getInstance()->direction=DIRECTION_WEST;

            }
            if (e->a=='d') {
                directions[DIRECTION_EAST]=false;
                CPlayerRenderable::getInstance()->ai_state=AI_STAND_AROUND;
                CPlayerRenderable::getInstance()->direction=DIRECTION_EAST;
            }
            if (e->a=='z') {
                directions[DIRECTION_EAST]=false;
                CPlayerRenderable::getInstance()->ai_state=AI_ATTACK;
            }
//        if (e->a=='m')
//          CEventManager::getInstance()->unsubscribe(0,&WorldMap);
            break;
        case EVENT_RENDER_FRAME: {
            float t=CClock::getInstance()->time_of_day();
            if ( ((~day_flags) & 0x1) == 0x1 && 0 <= t && t < 90) {
                day_flags=1;
                CEventManager::getInstance()->notify(Event(EVENT_ADD_CONSOLE, "It's now midnight."));
            } else if ( ((~day_flags) & 0x2) == 0x2 && 90 <= t && t < 180) {
                day_flags|=0x2;
                CEventManager::getInstance()->notify(Event(EVENT_ADD_CONSOLE, "It's now early morning."));
            } else if ( ((~day_flags) & 0x4) == 0x4 && 180 <= t && t < 270) {
                day_flags|=0x4;
                CEventManager::getInstance()->notify(Event(EVENT_ADD_CONSOLE, "It's now noon."));
            } else if ( ((~day_flags) & 0x8) == 0x8 && 270 <= t && t < 360) {
                day_flags|=0x8;
                CEventManager::getInstance()->notify(Event(EVENT_ADD_CONSOLE, "It's now evening."));
            }
            update();
            break;
        }
        case EVENT_COMMAND_MAP:
            world_map_on^=1;

            if (world_map_on) {
                CEventManager::getInstance()->subscribe(0,&WorldMap);
//         CEventManager::getInstance()->notify(Event(EVENT_ADD_CONSOLE,"Opening map..."));
            }
            else {
                CEventManager::getInstance()->unsubscribe(0,&WorldMap);
//         CEventManager::getInstance()->notify(Event(EVENT_ADD_CONSOLE,"Closing map..."));
            }
            break;
        }
    }

};
#endif
