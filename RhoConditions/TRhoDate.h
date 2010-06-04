#ifndef TRHODATE_H
#define TRHODATE_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRhoDate		    						//
//                                                                      //
// Date and time class							//
//                                                                      //
// Author: Marcel Kunze, RUB, Feb. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "TObject.h"

const Int_t Jul1901 = 2415386L;      // Julian day for 1/1/1901


class TRhoDate : public TObject
{
    
public:
    
    TRhoDate() : julnum(TRhoDate::currentDate()) {;} // today's date
    TRhoDate(Int_t j) : julnum(j) {;} // Julian day; beware type conversion
    
                                     /* Construct a TRhoDate with a given day of the year and a given year.
                                     The base date for this computation is Dec. 31 of the previous year.
                                     If year == 0, Construct a TRhoDate with Jan. 1, 1901 as the "day zero".
    i.e., TRhoDate(-1) = Dec. 31, 1900 and TRhoDate(1) = Jan. 2, 1901. */
    TRhoDate(unsigned long day, unsigned long year);
    
    //  Construct a TRhoDate for the given day, month, and year.
    TRhoDate(unsigned long d, unsigned long m, unsigned long y);

    Bool_t    Between(const TRhoDate& d1, const TRhoDate& d2) const { return julnum >= d1.julnum && julnum <= d2.julnum; }
    int       CompareTo(const TRhoDate*) const;
    unsigned long  Day() const;            // 1-365
    unsigned long  DayOfMonth() const;     // 1-31
    unsigned long  FirstDayOfMonth(unsigned long mon) const;
    unsigned long  FirstDayOfMonth() const { return FirstDayOfMonth(Month());}
    Bool_t    IsValid() const { return julnum>0; }
    Int_t     Julian()  const { return julnum;   }
    void      Julian(const Int_t j) { julnum = j; }
    Bool_t    Leap() const;
    TRhoDate   Max(const TRhoDate& dt) const;
    TRhoDate   Min(const TRhoDate& dt) const;
    unsigned long  Month() const;
    TRhoDate   Next(unsigned long day) const;                
    TRhoDate   Previous(unsigned long day) const;            
    unsigned long  WeekDay() const;
    unsigned long  Year() const;
    void      Extract(struct tm*) const; // set all tm members
    void      PrintOn( std::ostream& = std::cout) const;

    // Arithmetic operators:
    TRhoDate&  operator++()           { ++julnum; return *this; }
    TRhoDate&  operator--()           { --julnum; return *this; }
    TRhoDate&  operator+=(Int_t dd)   { julnum += dd; return *this; }
    TRhoDate&  operator-=(Int_t dd)   { julnum -= dd; return *this; }
    
    // Global friends:
    friend TRhoDate  operator+(const TRhoDate& dt, Int_t dd) { return TRhoDate(dt.julnum + dd); }
    friend TRhoDate  operator+(Int_t dd, const TRhoDate& dt) { return TRhoDate(dt.julnum + dd); }
    friend TRhoDate  operator-(const TRhoDate& dt, Int_t dd) { return TRhoDate(dt.julnum - dd); }
    friend Int_t    operator-(const TRhoDate& d1, const TRhoDate& d2) { return d1.julnum - d2.julnum; }
    friend Bool_t   operator<(const TRhoDate& d1, const TRhoDate& d2) { return d1.julnum < d2.julnum; }
    friend Bool_t   operator==(const TRhoDate& d1, const TRhoDate& d2) { return d1.julnum == d2.julnum; }    
    friend Bool_t   operator<=(const TRhoDate& d1, const TRhoDate& d2) { return d1.julnum <= d2.julnum; }
    friend Bool_t   operator>(const TRhoDate& d1, const TRhoDate& d2) { return d1.julnum > d2.julnum; }
    friend Bool_t   operator>=(const TRhoDate& d1, const TRhoDate& d2) { return d1.julnum >= d2.julnum; }
    friend Bool_t   operator!=(const TRhoDate& d1, const TRhoDate& d2) { return d1.julnum != d2.julnum; }
    
    // Static member functions:
    static  unsigned long DaysInYear(unsigned long year);
    static  unsigned long DaysInMonthYear(unsigned long month, unsigned long year);
    static  Bool_t   DayWithinMonth(unsigned long mon, unsigned long day, unsigned long yr);
    static  Int_t    Jday(unsigned long mon, unsigned long day, unsigned long year);
    static  Bool_t   LeapYear(unsigned long year);
    static  TRhoDate  Now();                  // return today's date.
    
    // static const data
    static const unsigned char daysInMonth[12];          // [1..31]
    static const unsigned long firstDayOfEachMonth[12];  // [0..334]
    
    
protected:
    
    static Bool_t assertWeekDayNumber(unsigned long d) {return d>=1 && d<=7;}
    static Bool_t assertIndexOfMonth(unsigned long m) {return m>=1 && m<=12;}

private:
    friend class TRhoTime;
    void    mdy(unsigned long& m, unsigned long& d, unsigned long& y) const;
    static  Int_t currentDate();
    Int_t   julnum; // Julian Day Number (not same as Julian date).
    static  void  clobber(struct tm*);

public:
    ClassDef(TRhoDate,1)  // Date class
};

//standalone print
std::ostream& operator << (std::ostream& o, const TRhoDate& );

#endif
