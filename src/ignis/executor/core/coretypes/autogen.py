import os.path
import sys
import re

TYPE = """
#include "ignis/executor/core/selector/ISelector.h"
{header}

std::shared_ptr<ignis::executor::core::selector::ITypeSelector> {var_name} = std::make_shared<ignis::executor::core::selector::ITypeSelectorImpl<{var_type}>>();

"""

FUNCTIONS = """
#include "ignis/executor/api/function/IBeforeFunction.h"
#include "ignis/executor/api/function/IFunction.h"
#include "ignis/executor/api/function/IFunction0.h"
#include "ignis/executor/api/function/IFunction2.h"
#include "ignis/executor/api/function/IVoidFunction.h"
#include "ignis/executor/api/function/IVoidFunction0.h"
#include "ignis/executor/api/function/IVoidFunction2.h"
{header}

{src_code}

std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> {var_name} = 
  std::make_shared<ignis::executor::core::selector::ISelectorGroupImpl<{var_type}>>("{var_type}");
"""

ROOT = """
#include "ignis/executor/core/ICoreTypes.h"

{extern}

std::vector<std::shared_ptr<ignis::executor::core::selector::ITypeSelector>> ignis::executor::core::ICoreTypes::defaultTypes() {{
    return {{
        {type_list}
    }};
}}

std::vector<std::shared_ptr<ignis::executor::core::selector::ISelectorGroup>> ignis::executor::core::ICoreTypes::defaultFunctions() {{
    return {{
        {function_list}
    }};
}}
"""

cpp_header = list()
cpp_types = list()
cpp_functions = list()

if os.path.exists("header"):
	with open("header") as file:
		for line in file:
			if line and not line.startswith("//"):
				cpp_header.append(line.strip())

if os.path.exists("types"):
	with open("types") as file:
		for line in file:
			if line and not line.startswith("//"):
				cpp_types.append(line.strip())

if os.path.exists("functions"):
	with open("functions") as file:
		name = None
		code = ""
		init = re.compile("(class|struct)([^:]+):.*")
		for line in file:
			if line.startswith("//"):
				continue
			if name is None:
				pattern = init.match(line)
				if pattern:
					name = pattern.group(2).strip()
					code = line
				continue
			else:
				code += line
				if line.startswith("};"):
					cpp_functions.append((name, code))
					name = None

wd = os.path.dirname(sys.argv[0])
path = os.path.join(wd, "autogen/")
if os.path.exists(path):
	for file in os.listdir(path):
		os.remove(os.path.join(path, file))
	os.rmdir(path)
os.mkdir(path)
str_header = ""
for line in cpp_header:
	str_header += line + "\n"

extern = ""
types = list()
for line in cpp_types:
	name = "type" + (str(len(types)).zfill(3))
	extern += "extern std::shared_ptr<ignis::executor::core::selector::ITypeSelector> " + name + ";\n"
	with open(path + name + ".cpp", "w") as file:
		file.write(TYPE.format(header=str_header, var_name=name, var_type=line))
	types.append(name)

funcs = list()
for fid, code in cpp_functions:
	name = "function" + (str(len(funcs)).zfill(3))
	extern += "extern std::shared_ptr<ignis::executor::core::selector::ISelectorGroup> " + name + ";\n"
	with open(path + name + ".cpp", "w") as file:
		file.write(FUNCTIONS.format(header=str_header, var_name=name, var_type=fid, src_code=code))
	funcs.append(name)

with open(path + "ICoreTypes.cpp", "w") as file:
	file.write(ROOT.format(extern=extern, type_list=", ".join(types), function_list=", ".join(funcs)))
