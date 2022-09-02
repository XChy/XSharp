#pragma once
#include "xsharp_global.h"
#include <atomic>

class XSharp_EXPORT XRefCount {
public:
	XRefCount()
		:mRefCount(1)
	{}
	XRefCount(const XRefCount& other)
		:mRefCount(other.refCount())
	{}
	int ref() {
		return ++mRefCount;
	}
	int unref() {
		return --mRefCount;
	}
	int refCount() const
	{
		return mRefCount;
	}
private:
	std::atomic<uint> mRefCount;
};

template<typename T>
class XSharp_EXPORT XSharedData {
public:
	XSharedData()
		:ref(new XRefCount),
		mData(nullptr)
	{}
	XSharedData(T* p)
		:ref(new XRefCount),
		mData(p)
	{}
	XSharedData(const XSharedData& other)
		:ref(other.ref),
		mData(other.data())
	{
		ref->ref();
	}
	XSharedData& operator=(const XSharedData& other)
	{
		if (this == &other) {
			return *this;
		}
		if (ref->unref() == 0) {
			delete ref;
			delete mData;
		}
		ref = other.ref;
		mData = other.data();
		ref->ref();
		return *this;
	}
	T* data() const
	{
		return mData;
	}
	~XSharedData()
	{
		if (ref->unref() == 0)
		{
			delete ref;
			delete mData;
		}
	}

	XRefCount* ref;
	T* mData;
};
