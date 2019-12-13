#pragma once
#include "Exceptions.h"

class OptionalDouble {
private:
	double value;
	bool hasValue;
public:
	double getValue() {
		if (hasValue)
			return value;
		else throw BadOptionalAccess();
	}
	void setValue(double value) {
		this->value = value;
		this->hasValue = true;
	}
	bool contains() {
		return hasValue;
	}
	OptionalDouble(double value) : value(value), hasValue(true) {};
	OptionalDouble() : value(0), hasValue(false) {};

	OptionalDouble& operator+=(const OptionalDouble& b) {
		if (!hasValue) {
			return *this;
		}
		value += b.value;
		return *this;
	}
	OptionalDouble& operator-=(const OptionalDouble& b) {
		if (!hasValue) {
			return *this;
		}
		value -= b.value;
		return *this;
	}
	OptionalDouble& operator*=(const OptionalDouble& b) {
		if (!hasValue) {
			return *this;
		}
		value *= b.value;
		return *this;
	}
	OptionalDouble& operator/=(const OptionalDouble& b) {
		if (!hasValue) {
			return *this;
		}
		value /= b.value;
		return *this;
	}
};