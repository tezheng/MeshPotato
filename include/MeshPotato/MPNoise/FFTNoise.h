/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Class for generating procedural FFT Noise
///
///
#ifndef __FFTNOISE_H__
#define __FFTNOISE_H__

#include "MeshPotato/MPUtils/Vector.h"
#include "MeshPotato/MPVolume/MPVolume.h"

#include "Noise.h"

namespace MeshPotato {

	namespace MPNoise {
		/*!
		* \class FFTNoise
		* \brief Class for generating procedural FFT noise using a grid
		*/
		class FFTNoise : public Noise
		{
			public:

				FFTNoise();
				~FFTNoise(){}

				const float eval( const MeshPotato::MPUtils::MPVec3& P ) const;


				void setParameters( const Noise_t& parameters );

				//TODO: Not implemented yet
				void getParameters( Noise_t& parameters ) const;


			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;



		};


	}
}
#endif
