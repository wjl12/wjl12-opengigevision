//-----------------------------------------------------------------------------
//  (c) 2005 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/SwissKnifeT.h,v 1.3 2006/07/18 13:31:53 rueder Exp $
//-----------------------------------------------------------------------------
/*!
\file
\brief    Definition of the SwissknifeT class template
*/

#ifndef GENAPI_SWISSKNIFET_H
#define GENAPI_SWISSKNIFET_H

namespace GenApi
{
    /** 
    \brief Implementation of the SwissKnife Interface
    */
    template<class Base>
    class GENAPI_DECL SwissKnifeT : public Base
    {
    public:


        //! Implementation of SwissKnife::GetValue(double value)
        virtual double _GetValue(double value) 
        {
            AutoLock l(Base::GetLock());

            #ifdef LOG4C            
                m_pValueLog->info("_GetValue(value)...");
            #endif
            double Value = Base::_GetValue(value);
            #ifdef LOG4C            
                m_pValueLog->info("...GetValue(value) = %f", Value );
            #endif
            return Value;

        }


    };



}

#endif