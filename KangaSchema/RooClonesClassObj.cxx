#ifndef ROOCLONESCLASSOBJ_IMPL 
#define ROOCLONESCLASSOBJ_IMPL 
// 
// Implementation of RooClonesClass: 
// 
 
// C++ headers: 
 
 
// This class header: 
#include "KangaSchema/RooClonesClassObj.h" 
 
 
// Collaborating class headers: 
#include "TBuffer.h" 
 
 
//----------------------------------------------------------------------- 
// Local Macros, Typedefs, Structures, Unions and Forward Declarations -- 
//----------------------------------------------------------------------- 
ClassImpT(RooClonesClassObj,T) 
 
 
// Static data members: 
template < class T > 
TClonesArray* RooClonesClassObj< T >::_theArray= 0; 
 
 
// Static member functions: 
template < class T > 
void RooClonesClassObj< T >::setTheArray( TClonesArray* theArray ) {  
 
  _theArray= theArray; 
  _theArray->BypassStreamer(kFALSE);
 
} 
 
 
// Constructor: 
template < class T > 
RooClonesClassObj< T >::RooClonesClassObj() :  
  RooEvtObj< T >(), 
  _nent( 0 ), 
  _array( 0 ), 
  _ownArray( kTRUE ) 
{} 
 
 
// Destructor: 
template < class T > 
RooClonesClassObj< T >::~RooClonesClassObj() {} 
 
 
// Stream an object of class RooEvtObj<T>: 
template < class T >  
void RooClonesClassObj< T >::Streamer( TBuffer & R__b ) { 
 
  if( R__b.IsReading() ) { 
    Version_t R__v= R__b.ReadVersion(); 
    // if (R__v) { } 
    RooEvtObj< T >::Streamer( R__b ); 
    R__b >> _nent; 
    _array->Streamer( R__b ); 
  }  
  else { 
    R__b.WriteVersion( RooClonesClassObj< T >::IsA() ); 
    RooEvtObj< T >::Streamer( R__b ); 
    R__b << _nent; 
    _array->Streamer( R__b ); 
  } 
 
} 
 
// Read a pointer to an object of type RooClonesClassObj<T>: 
template < class T >  
TBuffer & operator >> ( TBuffer & buf, RooClonesClassObj<T>* & obj ) { 
 
  obj= (RooClonesClassObj<T>*) buf.ReadObject( RooClonesClassObj<T>::Class() ); 
  return buf; 
 
} 
 
#endif 
