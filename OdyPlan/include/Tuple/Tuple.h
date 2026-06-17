#ifndef SIERRA_ODYPLAN_V1_OPERATOR_TUPLE_TUPLE_H
#define SIERRA_ODYPLAN_V1_OPERATOR_TUPLE_TUPLE_H

#include <tuple>
#include <type_traits>

#include "TupleField.h"

namespace Sierra::OdyPlan::v1::Operator::Tuple {
    template <typename T>
    struct is_std_tuple_of_odyfields : std::false_type {};

    template <typename... Ts>
    struct is_std_tuple_of_odyfields<std::tuple<Ts...>> : std::conjunction<is_tuple_field<Ts>...> {};

    template <typename T>
    inline constexpr bool is_std_tuple_of_odyfields_v = is_std_tuple_of_odyfields<std::remove_cvref_t<T>>::value;

    template <typename T>
    concept Tuple = is_std_tuple_of_odyfields_v<T>;

    template <TupleField... Ts>
    using TupleT = std::tuple<Ts...>;
} // Sierra::OdyPlan::v1::Operator::Tuple

#endif //SIERRA_ODYPLAN_V1_OPERATOR_TUPLE_TUPLE_H