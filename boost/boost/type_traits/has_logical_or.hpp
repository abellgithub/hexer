//  (C) Copyright 2009-2011 Frederic Bron.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_HAS_LOGICAL_OR_HPP_INCLUDED
#define BOOST_TT_HAS_LOGICAL_OR_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_logical_or
#define BOOST_TT_TRAIT_OP ||
#define BOOST_TT_FORBIDDEN_IF\
   /* pointer with fundamental non convertible to bool */\
   ::hexerboost::type_traits::ice_or<\
      ::hexerboost::type_traits::ice_and<\
         ::hexerboost::is_pointer< Lhs_noref >::value,\
         ::hexerboost::type_traits::ice_and<\
            ::hexerboost::is_fundamental< Rhs_nocv >::value,\
            ::hexerboost::type_traits::ice_not< ::hexerboost::is_convertible< Rhs_nocv, bool >::value >::value\
         >::value\
      >::value,\
      ::hexerboost::type_traits::ice_and<\
         ::hexerboost::is_pointer< Rhs_noref >::value,\
         ::hexerboost::type_traits::ice_and<\
            ::hexerboost::is_fundamental< Lhs_nocv >::value,\
            ::hexerboost::type_traits::ice_not< ::hexerboost::is_convertible< Lhs_nocv, bool >::value >::value\
         >::value\
      >::value\
   >::value


#include <boost/type_traits/detail/has_binary_operator.hpp>

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_FORBIDDEN_IF

#endif
