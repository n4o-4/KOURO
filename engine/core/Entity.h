#pragma onece
#include <cstdint>

class Entity
{
public:

	explicit Entity(uint32_t id = 0) : id_(id) {}
	uint32_t GetID() const { return id_; }
	bool isValid() const { return id_ != 0; }

	bool operator==(const Entity& other) const { return id_ == other.id_; }
	bool operator!=(const Entity& other) const { return id_ != other.id_; }

private:

	uint32_t id_;

};