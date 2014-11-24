
#include "MPNoise/Noise.h"
namespace MeshPotato {
	namespace MPNoise {


		Noise::Noise() {};

		Noise::~Noise() {};

		const float Noise::eval( const float x ) const { return 0;}
		const float Noise::eval( const MeshPotato::MPUtils::MPVec3& x ) const {return 0;}

		void Noise::setParameters( const Noise_t& parameters ) {}
		void Noise::getParameters( Noise_t& parameters ) const {}



		const Anchor evaluateAnchorChain( const AnchorChain& anchorList, const double x )
		{
			int anchorsize = (int)anchorList.size();
			int anchor = x * anchorsize;
			double w = x * anchorsize - (double)anchor;
			if( anchor < 0 )
			{
				anchor = 0;
				w = 0;
			}
			if( anchor >= anchorsize-1 )
			{
				anchor = anchorsize-2;
				w = 1;
			}

			double w0 = 1.0 - w;
			Anchor value;
			value.frequency                 = anchorList[anchor].frequency * w0                 + anchorList[anchor+1].frequency * w;
			value.translate                 = anchorList[anchor].translate * w0                 + anchorList[anchor+1].translate * w;
			value.octaves                   = anchorList[anchor].octaves * w0                   + anchorList[anchor+1].octaves * w;
			value.amplitude                 = anchorList[anchor].amplitude * w0                 + anchorList[anchor+1].amplitude * w;
			value.offset                    = anchorList[anchor].offset * w0                    + anchorList[anchor+1].offset * w;
			value.fjump                     = anchorList[anchor].fjump * w0                     + anchorList[anchor+1].fjump * w;
			value.roughness                 = anchorList[anchor].roughness * w0                 + anchorList[anchor+1].roughness * w;
			value.radius                    = anchorList[anchor].radius * w0                    + anchorList[anchor+1].radius * w;
			value.pscale                    = anchorList[anchor].pscale * w0                    + anchorList[anchor+1].pscale * w;
			value.amplitude                 = anchorList[anchor].amplitude * w0                 + anchorList[anchor+1].amplitude * w;
			value.gamma                     = anchorList[anchor].gamma * w0                     + anchorList[anchor+1].gamma * w;
			value.time                      = anchorList[anchor].time * w0                      + anchorList[anchor+1].time * w;
			value.fftLowCutoff              = anchorList[anchor].fftLowCutoff * w0              + anchorList[anchor+1].fftLowCutoff * w;
			value.fftHighCutoff             = anchorList[anchor].fftHighCutoff * w0             + anchorList[anchor+1].fftHighCutoff * w;
			value.fftPower                  = anchorList[anchor].fftPower * w0                  + anchorList[anchor+1].fftPower * w;
			value.fftNbGridPoints           = anchorList[anchor].fftNbGridPoints * w0           + anchorList[anchor+1].fftNbGridPoints * w;
			value.fftLength                 = anchorList[anchor].fftLength * w0                 + anchorList[anchor+1].fftLength * w;
			value.lognormalmean             = anchorList[anchor].lognormalmean * w0             + anchorList[anchor+1].lognormalmean * w;
			value.gaussianstandarddeviation = anchorList[anchor].gaussianstandarddeviation * w0 + anchorList[anchor+1].gaussianstandarddeviation * w;
			value.seed                      = anchorList[anchor].seed * w0                      + anchorList[anchor+1].seed * w;
			value.tangent                   = anchorList[anchor].tangent * w0                   + anchorList[anchor+1].tangent * w;
			value.normal                    = anchorList[anchor].normal * w0                    + anchorList[anchor+1].normal * w;
			value.binormal                  = anchorList[anchor].binormal * w0                  + anchorList[anchor+1].binormal * w;
			value.axis                      = anchorList[anchor].axis * w0                      + anchorList[anchor+1].axis * w;
			value.angle                     = anchorList[anchor].angle * w0                     + anchorList[anchor+1].angle * w;
			value.P                         = anchorList[anchor].P * w0                         + anchorList[anchor+1].P * w;
			value.v                         = anchorList[anchor].v * w0                         + anchorList[anchor+1].v * w;
			value.A                         = anchorList[anchor].A * w0                         + anchorList[anchor+1].A * w;

			return value;
		}

		const WispAnchor evaluateWispAnchorChain( const WispAnchorChain& alist, int anchor, const double x ) {}



		template< typename BaseNoise>
		 boost::shared_ptr<FractalSum<BaseNoise> > Ptr() {
			return boost::shared_ptr<FractalSum<BaseNoise> >(new FractalSum<BaseNoise>());
			}

			template< typename BaseNoise>
				FractalSum<BaseNoise>::FractalSum() :
					octaves    (3.0),
					fjump      (2.0),
					roughness  (0.5),
					frequency  (0.666666),
					translate  (MeshPotato::MPUtils::MPVec3(0,0,0)),
					offset     (0.0),
					axis       (MeshPotato::MPUtils::MPVec3(0,0,1)),
					angle      (0.0)
			{}

				template <typename BaseNoise>
				FractalSum<BaseNoise>::~FractalSum(){}

				template <typename BaseNoise>
				const float FractalSum<BaseNoise>::eval( const float x ) const
				{
					float exponent = 1;
					float amplitude = 1;
					float accum = 0;
					int ioct = (int)octaves;
					for( int oc=0;oc<ioct;oc++ )
					{
						const float X = (x - translate[0]) * frequency * exponent;
						accum += amplitude * _noise.eval( X );
						exponent *= fjump;
						amplitude *= roughness;
					}
					const float X = (x - translate[0]) * frequency * exponent;
					float val = amplitude * _noise.eval( X );
					accum += (octaves - (int)octaves) * val;
					return (accum + offset);
				}


				template <typename BaseNoise>
				const float FractalSum<BaseNoise>::eval( const MeshPotato::MPUtils::MPVec3& x ) const
				{
					float exponent = 1;
					float amplitude = 1;
					float accum = 0;
					int ioct = (int)octaves;
					MeshPotato::MPUtils::MPVec3 X = (x - translate);
					if( angle != 0.0 )
					{
						float ca = cos(angle);
						float sa = sin(angle);
						X = X*ca + axis*(axis*X)*(1.0-ca) + (axis.cross(X))*sa;
					}
					X *= frequency*exponent;
					for( int oc=0;oc<ioct;oc++ )
					{
						accum += amplitude * _noise.eval( X );
						X *= fjump;
						amplitude *= roughness;
					}
					float val = amplitude * _noise.eval( X );
					accum += (octaves - (int)octaves) * val;
					return (accum+offset);
				}

				template <typename BaseNoise>
				void FractalSum<BaseNoise>::setParameters( const Noise_t& parameters )
				{
					octaves = parameters.octaves;
					fjump = parameters.fjump;
					roughness = parameters.roughness;
					frequency = parameters.frequency;
					translate = parameters.translate;
					offset = parameters.offset;
					axis = parameters.axis;
					angle = parameters.angle;
					_noise.setTime( parameters.time );
				}

				template <typename BaseNoise>
				void FractalSum<BaseNoise>::getParameters( Noise_t& parameters ) const
				{
					parameters.octaves = octaves;
					parameters.fjump = fjump;
					parameters.roughness = roughness;
					parameters.frequency = frequency;
					parameters.translate = translate;
					parameters.offset = offset;
					parameters.axis = axis;
					parameters.angle = angle;
				}




				PRN::PRN(){}
				PRN::~PRN(){}

				const float PRN::eval(){ return 0; }

				void PRN::setParameters( const Noise_t& parameters ){}



	}
}
