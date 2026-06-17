#ifndef SIERRA_ODYPLAN_V1_OPERATOR_TUPLE_TUPLEFIELD_H
#define SIERRA_ODYPLAN_V1_OPERATOR_TUPLE_TUPLEFIELD_H

#include <tuple>
#include <type_traits>

namespace Sierra::OdyPlan::v1::Operator::Tuple {
    template <typename T, auto Name>
    struct TupleFieldT {
        T value;
    };

    template <typename T>
    struct is_tuple_field : std::false_type {};

    template <typename T, auto Name>
    struct is_tuple_field<TupleFieldT<T,Name>> : std::true_type {};

    template <typename T>
    inline constexpr bool is_tuple_field_v = is_tuple_field<std::remove_cvref_t<T>>::value;

    template<typename T>
    concept TupleField = is_tuple_field_v<T>;
} // Sierra::OdyPlan::v1::Operator::Tuple

#endif //SIERRA_ODYPLAN_V1_OPERATOR_TUPLE_TUPLEFIELD_H