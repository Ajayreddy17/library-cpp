#ifndef AJAY_CONSTRAINTS
#define AJAY_CONSTRAINTS

namespace mitsuha{
    template <typename ...Constraints> using constraints_t = std::enable_if_t<std::conjunction_v<Constraints...>, std::nullptr_t>;
    template <typename ReturnType, typename Callable, typename ...Args>
    using is_same_as_invoke_result = std::is_same<std::invoke_result_t<Callable, Args...>, ReturnType>;
}

#endif // AJAY_CONSTRAINTS