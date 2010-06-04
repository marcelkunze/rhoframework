#include "y4s.hh"
#include "TMath.h"

//ClassImp(Y4S)

#define  RADCORR  1
#define  BREITWIG 1     /* 1 QPC, 2 L=0-PS, 3 L=1 PS, 4 const. */
#define  REPROP   0     /* 1 real part of propagator used */
#define  BUFPRT   0
#define  TRACE    0

#define  Bmass       5.279
#define  hbarc_sq    389379.66
#define  ElectronMass  0.51099906e-03
#define  PionMass    0.1395675
#define  RmsEcm_min  2.e-04
#define  dkappa_min  1.e-10
#define  Acc         1.e-07

#define Sq6o35 0.549285249
#define XMB    5.17
#define XMU    0.33
#define XNORM  50.02279
#define RA     2.9152
#define RB     6.1425

double Y4S::EcmNominal;
double Y4S::DecayConst;
double Y4S::Y4SP;
double Y4S::alphaV;
double Y4S::Etha;
double Y4S::Rba;
double Y4S::RbEtha;
double Y4S::OaT;
double Y4S::F1;
double Y4S::F2;
double Y4S::F3;
double Y4S::F4;
double Y4S::EcmMin;
double Y4S::EcmMax;
double Y4S::EcmH;
double Y4S::EcmS;
double Y4S::Buf[Length+1];
double Y4S::P_0;
double Y4S::RmsEcm;
double Y4S::Gamma0;
double Y4S::Gamma_ee;
double Y4S::B_ee;
double Y4S::Gamma3Gluon;
double Y4S::Y4Smass;
double Y4S::Spin;
double Y4S::Color;
double Y4S::Flavor;


Double_t Y4S::resofu(Double_t *x, Double_t *par)
{  Double_t y; 
	
    double Y4sXsection = 0.;
    double dkappa_max, Xsect_hard, Xsect_soft, SoftPhoton_corr;
    double Wmin, Wmax;

    Y4Smass = 10.580; 
    EcmNominal = x[0]-par[1]-10.+Y4Smass;
  
    P_0 = par[0]; 

    Gamma0 = par[2];
    RmsEcm = par[3];
    B_ee = par[4]*1.e-5;

    Y4SP        = Pdec(Y4Smass,Bmass,Bmass); 

    Gamma_ee    = B_ee * Gamma0;

    DecayConst = 1.;
    DecayConst  = Gamma0/Ywidth(Y4Smass,0.);
//     cout << "%%decayconst =" << DecayConst;

    if (RmsEcm < RmsEcm_min)  {
#if RADCORR == 1
      /* .....Xsection for ideal machine */
       dkappa_max = 1.-4.*(Bmass/EcmNominal)*(Bmass/EcmNominal);
       if (dkappa_max < dkappa_min)  dkappa_max = dkappa_min;
       Xsect_hard = Dgauss(Dsigma,dkappa_min,dkappa_max,Acc);
       Xsect_soft = BreitWigner(EcmNominal)*
           (1.+Vertex_corr(EcmNominal))*
           pow(dkappa_min,Beta(EcmNominal));
       Y4sXsection = Xsect_hard+Xsect_soft;
#else
       Y4sXsection = BreitWigner(EcmNominal);
#endif
    }
    else
    {
      /* .....Xsection for real machine */
#if RADCORR == 1
       SoftPhoton_corr = pow(2*RmsEcm/EcmNominal,Beta(EcmNominal)) /
           RmsEcm * (1.+Vertex_corr(EcmNominal));
#endif
       Wmin = 2.*Bmass;
       Wmax = EcmNominal+5.*RmsEcm;
       if (Wmax < Wmin)  Wmax = Wmin;
#if RADCORR == 1
       Y4sXsection = Dgauss(RadGauss,Wmin,Wmax,Acc) *
           SoftPhoton_corr;
#else
       Y4sXsection = Dgauss(NonRadGauss,Wmin,Wmax,Acc);
#endif
    }

#if TRACE == 1
//    fprintf(stderr," Gamma0 %12.5f, EcmNom. %12.4f, sigma %10.4f\n",
//            Gamma0,EcmNominal,Y4sXsection);
#endif
    return Y4sXsection + P_0 / (EcmNominal*EcmNominal);
}


void Y4S::init()
{
    Yinit();
    
    alphaV      = 2./(137.0359895*TMath::Pi());
 
    EcmMin      = 10.540;
    EcmMax      = 10.740;
 
    Y4SP        = Pdec(Y4Smass,Bmass,Bmass); 

    Gamma_ee    = B_ee * Gamma0;

    DecayConst = 1.;
    DecayConst  = Gamma0/Ywidth(Y4Smass,0.);
//     cout << "%%decayconst =" << DecayConst;
    
    Compute_running_mass();

    return;
}


double Y4S::Dsigma(double dkappa) {
  /*
  --  Convolution of photon spectrum with BreitWigner
  */

    double Dsig;
    double EcmActual, Egamma;

    Dsig = 0.;
    EcmActual = EcmNominal * sqrt(1.-dkappa);
    if (EcmActual < 2.0*Bmass) {
        return Dsig;
    }
    Egamma = 0.5 * EcmNominal * dkappa;
    Dsig = BreitWigner(EcmActual) *
        PhotonSpectrum(EcmNominal,Egamma);

    return Dsig;
}

double Y4S::PhotonSpectrum(double W,double Egamma) {
 /***********************************************************
 --   Photon spectrum in first order QED;
          neglected terms: ~ 1 - Egamma/W + .5*(Egamma/W)**2
  **********************************************************/
    double PhotonSp = 0.;
    double dkappa, b;

    if (W <= 0.) { return PhotonSp; }
    dkappa = 2. * Egamma / W;
    b = Beta(W);
    PhotonSp = b * pow(dkappa,b-1.) * (1.+Vertex_corr(W));

    return PhotonSp;
}

double Y4S::Vertex_corr(double W) {
 /******************************************************
 --   Vertex correction; first order QED
 ******************************************************/

    double Vertex_co = 0.;
    if (W > 0.) {
        Vertex_co = alphaV * (1.5 * log(W/ElectronMass)
            - 1. + TMath::Pi()*TMath::Pi()/6.);
    }
    return Vertex_co;
}

double Y4S::ConflHyp(double A,double B,double X) {
 /*******************************************************
 --   Computes confluent hypergeometric function
 *******************************************************/
    double Xn, A1, B1, X1, Follow, Confl, Clast;

    Xn=1.0;
    A1=A;
    B1=B;
    X1=X;
    Follow=A1/B1*X1;
    Confl=1.+Follow;
    Clast=0.;
    while (fabs(Confl-Clast) > Acc) {
        Clast=Confl;
        A1 += 1.;
        B1 += 1.;
        Xn += 1.;
        Follow *= A1*X1/(B1*Xn);
        Confl += Follow;
    }
    return Confl;
}

double Y4S::Beta(double W) {
 /******************************************************
 --   Equivalent radiator thickness; first order QED
 ******************************************************/

    if (W > 0.) {
      return  alphaV*(2.*log(W/ElectronMass)-1.);
    }
    else { return 0.; }
}

void Y4S::Compute_running_mass() {
 /**********************************************************************
 --   Computes running mass
 **********************************************************************/
      double Thres, Shftm, Shfts;
      int I;

      EcmH        = (EcmMax-EcmMin)/Length;
 
#if REPROP == 1
      Thres = 4.*PionMass*PionMass;
      Shftm = Y4Smass*Dcauch(DispM,Thres,Smax,Y4Smass*Y4Smass,Acc)/Pi;

      for (I=0; I <= Length; I++) {
        EcmS = (I*EcmH + EcmMin);
        EcmS *= EcmS;
        Shfts = Y4Smass*Dcauch(DispS,Thres,Smax,EcmS,Acc)/Pi;
        if (Shfts == 0.)  {
          EcmS = ((I+1)*EcmH + EcmMin);
          EcmS *= EcmS;
          Shfts = Y4Smass*Dcauch(DispS,Thres,Smax,EcmS,Acc)/Pi;
          Buf[I] = 0.5 * (Buf[I-1] + Shfts - Shftm);
#if BUFPRT == 1
//          printf("%%%15.3f %5d: %15.7f, %15.7f/ %15.8f\n",
//              sqrt(EcmS),I,Shftm,Shfts,Buf[I]);
#endif
        }
        else {
          Buf[I] = Shfts-Shftm;
#if BUFPRT == 1
//          printf("%%%15.3f %5d: %15.7f, %15.7f: %15.8f\n",
//              sqrt(EcmS),I,Shftm,Shfts,Buf[I]);
#endif
        }
      }
#else
      for (I=0; I <= Length; I++) {
          Buf[I] = 0.;
      }
#endif

      return;
}

double Y4S::DispS(double Sprime) {
 /**********************************************************************
 --   Integrand of the dispersion integral at S
 **********************************************************************/
      double Ecm;

      Ecm = sqrt(Sprime);
      return Ywidth(Ecm,Gamma3Gluon)/(EcmS-Sprime);
}


double Y4S::DispM(double Sprime) {
 /**********************************************************************
 --   Integrand of the dispersion integral at M
 **********************************************************************/
      double Ecm;

      Ecm = sqrt(Sprime);
      return Ywidth(Ecm,Gamma3Gluon)/(Y4Smass*Y4Smass-Sprime);
}

double Y4S::Ywidth(double Ecm,double plusGamma) {
 /*****************************************************
 --   Y4S width in the QPC model
 *****************************************************/

      double Ywid;
      double P, Ecmf1, Ecmf2, Dlips, Petha, Y4S_ME;

#if BREITWIG == 4
      return Gamma0;
#else

      if (Ecm < 2.*PionMass)  return 0.;
      Ecmf1 = Ecm/Y4Smass;
      Ecmf2 = Ecmf1*Ecmf1;
      P = Pdec(Ecm,Bmass,Bmass);
      if (P == 0.)  return plusGamma/Ecmf2 ;

#if BREITWIG == 1
      Dlips = P/(8.*TMath::Pi()*Ecm*Ecm);
      Petha = P*Etha;
      Y4S_ME = OaT*(F1*pow(Petha,6.) + F2*pow(Petha,4.)
          + F3*Petha*Petha + F4) * exp(-0.25*Petha*Petha
          *RA*RB/Rba) * P;
      Ywid = DecayConst*Y4S_ME*Y4S_ME*Dlips;
#else
#if BREITWIG == 2
      Ywid = Gamma0 * P / Ecmf2 / Y4SP;
#else
#if BREITWIG == 3
      Ywid = Gamma0 * P*P*P / Ecmf2 / (Y4SP*Y4SP*Y4SP);
#endif
#endif
#endif

      return Ywid + plusGamma/Ecmf2;
#endif
}


void Y4S::Yinit()
      {
        Etha=XMB/(XMB+XMU);
        Rba=2.*RB+RA;
        RbEtha=RB*Etha;
        OaT   = 1./(TMath::Pi()*TMath::Pi()) * pow(RA,0.75) * pow(RB,1.5)
            / sqrt(Rba) * Sq6o35 * Spin * Color * Flavor
            * XNORM;
        F1= 16./3.*(RbEtha - Rba)*RA*RA*RA*pow(RB,6.)
            *pow(Rba,-8.);
        F2= 8.*(11.*RbEtha*RA - 14.*RbEtha*RB
            - 7.*RA*RA + 28.*RB*RB) * pow(RA*RB*RB,2.)
            *pow(Rba,-7.);
        F3= 28.*(13.*RbEtha*RA*RA -
            36.*RbEtha*RA*RB + 20.*RbEtha*RB*RB -
            5.*pow(RA,3.) + 10.*RA*RA*RB + 20.*RA*RB*RB
            - 40.*pow(RB,3.)) * RA*RB*RB*pow(Rba,-6.);
        F4= 70.*(5.*RbEtha*pow(RA,3.)
            - 22.*RbEtha*RA*RA*RB + 28.*RbEtha*RA*RB*RB
            - 8.*RbEtha*pow(RB,3.) - pow(RA,4.)
            + 4.*pow(RA,3.)*RB - 16.*RA*pow(RB,3.)
            + 16.*pow(RB,4.))/pow(Rba,5.);
        return;
}

double DgaussW[] = {
0.1012285362903762591525313543e0,
0.2223810344533744705443559944e0,
0.3137066458778872873379622020e0,
0.3626837833783619829651504493e0,
0.2715245941175409485178057246e-1,
0.6225352393864789286284383699e-1,
0.9515851168249278480992510760e-1,
0.1246289712555338720524762822e0,
0.1495959888165767320815017305e0,
0.1691565193950025381893120790e0,
0.1826034150449235888667636680e0,
0.1894506104550684962853967232e0
};

double DgaussX[] = {
0.9602898564975362316835608686e0,
0.7966664774136267395915539365e0,
0.5255324099163289858177390492e0,
0.1834346424956498049394761424e0,
0.9894009349916499325961541735e0,
0.9445750230732325760779884155e0,
0.8656312023878317438804678977e0,
0.7554044083550030338951011948e0,
0.6178762444026437484466717640e0,
0.4580167776572273863424194430e0,
0.2816035507792589132304605015e0,
0.9501250983763744018531933543e-1
};

double Y4S::Dgauss(double F(double),double A,double B,double EPS) {

      double AA,BB,C1,C2,U,S8,S16,CONST,DGAUSS;
	  int I;
     /******************************************************************

      ADAPTIVE DOUBLE PRECISION GAUSSIAN QUADRATURE.

      DGAUSS IS SET EQUAL TO THE APPROXIMATE VALUE OF THE INTEGRAL OF
      THE FUNCTION F OVER THE INTERVAL (A,B), WITH ACCURACY PARAMETER
      EPS.

      from CERNLIB FORTRAN

      ******************************************************************/

      DGAUSS = 0.;
      if (B == A) return DGAUSS;

      CONST = 0.005/(B-A);
      BB = A;

      /* COMPUTATIONAL LOOP. */
      while (BB != B) {

        AA = BB;
        BB = B;

        while (1) {
          C1 = 0.5*(BB+AA);
          C2 = 0.5*(BB-AA);
          S8 = 0.;
          for (I=0; I < 4; I++) {
            U = C2*DgaussX[I];
            S8 += DgaussW[I] * (F(C1+U) + F(C1-U));
          }
          S8 *= C2;
          S16 = 0.;
          for (I=4; I < 12; I++) {
            U = C2*DgaussX[I];
            S16 += DgaussW[I] * (F(C1+U) + F(C1-U));
          }
          S16 *= C2;
          if (fabs(S16-S8) <= EPS * (1.+fabs(S16)) )  break;
          BB = C1;
          if ( 1.+fabs(CONST*C2) == 1.)  {
//            fprintf(stderr,"Dgauss: too high accuracy required\n");
            break;
          }
        }
        DGAUSS += S16;
      }

      return DGAUSS;
}

double Y4S::Dcauch(double F(double),double A,double B,double S,double EPS) {
  /*
  compute the Cauchy principal value integral F(x) dx
  limits A..B
  singularity S
  accuracy EPS
  CERNLIB D104
  */

      double H, B0, AA, BB, C;
      double C1, C2, C3, C4, S8, S16, U;
      register int I;

      if ((S == A) || (S == B)) {
    /* error */
//        fprintf(stderr,"Dcauch: invalid S = A or S = B\n");
        return 0.;
      }
      else if  (((S < A) && (S < B)) || ((S > A) && (S > B))) {
        return Dgauss(F,A,B,EPS);
      }

      if (2.*S <= A+B) {
          H=Dgauss(F,2*S-A,B,EPS);
          B0=S-A;
      }
      else {
          H=Dgauss(F,A,2*S-B,EPS);
          B0=B-S;
      }
      C=0.005/B0;
      BB=0.;

      do {
          AA=BB;
          BB=B0;
          while (1) {
            C1=0.5*(BB+AA);
            C2=0.5*(BB-AA);
            C3=S+C1;
            C4=S-C1;
            S8=0;
            for (I = 0; I < 4; I++) {
              U=C2*DgaussX[I];
              S8 += DgaussW[I] * ((F(C3+U)+F(C4-U))+(F(C3-U)+F(C4+U)));
            }
            S8 *= C2;
            S16=0;

            for (I = 4; I < 12; I++) {
              U=C2*DgaussX[I];
              S16 += DgaussW[I] * ((F(C3+U)+F(C4-U))+(F(C3-U)+F(C4+U)));
            }
            S16 *= C2;

            if (fabs(S16-S8) <= EPS*(1+fabs(S16)))  break;
            BB=C1;
            if (1. + fabs(C*C2) == 1.) {
//              fprintf(stderr,"Dcauch: too high accuracy required\n");
              break;
            }
         }
         H += S16;
     }  while (BB != B0);
     return H;
}

#define PRECISION 0

#if PRECISION == 1
 /*
   high precision
  */

#define NC 25
#define PI 3.141592653589793238462643383

double gammaC[NC] = {
      3.657387725083382438498806839,
      1.957543456661268269283374226,
       .338297113826160389155851073,
       .042089512765575491985108397,
       .004287650482129087700428908,
       .000365212169294617670219822,
       .000027400642226422002717066,
       .000001812402333651244460305,
       .000000109657758659970699306,
       .000000005987184045520004695,
       .000000000307690805352477771,
       .000000000014317930296191576,
       .000000000000651087733480370,
       .000000000000025958498982228,
       .000000000000001107893892259,
       .000000000000000035474362017,
       .000000000000000001688607504,
       .000000000000000000027354358,
       .000000000000000000003029774,
      -.000000000000000000000057122,
       .000000000000000000000009077,
      -.000000000000000000000000505,
       .000000000000000000000000041,
      -.000000000000000000000000003,
       .000000000000000000000000001
};

#else

 /*
   standard precision
  */

#define NC 16
#define PI 3.14159265358979324

double gammaC[NC] = {
3.65738772508338244,
1.95754345666126827,
 .33829711382616039,
 .04208951276557549,
 .00428765048212909,
 .00036521216929462,
 .00002740064222642,
 .00000181240233365,
 .00000010965775866,
 .00000000598718405,
 .00000000030769081,
 .00000000001431793,
 .00000000000065109,
 .00000000000002596,
 .00000000000000111,
 .00000000000000004
};

#endif

double Y4S::gamma(double x) {
 
  double U, F, H, alfa, B0, B1, B2;
  int I, IM;

  U = x;
  B1 = B2 = 0.;

  if (x <= 0)  {
     if (x == floor(x)) {
        U = 1./B1;
     }
     else   U = 1.-U;
  }

  F = 1.;

  if (U < 3) {
      IM = (int)(4.-U);
	  for (I = 1; I <= IM; ++I) {
       F /= U;
       U += 1.;
     }
  } 
  else {
	 IM = (int)(U-3.);   
     for (I = 1; I <= IM; ++I) {
       U -= 1.;
       F *= U;
     }
  }
  U -= 3.;
  H = U+U-1.;
  alfa = H + H;
  
  for (I = NC-1; I >= 0; --I) {

      B0 = gammaC[I] + alfa * B1 - B2;
      B2 = B1;
      B1 = B0;
  }

  U = F * (B0 - H*B2);

  if (x < 0) U = PI / (sin(PI*x) * U);
  return U;
}

double Y4S::RadGauss(double W)
{
 /***********************************************************
 --   Convolution of photon spectrum with machine resolution
 ************************************************************/

    double RadGa = 0.;
    double z, b;
    double BetaHalf, BetaPower, GammaFunction_1,
        GammaFunction_2, Convolution;

    z = (EcmNominal-W)/RmsEcm;
    if (z  <=  -5.)  { return RadGa; }
    else {
        b = Beta(EcmNominal);
        if (z > -5. && z < 6.) {
            BetaHalf = b/2;
            BetaPower = pow(2.,BetaHalf);
            GammaFunction_1 = gamma(1.+BetaHalf);
            GammaFunction_2 = gamma(1.+b);
            Convolution = BetaPower * GammaFunction_1 *
		ConflHyp(0.5-BetaHalf,0.5,-0.5*z*z)/sqrt(2.*TMath::Pi());
              + BetaHalf*GammaFunction_2 /
              (BetaPower*GammaFunction_1) * z *
              ConflHyp(1.-BetaHalf,1.5,-0.5*z*z);
        }
        else if (z >= 6.) {
            Convolution = b * pow(z,b-1.) *
              (1.+(1.-b)*(2.-b)/(2.*z*z));
        }
    }

    RadGa = Convolution * BreitWigner(W);

    return RadGa;

}

double Y4S::NonRadGauss(double W)
{
 /***********************************************************
 --   Convolution of with machine resolution
 ************************************************************/

    double RadGa = 0.;
    double z;
    double Convolution;

    z = (EcmNominal-W)/RmsEcm;
    Convolution = exp(-z*z*0.5)/(sqrt(2.*TMath::Pi())*RmsEcm);

    RadGa = Convolution * BreitWigner(W);

    return RadGa;

}


double Y4S::BreitWigner(double W) {
 /************************************************************
 --   BreitWigner with running mass and energy dependent width
 ************************************************************/

    double BreitWig = 0.;
    double P, Dlips, Y4Swidth, Xmesh, Dmass_shift, FullMass_sq;
    int Ipoint;
    double W_sq = W*W;

    Y4Swidth = Ywidth(W,Gamma3Gluon);

    Xmesh = (W - EcmMin)/EcmH;
    if (Xmesh < 0.0) return 0.0;

    Ipoint = (int)Xmesh;
    if (Ipoint < Length) {
        Dmass_shift = Buf[Ipoint+1]*fabs(Xmesh-Ipoint) +
            Buf[Ipoint] * fabs(Xmesh-(Ipoint+1));
    }
    else {
        Dmass_shift = Buf[Length];
    }

    FullMass_sq = Y4Smass*Y4Smass + Dmass_shift;
    BreitWig = Gamma_ee * hbarc_sq * 12. * TMath::Pi() * Y4Swidth /
       ((W_sq - FullMass_sq)*(W_sq - FullMass_sq) + Y4Swidth * Y4Swidth *
       Y4Smass*Y4Smass);

    return BreitWig;
}
