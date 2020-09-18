#include <new> // for std::launder()

template<typename... Types>
class VariantStorage {
	using LargestT = LargestType<Typelist<Types...>>;
	alignas(Types...) unsigned char buffer[sizeof(LargestT)];
	unsigned char discriminator = 0;
public:
	unsigned char getDiscriminator() const { return discriminator; }
	void setDiscriminator(unsigned char d) { discriminator = d; }
	void* getRawBuffer() { return buffer; }
	const void* getRawBuffer() const { return buffer; }

	template<typename T>
	T* getBufferAs() { return std::launder(reinterpret_cast<T*>(buffer)); }

	template<typename T>
	const T* getBufferAs() const
	{
		return std::launder(reinterpret_cast<const T*>(buffer));
	}
};

template<typename T, typename... Types>
class VariantChoice {
	using Derived = Variant<Types...>;
	Derived& getDerived() { return *static_cast<Derived*>(this); }
	const Derived& getDerived() const
	{
		return *static_cast<const Derived*>(this);
	}
protected:
	constexpr static unsigned Discriminator =
		FindIndexOfT<Typelist<Types...>, T>::value + 1;
public:
	VariantChoice() {}
	// see variantchoiceinit.hpp
	VariantChoice(const T& value);
	VariantChoice(T&& value);

	bool destroy(); // see variantchoicedestroy.hpp
	// see variantchoiceassign.hpp
	Derived& operator=(const T& value);
	Derived& operator=(T&& value);
};

template<typename List, typename T, unsigned N = 0,
	bool Empty = IsEmpty<List>::value>
	struct FindIndexOfT;

// recursive case:
template<typename List, typename T, unsigned N>
struct FindIndexOfT<List, T, N, false>
	: public IfThenElse<std::is_same<Front<List>, T>::value,
	std::integral_constant<unsigned, N>,
	FindIndexOfT<PopFront<List>, T, N + 1>>
{};

// basis case:
template<typename List, typename T, unsigned N>
struct FindIndexOfT<List, T, N, true>
{};


template<typename... Types>
class Variant : private VariantStorage<Types...>,
	private VariantChoice<Types, Types...>...
{
	template<typename T, typename... OtherTypes>
	friend class VariantChoice;
public:
	template<typename T> bool is() const; // see variantis.hpp

	// see variantget.hpp
	template<typename T> T& get()&;
	template<typename T> const T& get() const&;
	template<typename T> T&& get()&&;

	// see variantvisit.hpp:
	template<typename R = ComputedResultType, typename Visitor>
	VisitResult<R, Visitor, Types&...> visit(Visitor&& vis)&;

	template<typename R = ComputedResultType, typename Visitor>
	VisitResult<R, Visitor, const Types&...> visit(Visitor&& vis) const&;

	template<typename R = ComputedResultType, typename Visitor>
	VisitResult<R, Visitor, Types&&...> visit(Visitor&& vis)&&;

	using VariantChoice<Types, Types...>::VariantChoice...;

	Variant(); // see variantdefaultctor.hpp
	Variant(const Variant& source); // see variantcopyctor.hpp
	Variant(Variant&& source); // see variantmovector.hpp

	template<typename... SourceTypes>
	Variant(const Variant<SourceTypes...>& source); // see variantcopyctortmpl.hpp

	template<typename... SourceTypes>
	Variant(Variant<SourceTypes...>&& source); // see variantmovectortmpl.hpp

	using VariantChoice<Types, Types...>::operator=...;

	Variant& operator= (const Variant& source); // see variantcopyassign.hpp
	Variant& operator= (Variant&& source); // see variantmoveassign.hpp

	template<typename... SourceTypes>
	Variant& operator= (const Variant<SourceTypes...>& source); // see variantcopyassigntmpl.hpp

	template<typename... SourceTypes>
	Variant& operator= (Variant<SourceTypes...>&& source); // see variantmoveassigntmpl.hpp

	bool empty() const; // see variantempty.hpp

	~Variant() { destroy(); }
	void destroy(); // see variantdestroy.hpp
};

Variant<int, double, std::string>