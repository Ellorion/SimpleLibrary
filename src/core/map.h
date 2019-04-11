///
/// @Obsolete since custom Array_Find implementation
///

#pragma once

template <typename K, typename V>
struct Pair {
	K key;
	V value;
};

template <typename K, typename V>
struct Map {
	Array<Pair<K, V>> a_data;

	bool is_key_overwriteable = false;
};

template <typename K, typename V>
instant bool
Map_Find(
	Map<K, V> *map,
	K key,
	u64 *index = 0
) {
	Assert(map);

	FOR_ARRAY(map->a_data, it) {
		Pair<K, V> *t_pair = &ARRAY_IT(map->a_data, it);

		if (t_pair->key == key) {
			if (index)
				*index = it;

			return true;
		}
	}

	return false;
}

template <typename K, typename V>
instant void
Map_Update(
	Map<K, V> *map,
	K key,
	V value
) {
	Assert(map);

	u64  index;
	bool found = Map_Find(map, key, &index);

	if (found) {
		Pair<K, V> *t_pair = &ARRAY_IT(map->a_data, index);
		t_pair->value = value;
	}
	else {
		AssertMessage(false, "[Map] Update map entry failed. Key not found.");
	}
}

template <typename K, typename V>
instant void
Map_Set(
	Map<K, V> *map,
	K key,
	V value
) {
	Assert(map);

	u64  index;
	bool found = Map_Find(map, key, &index);

	if (found) {
		if (!map->is_key_overwriteable) {
			Pair<K, V> *t_pair = &ARRAY_IT(map->a_data, index);
			t_pair->value = value;
		}
		else {
			AssertMessage(false, "[Map] Unique key values must be updated.");
		}
	}
	else {
		Pair<K, V> *t_pair;
		Array_AddEmpty(&map->a_data, &t_pair);

		t_pair->key   = key;
		t_pair->value = value;
	}
}

template <typename K, typename V>
instant bool
Map_Get(
	Map<K, V> *map,
	K key,
	V *value
) {
	Assert(map);
	Assert(value);

	u64  index;
	bool found = Map_Find(map, key, &index);

	if (found) {
		Pair<K, V> *t_pair = &ARRAY_IT(map->a_data, index);
		*value = t_pair->value;
	}

	return found;
}
