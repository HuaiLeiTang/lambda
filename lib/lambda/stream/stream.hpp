
#pragma once

namespace lambda {
namespace streams {

struct Pipeable {
    template <typename Arg, typename Pipe>
    static auto pipe(Arg &&arg, Pipe pipe) {
        return pipe(std::forward<Arg>(arg));
    }
};

template <typename T>
struct is_pipeable : std::is_base_of<Pipeable, T> {};

////////////////////

struct Stream {};

template <typename T>
struct is_stream : std::is_base_of<Stream, T> {};

template <class C>
class CollectionStream : public Stream {
    C collection;
    typename C::const_iterator begin, end;

public:
    using Type = typename C::value_type;

    CollectionStream(C &&collection)
        : collection(collection), begin(collection.cbegin()), end(collection.cend()) {}

    Maybe<Type> next() { return begin != end ? some(std::move(*begin++)) : none; }
};

template <class C, REQUIRE_CONCEPT(has_iterator_v<C>)>
auto stream(C &&c) {
    return CollectionStream<C>{std::forward<C>(c)};
}

////////////////////

template <typename Arg, typename Pipe,
          REQUIRE_CONCEPT(!is_pipeable<Arg>() && is_stream<Arg>() && is_pipeable<Pipe>())>
auto operator|(Arg &&arg, Pipe pipe) {
    return Pipe::pipe(std::forward<Arg>(arg), pipe);
}

} /* streams */
} /* lambda */
