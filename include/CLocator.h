#ifndef ILOCATE_H
#define ILOCATE_H

//#include "IUnitFactory.h"
#include "logging.h"

class IUnitFactory;
class IMap;
class IRenderer;

static IUnitFactory* g_unitFactory=0;
static IMap* g_map=0;
static IRenderer* g_renderer=0;

class CLocator {
public:
    static IUnitFactory* getUnitFactory() {
        if ( g_unitFactory == NULL ) {
            ERROR(LOG) << "Unit factory is not set! Exiting..";
            return NULL;
        }
        
        return (IUnitFactory*) g_unitFactory;
    }

    static void setUnitFactory(IUnitFactory* factory) {
        if ( !factory ) {
            INFO(LOG) << "Factory passed in is NULL.";
            return;
        }

        if ( g_unitFactory ) {
            WARN(LOG) << "Factory is already set to a value other than NULL. Replacing with new factory. Is this intentional?";
        }

        INFO(LOG) << "Setting factory to " << factory;
        
        g_unitFactory = factory;
    }
    static IMap* getMap() {
        if ( g_map == NULL ) {
            ERROR(LOG) << "Map is not set yet! Exiting...";
        }
        return g_map;
    }
    static void setMap(IMap* map) {
        if ( ! map ) {
            ERROR(LOG) << "Map passed to function is not set!";
            return;
        }

        if ( g_map ) {
            WARN(LOG) << "g_map is already set! Replacing g_map! with passed in pointer. Is this intentional?";
        }

        g_map = map;
    }
    static IRenderer* getRenderer() {
        if ( ! g_renderer ) {
            ERROR(LOG) << "Renderer is not yet set! Exiting...";
        }
        
        return g_renderer;
    }
    static void setRenderer(IRenderer* renderer) {
        if ( ! renderer ) {
            ERROR(LOG) << "Renderer passed to function is not set!";
            return;
        }
        
        if ( g_renderer ) {
            WARN(LOG) << "Renderer is already set! Replacing g_renderer with passed in pointer. Is this intentional?";
        }
        
        g_renderer = renderer;
    }
};

#endif
