#ifndef CDIALOG
#define CDIALOG
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

#endif