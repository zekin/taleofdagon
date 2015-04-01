#ifndef CDIALOGMANAGER
#define CDIALOGMANAGER
#include "CGUI.h"
class CDialogue {
private:
  CDialogue* next_dialogue;
  int next_dialogue_num;
  int portrait_type;
  char* lines;
public:
  CDialogue(int portrait, char* lines, int next_dialogue_number) : 
    portrait_type(portrait), 
    lines(lines), 
    next_dialogue(0), 
    next_dialogue_num(next_dialogue_number) {
  }
  void attach(CDialogue* next) {
    next_dialogue=next;
  }
  int next_dialogue_number() {
    return next_dialogue_num;
  }
  int portrait() {
    return portrait_type;
  }
  char* line() {
    return lines;
  }
  CDialogue* next() {
    return next_dialogue;
  }
  virtual void execute() {
    
  };
};
class CDialogManager : public IEventable {
private:
  std::vector<CDialogue*> dialogues;
  CDialogue* current_dialogue;
  CLabel dialogue_box;
  CLabel portrait_dagon;
  CLabel portrait_whiskers;
public:
  void initialize() {
    dialogues.push_back(new CDialogue(PORTRAIT_DAGON,     dialogue[DIALOG_1_START_DAGON],    DIALOG_2_START_WHISKERS));
    dialogues.push_back(new CDialogue(PORTRAIT_WHISKERS,  dialogue[DIALOG_2_START_WHISKERS], DIALOG_3_START_DAGON));
    dialogues.push_back(new CDialogue(PORTRAIT_DAGON,     dialogue[DIALOG_3_START_DAGON],    DIALOG_4_START_WHISKERS));
    dialogues.push_back(new CDialogue(PORTRAIT_WHISKERS,  dialogue[DIALOG_4_START_WHISKERS], DIALOG_5_START_DAGON));
    dialogues.push_back(new CDialogue(PORTRAIT_DAGON,     dialogue[DIALOG_5_START_DAGON],    -1));   
    
    for (int i=0; i<dialogues.size(); i++) {
      if (dialogues[i]->next_dialogue_number() != -1) {
        dialogues[i]->attach(dialogues[ dialogues[i]->next_dialogue_number() ]);
      }
    }
  }
  CDialogManager() : 
  current_dialogue(0), 
  dialogue_box(-0.10,0.6,1.2,0.4,"./graphics/ui/ui_dialogue_box.png"),
  portrait_dagon(0.0,0.7,0.2,0.2,"./graphics/portrait/dagon.png"),
  portrait_whiskers(0.0,0.7,0.2,0.2,"./graphics/portrait/whiskers.png") {
    initialize();
  }
  void renderPortrait(int type) {
    switch(type) {
      case PORTRAIT_DAGON: portrait_dagon.render(); break;
      case PORTRAIT_WHISKERS: portrait_whiskers.render(); break;
    }
  }
  virtual void notify(Event* e) {
    if (e->type==EVENT_SPAWN_DIALOG) {
      current_dialogue=dialogues[e->a];
    }
    if (e->type==EVENT_RENDER_FRAME && current_dialogue) {
      CCamera::getInstance()->startGUI();     
      dialogue_box.render();
      renderPortrait(current_dialogue->portrait());
      CFontOutputManager::getInstance()->write_text(current_dialogue->line(),0.225,0.705,0.015,0.750);
      CCamera::getInstance()->endGUI();     
    }
    
    if (e->type==EVENT_KEYPRESS_DOWN) {
      if (e->a==SDLK_SPACE) {
        if (current_dialogue==0) {
          return;
        } else if (current_dialogue->next() == 0) {
          current_dialogue=0;
        } else if (current_dialogue->next() != 0) {
          current_dialogue=current_dialogue->next();
        }
      }
    }
  }
};

#endif