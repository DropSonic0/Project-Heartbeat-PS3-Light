#!/usr/bin/env python
import os
import sys

opts = Variables([], ARGUMENTS)
opts.add(EnumVariable("platform", "Target platform", sys.platform, ["linux", "windows", "macos", "android", "ios"]))
opts.add(EnumVariable("target", "Compilation target", "debug", ["debug", "release"]))
opts.add(BoolVariable("use_llvm", "Use the LLVM compiler", False))
opts.add(EnumVariable("arch", "CPU architecture", "", ["", "x86_32", "x86_64", "arm32", "arm64", "rv64"]))

env = Environment(variables=opts)
Help(opts.GenerateHelpText(env))

if env["platform"] in ["linux", "windows"] and not env["arch"]:
    env["arch"] = "x86_64"

# Load godot-cpp SConstruct
SConscript("godot-cpp/SConstruct", exports="env")

# Add the source directory to the include path.
env.Append(CPPPATH=["src/"])

sources = Glob("src/*.cpp") + Glob("src/**/*.cpp")

library = env.SharedLibrary(
    "addons/project_heartbeat/bin/libproject_heartbeat{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

# Instructions to link against godot-cpp
# This is usually handled by the godot-cpp SConstruct exports,
# but ensure we have the necessary flags if needed.

Default(library)
