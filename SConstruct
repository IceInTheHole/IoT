
import os

env = SConscript(['build/SConscript'])

Export('env')

srcs = [f for f in env.Glob("http/*.c")]
srcs += [f for f in env.Glob("string/*.c")]
srcs += [f for f in env.Glob("dictionary/*.c")]
srcs += [f for f in env.Glob("net/*.c")]

env.StaticLibrary('iot', srcs)
