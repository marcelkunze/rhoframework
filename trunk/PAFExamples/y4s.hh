#ifndef ROOT_Event
#define ROOT_Event

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Event                                                                //
//                                                                      //
// Description of the event and track parameters                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "TObject.h"

#define  Length   101
#define  Smax     800.


class Y4S
{
public:

    Y4S()
    {
	P_0 = 3000.;
	RmsEcm = 5.2e-3;
	Gamma0 = 10.e-03;
        Gamma_ee = 0;
	B_ee = 2.1e-05;
	Gamma3Gluon = 0.e-06;
	Y4Smass = 10.580;
	Spin = 1./3.4641016151;
	Color = 1./3.;
	Flavor = 1./1.73205080757;
        init(); 
    }

    virtual ~Y4S() {}

static Double_t resofu(Double_t *x, Double_t *par);

static inline double Pdec(double A,double B,double C) {
 /**********************************************************
 --   CMS decay momentum for A --> B C
 **********************************************************/
    register double y;
    y = (A*A-(B+C)*(B+C))*(A*A-(B-C)*(B-C)) / (4.*A*A);
    return ( y > 0. ? sqrt(y) : 0. );
}

static double Dsigma(double x);

static double Dgauss(double fun(double),double xmin,
              double xmax, double accuracy);

static double Dcauch(double fun(double),double xmin,double xmax,
              double Singularity,double accuracy);


static double VertexCorr(double x);

static double Beta(double x);

static double Vertex_corr(double W);

static double gamma(double x);

static double ConflHyp(double A,double B,double X);

static double PhotonSpectrum(double W,double Egamma);

static double Ywidth(double Mass,double plusGamma);

static void Yinit();

static void init();

static double DispS(double Sprime);

static double DispM(double Sprime);

static void Compute_running_mass();

static double RadGauss(double x);
static double NonRadGauss(double x);
static double BreitWigner(double W);

/*
double EcmNominal,
       DecayConst,
       Y4SP,
       sqrt2Pi,
       alphaV, Etha, Rba, RbEtha, OaT, F1, F2, F3, F4,
       EcmMin, EcmMax, EcmH, EcmS, Buf[Length+1],
RmsEcm,
Gamma0,
Gamma_ee,
B_ee, 
Gamma3Gluon,
Y4Smass,
Spin,
Color,
Flavor;
*/

//ClassDef(Y4S,1) // Y4S fit

private:

static
double EcmNominal,
       DecayConst,
       Y4SP,
       alphaV,
       Etha, Rba, RbEtha, OaT, F1, F2, F3, F4,
       EcmMin, EcmMax, EcmH, EcmS, Buf[Length+1],

       P_0,	    /* sigma(cont? * s */
       RmsEcm,	    /* rms of EcmNominal (machine resolution) */
       Gamma0,	    /* total width at W=Y4Smass */
       Gamma_ee,
       B_ee,	    /* BR into e+e-, Gamma=0.28e-6 */
       Gamma3Gluon, /* 50.e-6, partial width into 3 Gluons */
       Y4Smass,

       Spin,
       Color,
       Flavor;

};

#endif
