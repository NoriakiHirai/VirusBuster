#include "Singleton.h"

const int MAX_FINALIZER_SIZE = 256;	// 管理オブジェクトの上限
static int finalizerCount = 0;

static SingletonFinalizer::FinalizerFunc finalizer[MAX_FINALIZER_SIZE];

void SingletonFinalizer::addFinalizer(FinalizerFunc func)
{
	assert(finalizerCount < MAX_FINALIZER_SIZE);
	finalizer[finalizerCount++] = func;
}

void SingletonFinalizer::finalize()
{
	for (auto i = finalizerCount - 1; i >= 0; --i)
	{
		(*finalizer[i])();
	}
	finalizerCount = 0;
}
