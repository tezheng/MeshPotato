#include <fftw3.h>
#include "MPVolumeGrid.h"
#include "DenseGrid.h"
namespace MeshPotato {
	namespace MPVolume {
		/// FFTDivFree makes a velocity field divergent free
		VolumeVectorPtr FFTDivFree( VolumeGridVectorPtr divfreeU, VolumeVectorPtr U );

	}
}
