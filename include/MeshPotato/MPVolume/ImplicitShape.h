/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Collection of classes for representing shapes
///
///   Copyright 2014 Kacey Coley
///   Licensed under the Apache License, Version 2.0 (the "License");
///   you may not use this file except in compliance with the License.
///   You may obtain a copy of the License at
///
///   http://www.apache.org/licenses/LICENSE-2.0
///
///   Unless required by applicable law or agreed to in writing, software
///   distributed under the License is distributed on an "AS IS" BASIS,
///   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
///   See the License for the specific language governing permissions and
///   limitations under the License.
///

#ifndef __IMPLICITSHAPE_H__
#define __IMPLICITSHAPE_H__
#include "MeshPotato/MPVolume/MPVolume.h"
#include "MeshPotato/MPUtils/Vector.h"
#include "MeshPotato/MPNoise/Noise.h"
#include "MeshPotato/MPNoise/PerlinNoise.h"
#include <boost/shared_ptr.hpp>
namespace MeshPotato {
	namespace MPVolume {
		/// Abstract Base class for implicit shapes
		class ImplicitShape: public MeshPotato::MPVolume::Volume<float> {
			public:
			virtual const volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const = 0;
			virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3 &P) const = 0;
		};
		class ImplicitSphere : public ImplicitShape {
			public:
				static VolumeFloatPtr Ptr(const float &_R,  const MeshPotato::MPUtils::MPVec3 &_C);
				ImplicitSphere(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C);
				~ImplicitSphere();
				const volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const;
				const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const;
			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;
		};
		class PyroclasticSphere : public ImplicitShape {
			public:
				static VolumeFloatPtr Ptr(const float &_R,  const MeshPotato::MPUtils::MPVec3 &_C, const MeshPotato::MPNoise::Noise_t &_noiseparms);
				PyroclasticSphere(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C, const MeshPotato::MPNoise::Noise_t &_noiseparms);
				~PyroclasticSphere();
				const volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const;
				const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const;
			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;
		};
		class Cumulo : public ImplicitShape {
			public:
				static VolumeFloatPtr Ptr(VolumeFloatPtr  &_f,  MPNoise::Noise_t &_noiseparms, VolumeVectorPtr &Y);
				Cumulo(VolumeFloatPtr  &_f,  MPNoise::Noise_t &_noiseparms, VolumeVectorPtr &Y);
				~Cumulo();
				const volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const;
				const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const;
			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;
		};

		template<typename T>
		class Clamp: public MeshPotato::MPVolume::Volume<T> {
			public:
			typedef typename GradType<T>::GType volumeGradType;
			static boost::shared_ptr<Volume<T> > Ptr(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > &_f,  const float _e0, const float _e1);

			Clamp(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > &_f,  const float _e0, const float _e1);
			virtual const T eval(const MeshPotato::MPUtils::MPVec3 &P) const;
			virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3 &P) const;
			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;
		};
		typedef Clamp<float> ClampVolumeFloat;

		template<typename T>
		class ConstantVolume: public MeshPotato::MPVolume::Volume<T> {
			public:
			typedef typename GradType<T>::GType volumeGradType;
			static boost::shared_ptr<Volume<T> > Ptr(const T _value);
			ConstantVolume(const T _value);
			virtual const T eval(const MeshPotato::MPUtils::MPVec3 &P) const;
			virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3 &P) const;
			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;

		};
	}
}
#endif // __IMPLICITSHAPE_H__
