import os
import subprocess
import json

print("This script will automate the following:")
print("  - Install vcpkg")
print("  - Bootstrap vcpkg")
print("  - Install vcpkg dependencies")
proceed = input("Continue? (Y/n) ")

if proceed == "n":
    print("Exiting...")
    exit(0)

cmake_config_file = open("cmake_config.json")
cmake_config = json.load(cmake_config_file)
cmake_config_file.close()

vcpkg_repo = "https://github.com/microsoft/vcpkg"
install_dir = "vcpkg"

subprocess.check_call(["git", "clone", vcpkg_repo, install_dir])

os.chdir(install_dir)
subprocess.check_call(["git", "checkout", "master"])
subprocess.check_call(["git", "pull"])

if os.name == "nt":
    subprocess.check_call([".\\bootstrap-vcpkg.bat"])
else:
    subprocess.check_call(["./bootstrap-vcpkg.sh"])

subprocess.check_call(["./vcpkg", "update"])

for pkg in cmake_config["packages"]:
    subprocess.check_call(["./vcpkg", "install", pkg])

subprocess.check_call(["./vcpkg", "update"])
