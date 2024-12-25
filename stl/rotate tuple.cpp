#include <tuple>

template <std::size_t K, typename Tuple, std::size_t... Is>
auto _rotate_tuple_impl(Tuple&& t, std::index_sequence<Is...>) {
    return std::make_tuple(std::move(std::get<(Is + K) % sizeof...(Is)>(t))...);
}

template <std::size_t K, typename... Args>
auto rotate_tuple(std::tuple<Args...>&& t) {
    return _rotate_tuple_impl<K>(std::move(t), std::make_index_sequence<sizeof...(Args)>{});
}


#include <iostream>

int main() {
    auto t = std::make_tuple(1, "abc", 3.2, 'c', true);
    auto rotated = rotate_tuple<3>(std::move(t));

    std::apply([](auto&&... args) {
        ((std::cout << args << " "), ...);
    }, rotated);

    return 0;
}
