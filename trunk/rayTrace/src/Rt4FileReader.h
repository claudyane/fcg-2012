/*
 * Rt4FileReader.h
 *
 *  Created on: May 14, 2012
 *      Author: eliana
 */

#ifndef RT4FILEREADER_H_
#define RT4FILEREADER_H_

#include <string>
#include "Scene.h"

class Rt4FileReader
{
    public:
        Rt4FileReader();

        virtual ~Rt4FileReader();

        void loadScene( const std::string filename, Scene* scene );
};

#endif /* RT4FILEREADER_H_ */
