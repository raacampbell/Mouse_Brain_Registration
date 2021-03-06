// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2009 Gael Guennebaud <gael.guennebaud@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EIGEN_TRIANGULARMATRIXVECTOR_H
#define EIGEN_TRIANGULARMATRIXVECTOR_H

namespace Eigen { 

namespace internal {

template<typename Index, int Mode, typename LhsScalar, bool ConjLhs, typename RhsScalar, bool ConjRhs, int StorageOrder, int Version=Specialized>
struct triangular_matrix_vector_product;

template<typename Index, int Mode, typename LhsScalar, bool ConjLhs, typename RhsScalar, bool ConjRhs, int Version>
struct triangular_matrix_vector_product<Index,Mode,LhsScalar,ConjLhs,RhsScalar,ConjRhs,ColMajor,Version>
{
  typedef typename scalar_product_traits<LhsScalar, RhsScalar>::ReturnType ResScalar;
  enum {
    IsLower = ((Mode&Lower)==Lower),
    HasUnitDiag = (Mode & UnitDiag)==UnitDiag,
    HasZeroDiag = (Mode & ZeroDiag)==ZeroDiag
  };
  static EIGEN_DONT_INLINE  void run(Index _rows, Index _cols, const LhsScalar* _lhs, Index lhsStride,
                                     const RhsScalar* _rhs, Index rhsIncr, ResScalar* _res, Index resIncr, const ResScalar& alpha);
};

template<typename Index, int Mode, typename LhsScalar, bool ConjLhs, typename RhsScalar, bool ConjRhs, int Version>
EIGEN_DONT_INLINE void triangular_matrix_vector_product<Index,Mode,LhsScalar,ConjLhs,RhsScalar,ConjRhs,ColMajor,Version>
  ::run(Index _rows, Index _cols, const LhsScalar* _lhs, Index lhsStride,
        const RhsScalar* _rhs, Index rhsIncr, ResScalar* _res, Index resIncr, const ResScalar& alpha)
  {
    static const Index PanelWidth = EIGEN_TUNE_TRIANGULAR_PANEL_WIDTH;
    Index size = (std::min)(_rows,_cols);
    Index rows = IsLower ? _rows : (std::min)(_rows,_cols);
    Index cols = IsLower ? (std::min)(_rows,_cols) : _cols;

    typedef Map<const Matrix<LhsScalar,Dynamic,Dynamic,ColMajor>, 0, OuterStride<> > LhsMap;
    const LhsMap lhs(_lhs,rows,cols,OuterStride<>(lhsStride));
    typename conj_expr_if<ConjLhs,LhsMap>::type cjLhs(lhs);
    
    typedef Map<const Matrix<RhsScalar,Dynamic,1>, 0, InnerStride<> > RhsMap;
    const RhsMap rhs(_rhs,cols,InnerStride<>(rhsIncr));
    typename conj_expr_if<ConjRhs,RhsMap>::type cjRhs(rhs);

    typedef Map<Matrix<ResScalar,Dynamic,1> > ResMap;
    ResMap res(_res,rows);

    for (Index pi=0; pi<size; pi+=PanelWidth)
    {
      Index actualPanelWidth = (std::min)(PanelWidth, size-pi);
      for (Index k=0; k<actualPanelWidth; ++k)
      {
        Index i = pi + k;
        Index s = IsLower ? ((HasUnitDiag||HasZeroDiag) ? i+1 : i ) : pi;
        Index r = IsLower ? actualPanelWidth-k : k+1;
        if ((!(HasUnitDiag||HasZeroDiag)) || (--r)>0)
          res.segment(s,r) += (alpha * cjRhs.coeff(i)) * cjLhs.col(i).segment(s,r);
        if (HasUnitDiag)
          res.coeffRef(i) += alpha * cjRhs.coeff(i);
      }
      Index r = IsLower ? rows - pi - actualPanelWidth : pi;
      if (r>0)
      {
        Index s = IsLower ? pi+actualPanelWidth : 0;
        general_matrix_vector_product<Index,LhsScalar,ColMajor,ConjLhs,RhsScalar,ConjRhs,BuiltIn>::run(
            r, actualPanelWidth,
            &lhs.coeffRef(s,pi), lhsStride,
            &rhs.coeffRef(pi), rhsIncr,
            &res.coeffRef(s), resIncr, alpha);
      }
    }
    if((!IsLower) && cols>size)
    {
      general_matrix_vector_product<Index,LhsScalar,ColMajor,ConjLhs,RhsScalar,ConjRhs>::run(
          rows, cols-size,
          &lhs.coeffRef(0,size), lhsStride,
          &rhs.coeffRef(size), rhsIncr,
          _res, resIncr, alpha);
    }
  }

template<typename Index, int Mode, typename LhsScalar, bool ConjLhs, typename RhsScalar, bool ConjRhs,int Version>
struct triangular_matrix_vector_product<Index,Mode,LhsScalar,ConjLhs,RhsScalar,ConjRhs,RowMajor,Version>
{
  typedef typename scalar_product_traits<LhsScalar, RhsScalar>::ReturnType ResScalar;
  enum {
    IsLower 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7                                                                                                                                                  	                                                                                                                                                                                                                                      	                                                    	                                                                                                                                                                                                                          	                                                     #    0  '  *  * 7 = 5 4 8 . 0 7 = 4 ; 4 2 / 5 / & 2 ) 2 ) 2 +  9 2 5 = E @ > B N E K N X H K H N B Q T N L L T \ K E R V N R L O U Y R O Y T L I O Y L Q K H K K V V N R B N R Q B H V Q N Y Q K R N H Q @ L > N N R T Q Q b H Q O I K I N H L H H N E K N K T N Q K H K K K E K K H H H K G L T G H B E E N K I H K K E > N B N > H Q K @ B ; N K K > B @ H B B > D H K ; E ; ; ; E K Q K K B H H E @ = E K E H B N H E = H @ E E H K @ : @ > B E E = E B @ B H B D B H K N B E K B K E B K B K > Q N Q A E K E E ; ; H K K = K A B N K K K E > K K K H K : A A K L H 6 @ 7 = = B @ @ > : = > B @ M > H N H > B > K E N H B = E N E H > B H K B : B @ B E B B B H E A B 9 6 @ : ; 7 B : = = = > = 7 9 B K @ E B ; H K K B E H H > K K H H E Q > B E H H K K N > K N N K K G B N K N E B B H @ E = @ B E H K H ; H B ; N G H K B > > H B N N K K N N N Q V [ N K N @ K > B B : > > = @ E B K O I V E Q Q K N E H B = K B @ K K H B K B > E B B : K H > H H = E > H B B B = E K K K H K E E E > K K A B B : = B > @ H E R E K N K G K > K E T B @ E N L K B E K E K E E H E E K E B H Q H E B @ H B E H T K H K G E B > H @ E D K G K N T E H Q E K Q K T R N E @ E N E K O H K B H K K H > Q B B E K H K H E N N K Q L K K K H H K K K H N H N K H E E E H E E B E B E E K E H K N N K B K K N K H H N E E E B H K B > H K E H H B E @ N K N K Q N N Y Q N N T T T R N K Q V L Y N U N Q Q T Q Q T Q T Y N Y Q Q R U \ \ Y V Y T U O K N N Y Y V V Y V T T H T O T N V T K O Q T U Q T Q N Q T L V K V R Q N Q N \ Q Y T T O N N E V H N K K N N G H E E H B K K K > N K V N Y V H V Q N L K N O _ K Q [ Q H K Y N N L N N K K H N K K K H N H T E N Q K K Y Q T Q N T T O D I L K K N E K E L Q K Q H N R \ V N N H K N Q Q Q T E E E E B B E 4 ; > ; = @ B B @ 9 B B E K H K N N N T Q K V T N Q N K I N N K H K L N N I K K B V K N K @ H H Q K H X Q H B G K K N N H K N Q K K Q O X N E B B H E > K K K = > H H X T K K K K K Q L E Q K K K V B K K K H Q T N E H N N K E H Q > > H N X K H N N N X U O R O K Q K Q N H T V K H Q E H K K E K H N > K K B E N N K K Q K K H N H O E V Q H Q N U N N N L E Q K N K K Q K K E N N B K K Q N B T Q H T Q G N @ N Q H B B E 9 = : : > E E E > = : > K H > H K E N Q K N H H T E U K K O K I B B K H H N @ K Q K N E E V Q V H Q H L T Q Q G E T H N H H T N B H K N K K E K K E N K B K > E > > 9 D > > B 9 B N B N N K E Q T K K [ I T K Q H N E N B H K H > K B > K B B N B H Q Q E H N K H E X B E H = E M V D Q B E V V N K N K N E B E H H > B > E B = @ K B B B K G N Y @ K K N I H B N B N K N N K > Q N H K N H K H N 6 H Q N B E K N Q E > E N E B @ H > B H K B @ B E B K K @ H K = E B K > K K B E H K = > H K : > > ; H K Y K T H B G K K B H : E : E K H E E E = > E > = H E E = H B H H = K E G K B K H K E E H K K N V K K N @ > H B > B K H E N K @ B K H K K H Q H E B @ K B > E B Q B Q N B K N B @ B H @ > E H B B B @ : B H = E H > E B E > @ E B K E B N @ E > > E E K > H N B K B N E : E E = B H B E B B K B E H B E E E K E E E B B B H > E @ H A E K J E B H K E B E H B E : > = D > 7 @ = : > B @ K B E H = N H H E E E E B E H H B B B H K K N K K > E @ B B Q B E H Q @ E = B H K N B E E E G B B B B B K E H K H H B E = E H B = H E B E = K K K K H G K B E E E E K E H @ @ : B E A A B G D B @ ; > @ B G B A E B K B @ E B D H @ G H B H G K H K @ H H > E E B @ @ E N J K H Q Q K G E H K K K N Q E N Q N K K Q G N K K Y Q N N K [ E N N N Q Q K K E N X K N N K G > G K K E B K T K E Q ^ T T H H Y H N E H N I H B H E H Q K K H T E N E @ D K K J B N V K K Q Q N O K N Q Q K K T T E Q X N N N K Y Q Y Q Y Q Q > K H B 4 & / +  $  !   $                 !                    !       !    $ + & ) 8 A > H K Q K T K Q Q K K V K T Y H N Q K Q Y V U T V Y Y V T V Q N T Y N T V N Q T T T Q V X Y Y Y K \ R Q N V _ Y Q Q V Y V T \ V T Y Y Y X K N K N H T V Q K K N N B T Q N Q B Q E K E N Q E Y N T N H T Q K N Y Y T Q T Q N Q T K H H Y Y L R N K T N > J K Q X K Q K D K G B H K N N T X V Q R V N H T K X N K Q N K K B N J K K H K K [ N H N Q N E N N H K K V T K Q K H T K N Q L N N N Q T Q K N Q N O K N N H U K N Q V \ T G V O N N Q K N H O T Q T Q Q N Q N Q T K K E E B E H N K B K T T R H K H Y N B N Q K N N N K Q T N H N N H T V Q T N T E @ H H H @ H E B Q N Q K N H E Y Q L N Q Q N K V N N N Q N Y T K E B E K K N N K Q Y N N K K N H K H H H Q B H N Q N K Q Q K Q H N Y N H B K N Q N K S K V K Q K N I Y Q B N Q E T K Q K Q E T K Q B N Q E E B H B E H B H J N Q T T N K N K E N B E B H B Q E K Q H T X N N Q Q T _ V N H O N K V K [ N E B H H E Q K Q E E N Y G Q Q T N I V N E Q N Y K T Q Q Y T Q V Q K K Q N T B E K K H H Q E N Q K E B N H V K H N T H K N N Q Q K K Y T N Q H N H B @ @ H B > B B B = > B B B B > N E > H K Q E K Q B B K K K N K H N N H N K A Q K H H K H E > B 5 E B E > @ @ > : K B B @ K E E B E B N G H B E > B @ B G @ B E A D B K G E D E Q E H B > > G N B B A @ @ D B H E B E B > D B B E @ K G B D E @ H K ; E H H H B E N N H K @ E B @ = H = E B D E H G H B E E @ H H H B B E E E K B @ @ E G H E @ H H N G E N K K K B K E B K E E E B B E K H G B N K B H E H Q K = H H B B K E @ N Q K K X G G H H J N Q E K K N N E N S E E K B N E E @ N K K B H @ K B E > H H D H H  K N G @ B N K B E E K N E K N H I N H H B H K N N K Q V K N > H N Q H N N H T T N V N K T N K N K E K N H Q K H K K B K N B Q M K Q \ K N K Q K H B K K B N K H P T K N H E > N N H K X K T Q N N Q T T N N K K K N K N H T J E H B B K K N N M K G D Q K B K K T Q B T K T N H > K K > B P N K Q T G T N E K [ K K Q K P K Q ^ N B Q [ H Q Y V T L T Q > X Y ^ H K X H N K K E E K K N X Q K Q Y T X K N T Q E T T K H E N H @ J K D B @ B E K K B E H H Q T V Q T K T Q Q N T X H G K K N H Q Q H H J H B Q K H K H Y Q H N N N N K E K K Q K H K G N U T O T [ N K Q Q X K K K Q T N H K K Q Q X B N E B K K K B H Q Q K J B E K K B K N H Q B Q Q Q T K V Q T X V U [ K K T N Q V K H N Q N T T K B N T N X N N N H Q H Q Q Q Q V B N K E H K N Y K E Q K H B B H Q E Q K N K K Q Q Q N Q H B T K K N E N K N T N D Q K N N H N N X K R K Q K T T T B K H B K N E N K H > E T N T Q Q Q Q H Q N B N K H N V H Q N K K N R V N T Q K N K N K 4 E K N Q K Q K K X T K N N Q K H N K N T N H K Q H K B Q K N H K @ N K K H J B N K K H Q K Q K E N N K H N K N N N Q H H K K K B K N E Q Q K B E H N E K J B D Q K Q X E H N B K E E E K K H J @ H B ; A H B B B H K > K 8 H = B J N A = B H = B : E B E : @ B : E B E @ A B D A A G 8 E : @ K G = D @ B @ A B N D J @ 