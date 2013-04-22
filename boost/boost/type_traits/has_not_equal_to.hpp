//  (C) Copyright 2009-2011 Frederic Bron.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_HAS_NOT_EQUAL_TO_HPP_INCLUDED
#define BOOST_TT_HAS_NOT_EQUAL_TO_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_not_equal_to
#define BOOST_TT_TRAIT_OP !=
#define BOOST_TT_FORBIDDEN_IF\
   ::hexerboost::type_traits::ice_or<\
      /* Lhs==pointer and Rhs==fundamental */\
      ::hexerboost::type_traits::ice_and<\
         ::hexerboost::is_pointer< Lhs_noref >::value,\
         ::hexerboost::is_fundamental< Rhs_nocv >::value\
      >::value,\
      /* Rhs==pointer and Lhs==fundamental */\
      ::hexerboost::type_traits::ice_and<\
         ::hexerboost::is_pointer< Rhs_noref >::value,\
         ::hexerboost::is_fundamental< Lhs_nocv >::value\
      >::value,\
      /* Lhs==pointer and Rhs==pointer and Lhs!=base(Rhs) and Rhs!=base(Lhs) and Lhs!=void* and Rhs!=void* */\
      ::hexerboost::type_traits::ice_and<\
         ::hexerboost::is_pointer< Lhs_noref >::value,\
         ::hexerboost::is_pointer< Rhs_noref >::value,\
         ::hexerboost::type_traits::ice_not<\
            ::hexerboost::type_traits::ice_or<\
               ::hexerboost::is_base_of< Lhs_noptr, Rhs_noptr >::value,\
               ::hexerboost::is_base_of< Rhs_noptr, Lhs_noptr >::value,\
               ::hexerboost::is_same< Lhs_noptr, Rhs_noptr >::value,\
               ::hexerboost::is_void< Lhs_noptr >::value,\
               ::hexerboost::is_void< Rhs_noptr >::value\
            >::value\
         >::value\
      >::value\
   >::value


#include <boost/type_traits/detail/has_binary_operator.hpp>

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_FORBIDDEN_IF

#endif
