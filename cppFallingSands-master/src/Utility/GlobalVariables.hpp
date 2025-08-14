
#ifndef GLOBALVARIABLES

#define GLOBALVARIABLES
#include <string>
class GlobalVariables {
public:
	static const int fps;
	const static inline int chunkSize{ 64 };
	static const int worldChunkWidth;
	static const int rendererScale;
	static const std::string title;
	static const int subChunkSizeX;
	static const int subChunkSizeY;

	static inline double getDeltaTime() {
		return deltaTime;
	}

	static inline void setDeltaTime(double time) {
		deltaTime = time;
	}
private:
	static inline double deltaTime = 0.0;
};

#endif /* */
