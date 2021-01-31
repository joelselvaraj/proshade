/***************************************************************************
  **************************************************************************
  
  SOFT: SO(3) Fourier Transforms
  Version 2.0

  Copyright (c) 2003, 2004, 2007 Peter Kostelec, Dan Rockmore
  
  This file is part of SOFT.

  SOFT is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  SOFT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
  See the accompanying LICENSE file for details.
  
  ************************************************************************
  ************************************************************************/

/*
  header file for full SO3 transform routines which use fftw

  these functions will write over the input data

  Forward_SO3_Naive_fftw_nt() - do forward full SO(3) transform
  Inverse_SO3_Naive_fftw_nt() - do inverse full SO(3) transform

  Both routines require fftw!!!


*/

#ifndef _SOFT_FFTW_NT_H
#define _SOFT_FFTW_NT_H 1

extern void Forward_SO3_Naive_fftw_nt( int ,
				       fftw_complex * ,
				       fftw_complex * ,
				       fftw_complex * ,
				       double * ,
				       double * ,
				       fftw_plan * ,
				       int ) ;

extern void Inverse_SO3_Naive_fftw_nt( int ,
				       fftw_complex * ,
				       fftw_complex * ,
				       fftw_complex * ,
				       double * ,
				       fftw_plan * ,
				       int ) ;

#endif /* _SOFT_FFTW_NT_H */

