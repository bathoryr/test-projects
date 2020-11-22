#pragma once

template <typename T>
class buffer {
    T* _buf;
	size_t _head;
    size_t _size;

    public:
    buffer(size_t size = 32) {
        _buf = new T[size];
        _head = 0;
        _size = size;
    }

    ~buffer() {
        delete [] _buf;
    }

    void add(T item) {
        _buf[_head] = item;
        if (++_head >= _size)
            _head = 0;
    }

    T get(size_t pos) {
        return _buf[pos];
    }

	size_t size() {
        return _size;
    }

	size_t head() {
        return _head;
    }

	void reset(T init_val = 0) {
		for (size_t i = 0; i < _size; ++i)
		{
			_buf[i] = init_val;
		}
		_head = 0;
	}

    int GetAvgVal() {
        T val = 0;
        for(size_t i = 0; i < _size; i++) {
            val += _buf[i];
        }
        return val / _size;
    }
};
