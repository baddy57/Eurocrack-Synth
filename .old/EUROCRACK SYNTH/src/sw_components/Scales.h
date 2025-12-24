#ifndef _SCALES_H_
#define _SCALES_H_
namespace scales {
	
	inline uint majorScale(uint root, uint at) {
		switch (at) {
		case 0: return root;				//W
		case 1: return (root + 2) % 12;		//W
		case 2: return (root + 4) % 12;		//H
		case 3: return (root + 5) % 12;		//W
		case 4: return (root + 7) % 12;		//W
		case 5: return (root + 9) % 12;		//W
		case 6: return (root + 11) % 12;	//H	
		}
		return 0;
	}

	inline uint minorScale(uint root, uint at) {
		return majorScale((root + 5) % 12, at); //relative major
	}

	inline uint chromaticScale(uint root, uint at) {
		return (root + at) % 12;
	}

	inline uint pentatonicMajorScale(uint root, uint at) {
		switch (at) {
		case 0: return root;				//W
		case 1: return (root + 2) % 12;		//W
		case 2: return (root + 4) % 12;		//WH
		case 3: return (root + 7) % 12;		//W
		case 4: return (root + 10) % 12;	//WH
		}
		return 0;
	}

	inline uint pentatonicMinorScale(uint root, uint at) {
		return pentatonicMajorScale((root + 9) % 12, at);
	}

	inline uint bluesScale(uint root, uint at) {
		if (at == 3) return (pentatonicMinorScale(root, at) + 1) % 12;
		else return pentatonicMinorScale(root, at);
	}

}

#endif