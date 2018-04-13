
#include "IExecutorData.h"

using namespace ignis::executor::core;

IExecutorData::IExecutorData() : Properties_parser(IPropertiesParser(context.getProperties())) {}
