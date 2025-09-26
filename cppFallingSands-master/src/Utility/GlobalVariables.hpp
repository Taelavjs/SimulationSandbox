
#ifndef GLOBALVARIABLES

#define GLOBALVARIABLES
#include <string>
class GlobalVariables {
public:
	static const int fps;
	const static inline int chunkSize{ 32 };
	static const int worldChunkWidth{ 32 };
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
	static inline double deltaTime = 1.0;
};

#endif /* */
