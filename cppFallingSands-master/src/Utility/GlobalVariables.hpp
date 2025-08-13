#include <string>
#ifndef GLOBALVARIABLES

#define GLOBALVARIABLES

class GlobalVariables {
public:
	const static inline int fps{ 60 };
	const static inline int chunkSize{ 48 * 8 };
	const static inline int rendererScale{ 3 };
	const static inline std::string title{ "NoitaB" };
	const static inline int subChunkSizeX = 8;
	const static inline int subChunkSizeY = 8;
	static inline double getDeltaTime() {
		return deltaTime;
	}

	static inline void setDeltaTime(double time) {
		deltaTime = time;
	}


private:
	static inline double deltaTime{ 0 };

};

#endif /* */
