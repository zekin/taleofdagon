#ifndef ILOCATE_H
#define ILOCATE_H

//#include "IUnitFactory.h"
#include "logging.h"

class IUnitFactory;

static IUnitFactory* g_unitFactory=0;

class CLocator {
public: 
  static IUnitFactory* getUnitFactory() {
    if ( g_unitFactory == NULL ) {  
      ERROR(LOG) << "Unit factory is not set! Exiting..";
      return NULL;
    }
    return (IUnitFactory*) g_unitFactory;
  }
    
  static void setUnitFactory(IUnitFactory* factory)
  {
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
};

#endif