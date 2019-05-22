#include <iostream>
#include <type_traits>
#include <memory>
#include <utility>
#include <functional>
#include <algorithm>

template <typename T> class signal;
template <typename T> class slot;
template <typename T> class slot_ref;
template <typename T> void connect(signal<T>& sig, slot<T>& slo);
template <typename T> void disconnect(signal<T>& sig, slot<T>& slo);

template <typename T>
class signal {
public:
	~signal() {
		while(!slots.empty())
			disconnect(*this, *slots[0]);
	}

	void emit(T value) {
		for (auto slo : slots)
			slo->receive(value);
	}

	std::vector<slot<T>*> slots;
};

template <typename T>
class slot {
public:
	~slot() {
		while(!signals.empty())
			disconnect(*signals[0], *this);
	}

	void receive(T value) {
		this->value = value;
		std::cout << value << std::endl;
	}

	T value;

	std::vector<signal<T>*> signals;
};

template <typename T>
void connect(signal<T>& sig, slot<T>& slo)
{
	sig.slots.push_back(&slo);
	slo.signals.push_back(&sig);
}

template <typename T>
void disconnect(signal<T>& sig, slot<T>& slo)
{
	sig.slots.erase(std::remove(sig.slots.begin(), sig.slots.end(), &slo));
	slo.signals.erase(std::remove(slo.signals.begin(), slo.signals.end(), &sig));
}

int main()
{
	signal<int> sig;
	slot<int> slo;

	connect(sig, slo);

	sig.emit(2);

	return 0;
}
