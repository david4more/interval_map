
#include <map>
#include <iostream>

template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K, V> m_map;
public:
	template<typename V_forward>
	interval_map(V_forward&& val)
		: m_valBegin(std::forward<V_forward>(val))
	{
	}

	template<typename V_forward>
	void assign(K const& keyBegin, K const& keyEnd, V_forward&& val)
		requires (std::is_same<std::remove_cvref_t<V_forward>, V>::value)
	{
		if (!(keyBegin < keyEnd))
			return;

		if (m_map.empty())							
		{
			if (m_valBegin != val)
				m_map.emplace(keyBegin, std::forward<V_forward>(val));
			
			return;
		}

		auto beginIt = m_map.lower_bound(keyBegin);		
		if (beginIt == m_map.end())
		{
			if (std::prev(beginIt)->second != val)
				m_map.emplace(keyBegin, std::forward<V_forward>(val));

			return;
		}

		auto endIt = m_map.upper_bound(keyEnd);		
		if (endIt == m_map.begin())
		{
			if (val != m_valBegin)
				m_map.emplace(keyBegin, std::forward<V_forward>(val));

			return;
		}

		if (beginIt == m_map.begin() && endIt == m_map.end())
		{
			if (!(std::prev(endIt)->first < keyEnd))
			// Check if the key of the last element is equal, which may occur due to using upper_bound.
				m_map.erase(beginIt, std::prev(endIt));
			else
				m_map.erase(beginIt, endIt);

			if (val != m_valBegin)
				m_map.emplace(keyBegin, std::forward<V_forward>(val));

			return;
		}

		if (beginIt == m_map.begin())
		{
			auto temp = std::prev(endIt)->second;
			m_map.erase(beginIt, endIt);
			if (val != temp)
				m_map.emplace(keyEnd, std::forward<V_forward>(temp));
			
			if (val != m_valBegin)
				m_map.emplace(keyBegin, std::forward<V_forward>(val));

			return;
		}
		
		if (endIt == m_map.end())
		{
			auto temp = std::prev(beginIt)->second;
			m_map.erase(beginIt, endIt);

			if (val != temp)
				m_map.emplace(keyBegin, std::forward<V_forward>(val));

			return;
		}

		auto tempB = std::prev(beginIt)->second;
		auto tempE = std::prev(endIt)->second;
		m_map.erase(beginIt, endIt);
		if (val != tempE)
			m_map.emplace(keyEnd, std::forward<V_forward>(tempE));
		if (val != tempB)
			m_map.emplace(keyBegin, std::forward<V_forward>(val));
	}

	// look-up of the value associated with key
	V const& operator[](K const& key) const {
		auto it = m_map.upper_bound(key);
		if (it == m_map.begin()) {
			return m_valBegin;
		}
		else {
			return (--it)->second;
		}
	}
};

void IntervalMapTest()
{
	int x = 0, y;
	char z;
	interval_map<int, char> m('D');
	m.assign(0, 4, 'a');
	m.assign(5, 9, 'b');
	m.assign(10, 14, 'c');
	m.assign(15, 19, 'd');
	m.assign(7, 14, 'x');

	std::cout << "\n\n";
	for (auto x : m.m_map)
		std::cout << x.first << ' ' << x.second << '\n';
	std::cout << "\nD " << m.m_valBegin << "\n";
}
int main()
{
	IntervalMapTest();
	return 0;
}