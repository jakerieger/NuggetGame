import json
import os
import shutil

configFile = open("cmake_config.json")
config = json.load(configFile)
configFile.close()

vcpkg_root = config["vcpkg_root"]
generator = config["generator"]

if os.path.exists("build") and os.path.isdir("build"):
    shutil.rmtree("build")

os.mkdir("build")

cmake_debug = f"cmake -B build/Debug -G \"{generator}\" -DCMAKE_TOOLCHAIN_FILE={vcpkg_root} -DCMAKE_BUILD_TYPE=Debug"
cmake_release = f"cmake -B build/Release -G \"{generator}\" -DCMAKE_TOOLCHAIN_FILE={vcpkg_root} -DCMAKE_BUILD_TYPE=Release"
exit_code = os.system(cmake_debug)
if exit_code != 0:
    exit(exit_code)
exit_code = os.system(cmake_release)
if exit_code != 0:
    exit(exit_code)