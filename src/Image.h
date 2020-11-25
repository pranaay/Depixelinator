#ifndef IMAGE_H
#define IMAGE_H

#include "Pixels.h"
#include "GL/gl3w.h"
class Image {

	public:
        ~Image();
		Image(const char* filename, unsigned int slot);
		int getWidth();
		int getHeight();
        unsigned int getTexId();
        unsigned char* getBuffer();
	private:
		int m_Height;
		int m_Width;
		int m_BPP;
		unsigned int m_texId;
        unsigned char* m_Buffer;
        Pixels **m_img;

};
#endif