#ifndef IRESOURCE_H
#define IRESOURCE_H

#include <string>
#include <iostream>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include "enum.h"
#include <iomanip>
#include <vector>
#include <map>
#include "logging.h"

//was planning on using this for all the resources, not sure to what benefit it is now
class IResource {
protected:
    int type;
    std::string path;
public:
    IResource() {}
    IResource(std::string path) : path(path) {}
    virtual ~IResource() {}
    virtual void load(std::string path) {}
};

//takes in an image and outputs a sprite sheet, you can grab at frames linearly with it and get sprites from the resource manager
class CSpriteSheet : public IResource {
protected:
    SDL_Surface* surface;
    unsigned int* sheet_texture_id;
    int cell_width;
    int cell_height;
    bool linear;
public:
    CSpriteSheet(std::string path, bool linear=false) : IResource(path), linear(linear) {
        load(true);
        loadTexture();
    }
    CSpriteSheet(std::string path, int cell_width, int cell_height, bool linear=false) : linear(linear), IResource(path), cell_width(cell_width), cell_height(cell_height), surface(0)
    {
        load(false);
        loadTexture();
    }
    virtual ~CSpriteSheet() {
        delete[] sheet_texture_id;
        if (surface)
            delete surface;
    }
    std::string getPath() {
        return path;
    }
    virtual void load(bool use_full_image) {
        surface=IMG_Load(path.c_str());
        if (surface) {
            INFO(LOG) << "Loaded " << path;
        } else {
            ERROR(LOG) << "Error: could not load image at path " << path;
        }
        if (!use_full_image) {
            if ((surface->w/cell_width*surface->h/cell_height)>0) {
                sheet_texture_id=new unsigned int[surface->w/cell_width*surface->h/cell_height];
            }
        } else {
            sheet_texture_id=new unsigned int[1];
            cell_width=surface->w;
            cell_height=surface->h;
        }
    }
    SDL_Rect getFrameSDL(int i) {
        if (cell_width > surface->w) {
            ERROR(LOG) << "Error: Width of frame is bigger than cell width, using image dimensions";
            cell_width=surface->w;
        }
        if (cell_width < 1) {
            ERROR(LOG) << "Error: Width of frame is less than 1, setting to 1.";
            cell_width=1;
        }
        if (cell_height > surface->h) {
            ERROR(LOG) << "Error: Height of frame is bigger than cell height, using image dimensions";
            cell_height=surface->h;
        }
        if (cell_height < 1) {
            ERROR(LOG) << "Error: Height of frame is less than 1, setting to 1.";
            cell_height=1;
        }
        if (i<0) {
            ERROR(LOG) << "Error: Frame less than 0 passed into frame animation";
        }
        int num_cells_wide=surface->w/cell_width;
        int num_cells_high=surface->h/cell_width;
        int x1=(int)((i%num_cells_wide)*cell_width);
        int y1=(int)((i/num_cells_wide)*cell_height);
        int x2=(int)((i%num_cells_wide+1)*cell_width);
        int y2=(int)((i/num_cells_wide+1)*cell_height);

        SDL_Rect r = {x1,y1,cell_width,cell_height};
        return r;
    }
    Rect getFrame(int i) {
        if (cell_width > surface->w) {
            ERROR(LOG) << "Error: Width of frame is bigger than cell width, using image dimensions";
            cell_width=surface->w;
        }
        if (cell_width < 1) {
            ERROR(LOG) << "Error: Width of frame is less than 1, setting to 1.";
            cell_width=1;
        }
        if (cell_height > surface->h) {
            ERROR(LOG) << "Error: Height of frame is bigger than cell height, using image dimensions";
            cell_height=surface->h;
        }
        if (cell_height < 1) {
            ERROR(LOG) << "Error: Height of frame is less than 1, setting to 1.";
            cell_height=1;
        }
        if (i<0) {
            ERROR(LOG) << "Error: Frame less than 0 passed into frame animation";
        }
        int num_cells_wide=surface->w/cell_width;
        int num_cells_high=surface->h/cell_width;
        double x1=(double)((i%num_cells_wide)*cell_width)/(double)(num_cells_wide*cell_width);
        double y1=(double)((i/num_cells_wide)*cell_height)/(double)(num_cells_high*cell_height);
        double x2=(double)((i%num_cells_wide+1)*cell_width)/(double)(num_cells_wide*cell_width);//*cell_width)/(WIDTH_CELLS*cell_width);
        double y2=(double)((i/num_cells_wide+1)*cell_height)/(double)(num_cells_high*cell_height);

        return Rect(XY(x1,y1),XY(x2,y1),XY(x1,y2),XY(x2,y2));
    }

    virtual SDL_Surface* getResource() {
        return surface;
    };
    void loadTexture() {
        const int max_frames=(surface->w/cell_width) * (surface->h/cell_height);
        INFO(LOG) << "Number of textures to load " << max_frames;
        if (max_frames<0) {
            ERROR(LOG) << "Error: Trying to allocate a negative number of textures?";
        } else if (max_frames>1000) {
            ERROR(LOG) << "Error: Why are you trying to allocate so many textures? (> 1000)";
        }
        glGenTextures(max_frames,sheet_texture_id);
        for (int i=0; i<max_frames; i++) {
            glBindTexture(GL_TEXTURE_2D, sheet_texture_id[i]);
            if (linear) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            } else {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
            SDL_Surface* temp_surface=SDL_CreateRGBSurface(0,cell_width,cell_height,32,0xFF,0xFF00,0xFF0000,0xFF000000);
            SDL_SetAlpha(surface,0,SDL_SRCALPHA);
            SDL_Rect r1=getFrameSDL(i);

//      INFO(LOG) << "Texture Copied: " << r1.w << ", " << r1.h << " from " << "("  << r1.x << ", " << r1.y << ")";
//      SDL_FillRect(temp_surface,&r1, 0xff0000ff);

            SDL_BlitSurface(surface, &r1, temp_surface, 0);
//      glGenerateMipmap(GL_TEXTURE_2D);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp_surface->w, temp_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp_surface->pixels);
//      gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,temp_surface->w,temp_surface->h,GL_RGBA,GL_UNSIGNED_BYTE,temp_surface->pixels);
//      delete temp_surface;

            SDL_FreeSurface(temp_surface);
        }
    }
    unsigned int getTextureID(int num) {
        return sheet_texture_id[num];
    }

};


// needs refactoring, we can merge this with CSpriteSheet

class CMapSheet : public IResource {
protected:
    SDL_Surface* surface;
    unsigned int* sheet_texture_id;
    int cell_width;
    int cell_height;
public:
    CMapSheet() {}
    CMapSheet(std::string path, int cell_width, int cell_height) : IResource(path), cell_width(cell_width), cell_height(cell_height), surface(0)
    {
        load();
        loadTexture();
    }
    virtual ~CMapSheet() {
        delete[] sheet_texture_id;
        if (surface)
            delete surface;
    }
    virtual void load() {
        surface=IMG_Load(path.c_str());
        if (surface==0)
            ERROR(LOG) << "Error: could not load image at path " << path;
        else
            INFO(LOG) << "Loaded " << path;
        if ((surface->w/cell_width*surface->h/cell_height)>0)
            sheet_texture_id=new unsigned int[surface->w/cell_width * surface->h/cell_height];
    }
    std::string getPath() {
        return path;
    }
    SDL_Rect getFrameSDL(int i) {
        if (cell_width > surface->w) {
            ERROR(LOG) << "Error: Width of frame is bigger than cell width, using image dimensions";
            cell_width=surface->w;
        }
        if (cell_width < 1) {
            ERROR(LOG) << "Error: Width of frame is less than 1, setting to 1.";
            cell_width=1;
        }
        if (cell_height > surface->h) {
            ERROR(LOG) << "Error: Height of frame is bigger than cell height, using image dimensions";
            cell_height=surface->h;
        }
        if (cell_height < 1) {
            ERROR(LOG) << "Error: Height of frame is less than 1, setting to 1.";
            cell_height=1;
        }
        if (i<0) {
            ERROR(LOG) << "Error: Frame less than 0 passed into frame animation";
        }
        int WIDTH_CELLS=surface->w/cell_width;
        int HEIGHT_CELLS=surface->h/cell_width;
        int x1=(int)((i%WIDTH_CELLS)*cell_width);
        int y1=(int)((i/WIDTH_CELLS)*cell_height);
        int x2=(int)((i%WIDTH_CELLS+1)*cell_width);
        int y2=(int)((i/WIDTH_CELLS+1)*cell_height);

        SDL_Rect r = {x1,y1,cell_width,cell_height};
        return r;
    }  Rect getFrame(int i) {
        if (cell_width > surface->w) {
            ERROR(LOG) << "Error: Width of frame is bigger than cell width, using image dimensions";
            cell_width=surface->w;
        }
        if (cell_width < 1) {
            ERROR(LOG) << "Error: Width of frame is less than 1, setting to 1.";
            cell_width=1;
        }
        if (cell_height > surface->h) {
            ERROR(LOG) << "Error: Height of frame is bigger than cell height, using image dimensions";
            cell_height=surface->h;
        }
        if (cell_height < 1) {
            ERROR(LOG) << "Error: Height of frame is less than 1, setting to 1.";
            cell_height=1;
        }
        if (i<0) {
            ERROR(LOG) << "Error: Frame less than 0 passed into frame animation";
        }
        int num_cells_wide=surface->w/cell_width;
        int num_cells_high=surface->h/cell_width;
        double x1=(double)((i%num_cells_wide)*cell_width)/(double)(num_cells_wide*cell_width);
        double y1=(double)((i/num_cells_wide)*cell_height)/(double)(num_cells_high*cell_height);
        double x2=(double)((i%num_cells_wide+1)*cell_width)/(double)(num_cells_wide*cell_width);//*cell_width)/(WIDTH_CELLS*cell_width);
        double y2=(double)((i/num_cells_wide+1)*cell_height)/(double)(num_cells_high*cell_height);

        return Rect(XY(x1,y1),XY(x2,y1),XY(x1,y2),XY(x2,y2));
    }

    virtual SDL_Surface* getResource() {
        return surface;
    };
    void loadTexture() {
        const int max_frames=(surface->w/cell_width) * (surface->h/cell_height);
        INFO(LOG) << "Number of textures to load " << max_frames;
        if (max_frames<0) {
            ERROR(LOG) << "Error: Trying to allocate a negative number of textures?";
        } else if (max_frames>1000) {
            ERROR(LOG) << "Error: Why are you trying to allocate so many textures? (> 1000)";
        }
        SDL_Surface* s=IMG_Load("./graphics/mask.png");
        glGenTextures(max_frames,sheet_texture_id);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
        for (int i=0; i<max_frames; i++) {
            glBindTexture(GL_TEXTURE_2D, sheet_texture_id[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            SDL_Surface* temp_surface=SDL_CreateRGBSurface(0,cell_width*2,cell_height*2,32,s->format->Rmask,s->format->Gmask,s->format->Bmask,s->format->Amask);
            for (int j=0; j<cell_width*cell_height*4; j++) {
                ((unsigned int*)temp_surface->pixels)[j] = ((unsigned int*)s->pixels)[j]&0xFF000000;
            }
            SDL_Rect r1=getFrameSDL(i);
            SDL_Rect r2= {0,0,cell_width,cell_height};
            SDL_BlitSurface(surface, &r1, temp_surface, &r2);
            r2.x+=cell_width;
            SDL_BlitSurface(surface, &r1, temp_surface, &r2);
            r2.y+=cell_height;
            SDL_BlitSurface(surface, &r1, temp_surface, &r2);
            r2.x-=cell_width;
            SDL_BlitSurface(surface, &r1, temp_surface, &r2);
//      for (int i=0; i<cell_width*cell_height*4; i++)
            gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,temp_surface->w,temp_surface->h,GL_RGBA,GL_UNSIGNED_BYTE,temp_surface->pixels);
//      gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,s->w,s->h,GL_RGBA,GL_UNSIGNED_BYTE,s->pixels);
//      delete temp_surface;
            SDL_FreeSurface(temp_surface);
        }
    }
    unsigned int getTextureID(int id) {
        return sheet_texture_id[id];
    }

};

struct FrameList {
private:
    float timing;
    std::vector<int> frames;
public:
    FrameList(float timing) : timing(timing) {}
    void addFrame(int frame) {
        frames.push_back(frame);
    }
    int getFrame(float time) {
        float time_left=fmod(time,timing);
        int total_frames=frames.size();

        return frames[total_frames*(time_left/timing)];
    }

};
class CResourceManager {
private:
    std::map<std::string, FrameList*> animation_list;
    std::vector<CSpriteSheet*> sprite_sheets;
    std::vector<CMapSheet*> map_sheets;
public:
    CResourceManager() {
        /* decide the logic for the frame animations would go better here */
        animation_list["IDLE_NORTH"]=new FrameList(1.0);
        animation_list["IDLE_NORTH"]->addFrame(9);
        animation_list["IDLE_EAST"]=new FrameList(1.0);
        animation_list["IDLE_EAST"]->addFrame(6);
        animation_list["IDLE_WEST"]=new FrameList(1.0);
        animation_list["IDLE_WEST"]->addFrame(3);
        animation_list["IDLE_SOUTH"]=new FrameList(1.0);
        animation_list["IDLE_SOUTH"]->addFrame(0);
        animation_list["WALK_NORTH"]=new FrameList(1.0);
        animation_list["WALK_NORTH"]->addFrame(9);
        animation_list["WALK_NORTH"]->addFrame(10);
        animation_list["WALK_NORTH"]->addFrame(9);
        animation_list["WALK_NORTH"]->addFrame(11);
        animation_list["WALK_EAST"]=new FrameList(1.0);
        animation_list["WALK_EAST"]->addFrame(6);
        animation_list["WALK_EAST"]->addFrame(7);
        animation_list["WALK_EAST"]->addFrame(6);
        animation_list["WALK_EAST"]->addFrame(8);
        animation_list["WALK_WEST"]=new FrameList(1.0);
        animation_list["WALK_WEST"]->addFrame(3);
        animation_list["WALK_WEST"]->addFrame(4);
        animation_list["WALK_WEST"]->addFrame(3);
        animation_list["WALK_WEST"]->addFrame(5);
        animation_list["WALK_SOUTH"]=new FrameList(1.0);
        animation_list["WALK_SOUTH"]->addFrame(0);
        animation_list["WALK_SOUTH"]->addFrame(1);
        animation_list["WALK_SOUTH"]->addFrame(0);
        animation_list["WALK_SOUTH"]->addFrame(2);
        animation_list["ATTACK_NORTH"]=new FrameList(1.0);
        animation_list["ATTACK_NORTH"]->addFrame(10);
        animation_list["ATTACK_EAST"]=new FrameList(1.0);
        animation_list["ATTACK_EAST"]->addFrame(7);
        animation_list["ATTACK_WEST"]=new FrameList(1.0);
        animation_list["ATTACK_WEST"]->addFrame(4);
        animation_list["ATTACK_SOUTH"]=new FrameList(1.0);
        animation_list["ATTACK_SOUTH"]->addFrame(1);

    }
    //This function attempts to get a resource, if it fails it attempts to load it with the properties specified.
    FrameList* getFrameList(std::string name) {
        if (animation_list[name])
            return animation_list[name];
        else
            return 0;
    }
    CSpriteSheet* getSpriteSheet(std::string path, bool linear=false) {
        for (int i=0; i<sprite_sheets.size(); i++) {
            if (sprite_sheets[i]->getPath() == path) {
                return sprite_sheets[i];
            }
        }
        CSpriteSheet* new_sheet=new CSpriteSheet(path, linear);
        sprite_sheets.push_back(new_sheet);
        return new_sheet;

    }
    CSpriteSheet* getSpriteSheet(std::string path, int cell_width, int cell_height, bool linear=false) {
        for (int i=0; i<sprite_sheets.size(); i++) {
            if (sprite_sheets[i]->getPath() == path) {
                return sprite_sheets[i];
            }
        }
        CSpriteSheet* new_sheet=new CSpriteSheet(path,cell_width,cell_height, linear);
        sprite_sheets.push_back(new_sheet);
        return new_sheet;
    }
    CMapSheet* getMapSheet(std::string path, int cell_width, int cell_height) {
        for (int i=0; i<map_sheets.size(); i++) {
            if (map_sheets[i]->getPath() == path) {
                return map_sheets[i];
            }
        }
        CMapSheet* new_sheet=new CMapSheet(path,cell_width,cell_height);
        map_sheets.push_back(new_sheet);
        return new_sheet;
    }
//    CMapSheet* new_sheet=new CMapSheet(path);

    static CResourceManager* getInstance() {
        static CResourceManager* instance=0;
        if (instance==0) {
            instance=new CResourceManager();
        }
        return instance;
    }
};
class CFontOutputManager {
private:
    CSpriteSheet* font_texture;
    int frameNumberForFontCharacter(int char_value) {
        if (0x10 <= char_value && char_value < 0x80) {
            return char_value-0x10;
        } else {
            return 0x10;
        }
    }
    void setColor(char number) {
        switch(number) {
        case '0':
            glColor3f(1.0,1.0,1.0);
            break;
        case '1':
            glColor3f(0.0,0.0,0.0);
            break;
        case '2':
            glColor3f(1.0,0.0,0.0);
            break;
        case '3':
            glColor3f(0.0,1.0,0.0);
            break;
        case '4':
            glColor3f(0.0,0.0,1.0);
            break;
        case '5':
            glColor3f(1.0,1.0,0.0);
            break;
        case '6':
            glColor3f(0.0,1.0,1.0);
            break;
        case '7':
            glColor3f(1.0,0.0,1.0);
            break;
        case '8':
            glColor3f(0.0,0.0,0.0);
            break;
        case '9':
            glColor3f(0.0,0.0,0.0);
            break;
        default:
            break;
        }
    }
public:
    CFontOutputManager() {
        font_texture=CResourceManager::getInstance()->getSpriteSheet("./graphics/ui/ui_font.png",7,14);
    }

    void write_text(char* string, float x, float y, float size) {
        int i=0;
        if (!string)
            return;

        for (char* string_iterator=string, i=0; *string_iterator!=0; string_iterator++, i++) {
            if (*string_iterator=='~') {
                if ('0' <= *(string_iterator+1) && *(string_iterator+1) <= '9') {
                    string_iterator++;
                    i--;
                    setColor(*(string_iterator));
                    continue;
                }
            }
            glBindTexture(GL_TEXTURE_2D, font_texture->getTextureID(frameNumberForFontCharacter(*string_iterator)));
            glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex3f(i*size+x,y,0);
            glTexCoord2f(1,0);
            glVertex3f(i*size+x+size,y,0);
            glTexCoord2f(1,1);
            glVertex3f(i*size+x+size,y+size*2,0);
            glTexCoord2f(0,1);
            glVertex3f(i*size+x,y+size*2,0);
            glEnd();
        }
        glColor3f(1.0,1.0,1.0);
    }
    void write_text(char* string, float x, float y, float size, float width) {
        int i=0;
        int j=0;
        if (!string)
            return;

        for (char* string_iterator=string, i=0; *string_iterator!=0; string_iterator++, i++) {
            if (*string_iterator=='~') {
                if ('0' <= *(string_iterator+1) && *(string_iterator+1) <= '9') {
                    string_iterator++;
                    i--;
                    setColor(*(string_iterator));
                    continue;
                }
            }
            if ( (i+1)*size > width) {
                j++;
                i=0;
            }
            glBindTexture(GL_TEXTURE_2D, font_texture->getTextureID(frameNumberForFontCharacter(*string_iterator)));
            glBegin(GL_QUADS);
            glTexCoord2f(0,0);
            glVertex3f(i*size+x,y+j*size*2,0);
            glTexCoord2f(1,0);
            glVertex3f(i*size+x+size,y+j*size*2,0);
            glTexCoord2f(1,1);
            glVertex3f(i*size+x+size,y+(j+1)*size*2,0);
            glTexCoord2f(0,1);
            glVertex3f(i*size+x,y+(j+1)*size*2,0);
            glEnd();
        }
        glColor3f(1.0,1.0,1.0);
    }

    static CFontOutputManager* getInstance() {
        static CFontOutputManager* instance=0;
        if (instance==0) {
            instance=new CFontOutputManager();
        }
        return instance;
    }
};
#endif
//    gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,surface->w,surface->h,GL_RGB,GL_UNSIGNED_BYTE,surface->pixels);

//    SDL_Surface* mask=IMG_Load("./graphics/mask.png");
//    if (mask==NULL) {
//      std::clog << "Error: could not load mask" << std::endl;
//    } else {
//      std::clog << "Created new mask surface" << std::endl;
//    }
//    SDL_Surface* temp_surface=SDL_CreateRGBSurface(0,surface->w*2,surface->h*2,32,0xFF,0xFF00,0xFF0000,0xFF000000);
//    if (temp_surface==NULL) {
//      std::clog << "Error: Could not create a new surface." << std::endl;
//    } else {
//      std::clog << "Created new surface" << std::endl;
//    }
//
//    for (int i=0; i<surface->h/cell_height; i++) {
//      for (int j=0; j<surface->w/cell_width; j++) {
//        unsigned int* ptr=(unsigned int*)temp_surface->pixels;
//        SDL_Rect rect1 = {cell_width*j,cell_height*i,cell_width,cell_height};
//        SDL_Rect rect2 = {(cell_width*2)*j,(cell_height*2)*i,cell_width,cell_height};
//        SDL_Rect rect3 = {0,0,64,64};
//        SDL_BlitSurface(surface,&rect1,mask,&rect3);
//        SDL_BlitSurface(surface,&rect1,temp_surface,&rect2);
//        rect2.x+=cell_width;
//        SDL_BlitSurface(surface,&rect1,temp_surface,&rect2);
//        rect2.x-=cell_width;
//        rect2.y+=cell_height;
//        SDL_BlitSurface(surface,&rect1,temp_surface,&rect2);
//        rect2.x+=cell_height;
//        SDL_BlitSurface(surface,&rect1,temp_surface,&rect2);
//        for(int k=0;k<32;k++)
//          for (int l=0;k<32;k++)
//            ptr[k+l*temp_surface->w]=ptr[k+l*temp_surface->w] | 0x7f000000;
//      }
//    }

